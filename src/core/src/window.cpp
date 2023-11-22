
#include "battery/application.hpp"
#include "battery/window.hpp"
#include "battery/fs.hpp"
#include "battery/exception.hpp"

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"
#include "SDL_image.h"

#ifdef B_OS_WINDOWS
#include <Windows.h>
#include <dwmapi.h>
#endif

namespace b {

    b::Vec2 getPrimaryMonitorSize() {
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        return b::Vec2(DM.w, DM.h);
    }

    Window::Window() {
        m_windowPositionJsonFilePath = b::Folders::AppConfigDir() / "windowposition.cache";
    }

    Window::~Window() noexcept {
        destroy();
    }

    b::Application& Window::app() const {
        return b::Application::get();
    }

    void Window::create(const std::string& title, const b::Vec2& size, std::optional<WindowOptions> options) {

        // Check if a window already exists
        if (m_windowExists) {
            throw std::runtime_error("b::Window: The ImGui SDL backend used by battery does not "
                                     "support multiple windows yet and a window already exists.");
        }
        m_windowExists = true;

        // Load options
        if (options) {
            m_options = *options;
            m_win32IDMActive = !m_options.useWin32ImmersiveDarkMode;
        }

        // Calculate window size and position
        b::Vec2 newSize = size;
        b::Vec2 newPosition = (getPrimaryMonitorSize() - size) / 2.0;
        bool maximized = false;

        // Restore the previous window position and size
        if (m_options.rememberWindowPosition) {
            if (loadCachedWindowState()) {
                newPosition = m_lastWindowState.position;
                newSize = m_lastWindowState.size;
                maximized = m_lastWindowState.maximized;
            }
        }

        // Setup SDL window flags
        int flags = 0;
        if (m_options.startAsVisible) {
            flags |= SDL_WINDOW_SHOWN;
        }
        else {
            flags |= SDL_WINDOW_HIDDEN;
        }
        if (m_options.resizable) {
            flags |= SDL_WINDOW_RESIZABLE;
        }

        // Create the SDL window
        m_sdlWindow = SDL_CreateWindow(
                title.c_str(),
                static_cast<int>(newPosition.x),
                static_cast<int>(newPosition.y),
                static_cast<int>(newSize.x),
                static_cast<int>(newSize.y),
                flags | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI
        );
        m_sdlWindowID = SDL_GetWindowID(m_sdlWindow);
        m_windowIDs.insert({ m_sdlWindowID, *this });
        updateWin32DarkMode();

        if (maximized) {

        }

        // setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
//        io.Fonts->AddFontFromFileTTF("verdana.ttf", 18.0f, NULL, NULL);

//        setImGuiStyle();

        // Setup SDL renderer
        m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow, -1,
                                           SDL_RENDERER_ACCELERATED);
        if (m_sdlRenderer == nullptr) {
            throw std::runtime_error(b::format("b::Window: Failed to create SDL renderer: {}",
                                               SDL_GetError()));
        }

        // Setup SDL OpenGL context
        m_sdlGLContext = SDL_GL_CreateContext(m_sdlWindow);
        if (m_sdlGLContext == nullptr) {
            throw std::runtime_error(b::format("b::Window: Failed to create SDL OpenGL context: {}",
                                               SDL_GetError()));
        }

        ImGui_ImplSDL2_InitForOpenGL(m_sdlWindow, m_sdlGLContext);
        ImGui_ImplOpenGL3_Init();

