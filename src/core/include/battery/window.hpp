#pragma once

#include <utility>

#include "string.hpp"
#include "vec.hpp"
#include "folders.hpp"
#include "resource_loader.hpp"
#include "eventbus.hpp"
#include "glaze/glaze.hpp"
#include "imgui.h"

union SDL_Event;
struct SDL_Window;

namespace b {

    class Application;

    struct WindowState {
        b::Vec2 size;
        b::Vec2 position;
        bool maximized = false;
    };

}   // namespace b

template <>
struct glz::meta<b::WindowState> {
    using T = b::WindowState;
    static constexpr auto value = object(
            "size", &T::size,
            "position", &T::position,
            "maximized", &T::maximized
    );
};

#ifdef B_OS_WINDOWS
struct HWND__;
typedef HWND__ *HWND;
#endif

namespace b {

#ifdef B_OS_WINDOWS
    using WindowHandle = HWND;
#else
    using WindowHandle = void*;
#endif

    struct WindowOptions {
#ifndef B_OS_WEB
        std::optional<b::fs::path> lastWindowStateJsonFilePath = b::Folders::AppConfigDir() / "windowstate.json";
        std::optional<ImColor> win32CustomTitleBarColor = Constants::DefaultWindowTitlebarColor();
        std::optional<ImColor> win32CustomWindowFrameColor = Constants::DefaultWindowFrameColor();
        bool startAsVisible = true;
        bool resizable = true;
#endif // !B_OS_WEB
    };

    static b::Vec2 GetPrimaryMonitorSize();

    class Window {
    public:

//        double framerate { 0.0 };
//        double frametime { 0.0 };
//        uint64_t framecount { 0 };
//        MousePositionData mouse;
//        bool useWin32ImmersiveDarkMode = true;
//        widget_style style;

        Window(const std::string& title, const b::Vec2& size, const WindowOptions& options = WindowOptions());
        virtual ~Window() noexcept;

        b::Application& app() const;

        virtual void onAttach() {};
        virtual void onUpdate() {};
        virtual void onRender() {};
        virtual void onDetach() {};

        void showInTaskbar();
        void hideFromTaskbar();
        void maximize();
        void minimize();
        void restore();
        bool isMaximized();
        bool isMinimized();

        bool setIcon(const b::Resource& icon);

        void setPosition(const b::Vec2& position);
        b::Vec2 getPosition() const;

        void setSize(const b::Vec2& size);
        b::Vec2 getSize() const;

        // True if the window is completely outside of any monitor
        bool isWindowOffscreen() const;

        void centerOnPrimaryMonitor();

        void focus();
        bool hasFocus() const;

        void setTitle(const std::string& title);

        WindowHandle getSystemHandle() const;

        // Prevent all move and assignment operations due to the SDL handle
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        template<typename T, typename... TArgs>
        auto attachEventHandler(TArgs&&... args) {
            return m_eventbus.attachEventHandler<T>(std::forward<TArgs>(args)...);
        }

        template<typename T, typename... TArgs>
        auto dispatchEvent(TArgs&&... args) {
            return m_eventbus.dispatchEvent<T>(std::forward<TArgs>(args)...);
        }

        SDL_Window* getSDLWindow() const {
            return m_sdlWindow;
        }

        uint32_t getSDLWindowID() const {
            return m_sdlWindowID;
        }

        bool processImGuiSDLEvent(SDL_Event* event);
        bool processSDLWindowEvent(SDL_Event* event);

    protected:

        SDL_Window* m_sdlWindow {};
        uint32_t m_sdlWindowID = 0;
        inline static bool m_windowExists {};

    private:

#ifndef B_OS_WEB
        std::optional<WindowState> loadCachedWindowState() const;
        bool writeCachedWindowState(WindowState state) const;
#endif // !B_OS_WEB

        void renderContent();
        void updateWin32DarkMode();
        void processWindowEvents();
        void renderErrorMessage(const std::string& error);

//        b::Events::MouseMoveEvent m_mouseMoveEventData;
//        std::shared_ptr<b::event_bus> m_eventbus;

        b::EventBus m_eventbus;

        WindowOptions m_options {};
    };

}  // namespace b
