
#include "battery/window.hpp"
#include "battery/fs.hpp"
#include "battery/exception.hpp"

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
        m_windowIDs.insert({ SDL_GetWindowID(m_sdlWindow), *this });
    }

    Window::~Window() noexcept {
        m_windowIDs.erase(SDL_GetWindowID(m_sdlWindow));

        if (m_options.rememberWindowPosition) {
            writeCachedWindowState();
        }

        close();
    }

    b::Application& Window::app() const {
        return b::Application::get();
    }

    void Window::create(const std::string& title, const b::Vec2& size, std::optional<WindowOptions> options) {

        b::Vec2 newSize = size;
        b::Vec2 newPosition = (getPrimaryMonitorSize() - size) / 2.0;
        bool maximized = false;

        if (options) {
            m_options = *options;
            m_win32IDMActive = !m_options.useWin32ImmersiveDarkMode;
        }

        // Restore the previous window position and size
        if (m_options.rememberWindowPosition) {
            if (loadCachedWindowState()) {
                newPosition = m_lastWindowState.position;
                newSize = m_lastWindowState.size;
                maximized = m_lastWindowState.maximized;
            }
        }

        int flags = 0;
        if (m_options.startAsVisible) {
            flags |= SDL_WINDOW_SHOWN;
        }
        else {
            flags |= SDL_WINDOW_HIDDEN;
        }

        // Create the window
        m_sdlWindow = SDL_CreateWindow(
                title.c_str(),
                static_cast<int>(newPosition.x),
                static_cast<int>(newPosition.y),
                static_cast<int>(newSize.x),
                static_cast<int>(newSize.y),
                flags
        );

        if (maximized) {

        }

//        if (m_firstWindowCreation) {
//            if (!ImGui::SFML::Init(getRenderWindow())) {
//                throw std::runtime_error("Failed to initialize ImGui-SFML");
//            }
//
//            b::make_default_themes_available();     // TODO: Move these to a better place
//            b::LoadDefaultFonts();
//
//            m_firstWindowCreation = false;
//        }

        // Load default battery icon
        setIcon(b::Resource::FromBase64(b::Constants::BatteryIconBase64()));
    }

    void Window::close() {
        if (m_sdlWindow) {
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

    }

//    void Window::invokeUpdate() {
//
//        updateWin32DarkMode();
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