        // Load default battery icon
        setIcon(b::Resource::FromBase64(b::Constants::BatteryIconBase64()));
    }

    void Window::destroy() {
        if (m_sdlWindow) {

            ImGui_ImplOpenGL3_Shutdown();
            SDL_GL_DeleteContext(m_sdlGLContext);

            ImGui_ImplSDL2_Shutdown();
            SDL_DestroyRenderer(m_sdlRenderer);

            ImGui::DestroyContext();

            if (m_options.rememberWindowPosition) {
                writeCachedWindowState();
            }

            m_windowIDs.erase(m_sdlWindowID);
            SDL_DestroyWindow(m_sdlWindow);
            m_sdlWindow = nullptr;
        }
    }

    void Window::setWindowPositionJsonFilePath(const b::fs::path& filename) {
        if (isOpen()) {
            b::log::warn("b::Window::setWindowPositionJsonFilePath() called after window was already created. "
                         "This will not have any effect on the current window.");
        }
        m_windowPositionJsonFilePath = filename;
    }

    void Window::showInTaskbar() {
#ifdef B_OS_WINDOWS
        int32_t style = GetWindowLongW(getSystemHandle(), GWL_STYLE);
        style &= ~(WS_VISIBLE);
        SetWindowLongW(getSystemHandle(), GWL_STYLE, style);
        ShowWindow(getSystemHandle(), SW_SHOW);
#else
#warning "Not implemented"
#endif
    }

    void Window::hideFromTaskbar() {
#ifdef B_OS_WINDOWS
        long style = GetWindowLongW(getSystemHandle(), GWL_STYLE);
        style |= WS_VISIBLE;
        ShowWindow(getSystemHandle(), SW_HIDE);
        SetWindowLongW(getSystemHandle(), GWL_STYLE, style);
        ShowWindow(getSystemHandle(), SW_SHOW);
#else
#warning "Not implemented"
#endif
    }

    void Window::maximize() {
#ifdef B_OS_WINDOWS
        ShowWindow(getSystemHandle(), SW_MAXIMIZE);
#else
#warning "Not implemented"
#endif
    }

    void Window::minimize() {
#ifdef B_OS_WINDOWS
        ShowWindow(getSystemHandle(), SW_MINIMIZE);
#else
#warning "Not implemented"
#endif
    }

    void Window::restore() {
#ifdef B_OS_WINDOWS
        ShowWindow(getSystemHandle(), SW_RESTORE);
#else
#warning "Not implemented"
#endif
    }

    bool Window::isMaximized() {
#ifdef B_OS_WINDOWS
        return IsZoomed(getSystemHandle()) != 0;
#else
#warning "Not implemented"
#endif
    }

    bool Window::isMinimized() {
#ifdef B_OS_WINDOWS
        return IsIconic(getSystemHandle()) != 0;
#else
#warning "Not implemented"
#endif
    }

    bool Window::setIcon(const b::Resource& icon) {
        SDL_RWops* rwops = SDL_RWFromMem((void*)(icon.data()), icon.size());
        if (!rwops) {
            return false;
        }

        SDL_Surface* surface = IMG_LoadPNG_RW(rwops);
        if (!surface) {
            SDL_RWclose(rwops);
            return false;
        }

        SDL_SetWindowIcon(m_sdlWindow, surface);

        SDL_FreeSurface(surface);
        SDL_RWclose(rwops);
        return true;
    }

    void Window::setPosition(const b::Vec2& position) {
        if (!m_sdlWindow) {
            throw b::window_not_created_error();
        }
        SDL_SetWindowPosition(m_sdlWindow, static_cast<int>(position.x), static_cast<int>(position.y));
    }

    b::Vec2 Window::getPosition() const {
        if (!m_sdlWindow) {
            throw b::window_not_created_error();
        }
        int x = 0;
        int y = 0;
        SDL_GetWindowPosition(m_sdlWindow, &x, &y);
        return b::Vec2(x, y);
    }

    void Window::setSize(const b::Vec2& size) {
        if (!m_sdlWindow) {
            throw b::window_not_created_error();
        }
        SDL_SetWindowSize(m_sdlWindow, static_cast<int>(size.x), static_cast<int>(size.y));
    }

    b::Vec2 Window::getSize() const {
        if (!m_sdlWindow) {
            throw b::window_not_created_error();
        }
        int x = 0;
        int y = 0;
        SDL_GetWindowSize(m_sdlWindow, &x, &y);
        return b::Vec2(x, y);
    }

    void Window::focus() {
        if (!m_sdlWindow) {
            throw b::window_not_created_error();
        }
        SDL_RaiseWindow(m_sdlWindow);
        SDL_SetWindowInputFocus(m_sdlWindow);
    }

    bool Window::hasFocus() const {
        if (!m_sdlWindow) {
            throw b::window_not_created_error();
        }
        auto flags = SDL_GetWindowFlags(m_sdlWindow);
        return flags & SDL_WINDOW_INPUT_FOCUS || flags & SDL_WINDOW_MOUSE_FOCUS;
    }

    bool Window::isOpen() const {
        return m_sdlWindow != nullptr;
    }

    void Window::setTitle(const std::string& title) {
        if (!m_sdlWindow) {
            throw b::window_not_created_error();
        }
        SDL_SetWindowTitle(m_sdlWindow, title.c_str());
    }

    HWND Window::getSystemHandle() {
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(m_sdlWindow, &wmInfo);

#ifdef B_OS_WINDOWS
        return wmInfo.info.win.window;
#else
#error not implemented
#endif
    }

    void Window::update() {
        updateWin32DarkMode();
    }

//    void Window::invokeUpdate() {
//
//
//        mouse.setScrollDelta({});   // Reset scroll to zero
//        processWindowEvents();
//        mouse.updatePositionData(sf::Mouse::getPosition(getRenderWindow()));
//        mouse.updateButtonData();
//        b::update_themes();     // TODO: Move this to a better place
//
//        try {
//            onUpdate();
//            prepareFrontendScript();
//        }
//        catch (const std::exception &e) {
//            recoverImGuiStacks();
//            m_errorMessage = e.what();
//            b::log::core::error("Unhandled exception:\n{}", e.what());
//        }
//
//        if (isOpen()) {
//            m_lastWindowState.position = getPosition();
//            m_lastWindowState.size = getSize();
//            m_lastWindowState.maximized = isMaximized();
//        }
//    }

