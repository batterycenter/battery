
#include "battery/application.hpp"
#include "battery/window.hpp"
#include "battery/fs.hpp"
#include "battery/exception.hpp"

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

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

    Window::Window(const std::string& title, const b::Vec2& size, const WindowOptions& options)
            : m_options(options) {

        // Check if a window already exists
        if (m_windowExists) {
            throw std::runtime_error("b::Window: The ImGui SDL backend used by battery does not "
                                     "support multiple windows yet and a window already exists.");
        }
        m_windowExists = true;

        // Calculate window size and position
        b::Vec2 newSize = size;
        b::Vec2 newPosition = (getPrimaryMonitorSize() - size) / 2.0;
        bool maximized = false;

        // Restore the previous window position and size
        if (auto state = loadCachedWindowState(); state) {
            newPosition = state->position;
            newSize = state->size;
            maximized = state->maximized;
        }

        // Setup SDL window flags
        uint32_t flags = 0;
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
        updateWin32DarkMode();

        if (isWindowOffscreen()) {
            b::log::warn("b::Window was completely outside of any connected monitor. Centering on primary monitor.");
            centerOnPrimaryMonitor();
        }

        if (maximized) {
            maximize();
        }

        // Load default battery icon
        setIcon(b::Resource::FromBase64(b::Constants::BatteryIconBase64()));
        focus();
    }

    Window::~Window() noexcept {
        if (m_sdlWindow) {
            bool maximized = isMaximized();
            if (maximized) {
                restore();
            }
            writeCachedWindowState({ getSize(), getPosition(), maximized });
            SDL_DestroyWindow(m_sdlWindow);
            m_sdlWindow = nullptr;
        }
    }

    b::Application& Window::app() const {
        return b::Application::get();
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
        SDL_SetWindowPosition(m_sdlWindow, static_cast<int>(position.x), static_cast<int>(position.y));
    }

    b::Vec2 Window::getPosition() const {
        int x = 0;
        int y = 0;
        SDL_GetWindowPosition(m_sdlWindow, &x, &y);
        return b::Vec2(x, y);
    }

    void Window::setSize(const b::Vec2& size) {
        SDL_SetWindowSize(m_sdlWindow, static_cast<int>(size.x), static_cast<int>(size.y));
    }

    b::Vec2 Window::getSize() const {
        int x = 0;
        int y = 0;
        SDL_GetWindowSize(m_sdlWindow, &x, &y);
        return b::Vec2(x, y);
    }

#ifdef B_OS_WINDOWS
    static BOOL getWin32MonitorsImpl(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
        auto *monitors = reinterpret_cast<std::vector<HMONITOR>*>(dwData);
        monitors->push_back(hMonitor);
        return TRUE;
    }

    static std::vector<HMONITOR> getAllWin32Monitors() {
        std::vector<HMONITOR> monitors;
        if (EnumDisplayMonitors(nullptr, nullptr, getWin32MonitorsImpl, reinterpret_cast<LPARAM>(&monitors)) == 0) {
            return {};
        }
        return monitors;
    }

    static bool isPointInRect(const b::Vec2& point, const RECT& rect) {
        return point.x >= rect.left && point.x <= rect.right && point.y >= rect.top && point.y <= rect.bottom;
    }