//    void Window::invokeRender() {
//        clear(b::Constants::DefaultWindowBackgroundColor());
//        ImGui::SFML::Update(getRenderWindow(), m_deltaClock.restart());
//
//        b::LockFontStack();
//        renderContent();
//        b::UnlockFontStack();
//
//        ImGui::SFML::Render(getRenderWindow());
//        getRenderWindow().display();
//    }

//    void Window::recoverImGuiStacks() {
//        ImGui::ErrorCheckEndFrameRecover(nullptr);
//        ImGuiContext& g = *GImGui;
//
//        if (g.CurrentWindowStack.empty()) {
//            return;
//        }
//
//        ImGuiStackSizes* stackSizes = &g.CurrentWindowStack.back().StackSizesOnBegin;
//        while (g.FontStack.Size > stackSizes->SizeOfFontStack) {
//            ImGui::PopFont();
//        }
//    }

    bool Window::processSDLEvent(SDL_Event* event) {
        return ImGui_ImplSDL2_ProcessEvent(event);
    }

    bool Window::processSDLWindowEvent(SDL_Event* event) {
        switch (event->window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                if (!dispatchEvent<WindowCloseEvent>()) {
                    app().close();
                }
                return true;

            case SDL_WINDOWEVENT_RESIZED:
                dispatchEvent<WindowResizeEvent>(b::Vec2(event->window.data1, event->window.data2));
                return true;

            case SDL_WINDOWEVENT_FOCUS_LOST:
                dispatchEvent<WindowLostFocusEvent>();
                return true;

            case SDL_WINDOWEVENT_FOCUS_GAINED:
                dispatchEvent<WindowGainedFocusEvent>();
                return true;

            case SDL_WINDOWEVENT_MAXIMIZED:
                dispatchEvent<WindowMaximizedEvent>();
                return true;

            case SDL_WINDOWEVENT_MINIMIZED:
                dispatchEvent<WindowMinimizedEvent>();
                return true;

            case SDL_WINDOWEVENT_RESTORED:
                dispatchEvent<WindowRestoredEvent>();
                return true;

            case SDL_WINDOWEVENT_MOVED:
                dispatchEvent<WindowMovedEvent>(b::Vec2(event->window.data1, event->window.data2));
                return true;

            case SDL_WINDOWEVENT_HIDDEN:
                dispatchEvent<WindowHiddenEvent>();
                return true;

            case SDL_WINDOWEVENT_SHOWN:
                dispatchEvent<WindowShownEvent>();
                return true;

            case SDL_WINDOWEVENT_EXPOSED:
                dispatchEvent<WindowExposedEvent>();
                return true;

            case SDL_WINDOWEVENT_ENTER:
                dispatchEvent<MouseEnteredWindowEvent>();
                return true;

            case SDL_WINDOWEVENT_LEAVE:
                dispatchEvent<MouseLeftWindowEvent>();
                return true;
        }
        return false;
    }

    bool Window::loadCachedWindowState() {
        if (m_windowPositionJsonFilePath.empty()) {
            return false;
        }

        if (!b::fs::exists(m_windowPositionJsonFilePath)) {
            return false;
        }

        return glz::read_json(m_lastWindowState, b::fs::read(m_windowPositionJsonFilePath));
    }

    bool Window::writeCachedWindowState() {
        return b::fs::write(m_windowPositionJsonFilePath, glz::write_json(m_lastWindowState));
    }

    void Window::updateWin32DarkMode() {
        // bool const useWin32ImmersiveDarkMode = b::style::get<bool>("b::window.win32ImmersiveDarkMode"); // TODO: <- Interesting Copilot suggestion
#ifdef B_OS_WINDOWS
        if (m_options.useWin32ImmersiveDarkMode != m_win32IDMActive) {
            BOOL useDarkMode = static_cast<BOOL>(m_options.useWin32ImmersiveDarkMode);
            bool const setImmersiveDarkModeSuccess = SUCCEEDED(DwmSetWindowAttribute(
                    getSystemHandle(), DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
                    &useDarkMode, sizeof(useDarkMode)));
            m_win32IDMActive = m_options.useWin32ImmersiveDarkMode;
            if (!setImmersiveDarkModeSuccess) {
                throw std::runtime_error("Failed to set immersive dark mode on b::window");
            }
        }
#endif
    }

} // namespace b