#endif

    bool Window::isWindowOffscreen() const {
#ifdef B_OS_WINDOWS
        MONITORINFO monitorInfo;
        monitorInfo.cbSize = sizeof(MONITORINFO);
        for (auto& monitor : getAllWin32Monitors()) {
            if (GetMonitorInfoW(monitor, &monitorInfo) == 0) {
                b::log::error("GetMonitorInfoW failed, pretending that window is outside of monitor space");
                return true;
            }

            // We say the window is on the monitor if at least one corner is in the area
            auto pos = getPosition();
            auto size = getSize();
            if (isPointInRect({ pos.x, pos.y }, monitorInfo.rcWork) ||
                isPointInRect({ pos.x + size.x, pos.y }, monitorInfo.rcWork) ||
                isPointInRect({ pos.x + size.x, pos.y + size.y }, monitorInfo.rcWork) ||
                isPointInRect({ pos.x, pos.y + size.y }, monitorInfo.rcWork)) {
                return false;
            }
        }
#endif
        return true;
    }

    void Window::centerOnPrimaryMonitor() {
        auto monitorSize = getPrimaryMonitorSize();
        auto size = b::Vec2(std::min(getSize().x, monitorSize.x), std::min(getSize().y, monitorSize.y));
        setSize(size);
        setPosition((monitorSize - size) / 2.0);
    }

    void Window::focus() {
        SDL_RaiseWindow(m_sdlWindow);
        SDL_SetWindowInputFocus(m_sdlWindow);
    }

    bool Window::hasFocus() const {
        auto flags = SDL_GetWindowFlags(m_sdlWindow);
        return flags & SDL_WINDOW_INPUT_FOCUS || flags & SDL_WINDOW_MOUSE_FOCUS;
    }

    void Window::setTitle(const std::string& title) {
        SDL_SetWindowTitle(m_sdlWindow, title.c_str());
    }

    WindowHandle Window::getSystemHandle() const {
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(m_sdlWindow, &wmInfo);

#ifdef B_OS_WINDOWS
        return wmInfo.info.win.window;
#else
#warning not implemented
        return nullptr;
#endif
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

    bool Window::processImGuiSDLEvent(SDL_Event* event) {
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

    std::optional<WindowState> Window::loadCachedWindowState() const {
        if (!m_options.lastWindowStateJsonFilePath) {
            return {};
        }
        if (m_options.lastWindowStateJsonFilePath->empty()) {
            return {};
        }

        if (!b::fs::exists(*m_options.lastWindowStateJsonFilePath)) {
            return {};
        }

        auto state = glz::read_json<WindowState>(
                b::fs::read(*m_options.lastWindowStateJsonFilePath));

        if (state) {
            return *state;
        }
        else {
            return {};
        }
    }

    bool Window::writeCachedWindowState(WindowState state) const {
        if (!m_options.lastWindowStateJsonFilePath) {
            return false;
        }
        if (m_options.lastWindowStateJsonFilePath->empty()) {
            return false;
        }
        auto result = b::fs::try_write(*m_options.lastWindowStateJsonFilePath,
                                       glz::write_json(state));
        if (result) {
            return true;
        }
        else {
            return false;
        }
    }

#ifdef B_OS_WINDOWS
    static COLORREF ImColToColorref(const ImColor& color) {
        COLORREF colorref =
                static_cast<uint32_t>(color.Value.z * 255) << 16 |
                static_cast<uint32_t>(color.Value.y * 255) << 8 |
                static_cast<uint32_t>(color.Value.x * 255);
        return colorref;
    }
#endif

    void Window::updateWin32DarkMode() {
        // bool const useWin32ImmersiveDarkMode = b::style::get<bool>("b::window.win32ImmersiveDarkMode"); // TODO: <- Interesting Copilot suggestion
#ifdef B_OS_WINDOWS
        // The titlebar color itself
        if (m_options.win32CustomTitleBarColor) {
            COLORREF titlebarColor = ImColToColorref(*m_options.win32CustomTitleBarColor);
            DwmSetWindowAttribute(
                    getSystemHandle(), DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,
                    &titlebarColor, sizeof(titlebarColor));
        }

        // The titlebar frame color (1-pixel boundary)
        if (m_options.win32CustomWindowFrameColor) {
            COLORREF frameColor = ImColToColorref(*m_options.win32CustomWindowFrameColor);
            DwmSetWindowAttribute(
                    getSystemHandle(), DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR,
                    &frameColor, sizeof(frameColor));
        }
#endif
    }

} // namespace b
