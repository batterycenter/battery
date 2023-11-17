#pragma once

#include <utility>

#include "string.hpp"
#include "vec.hpp"
#include "resource_loader.hpp"
#include "glaze/glaze.hpp"

struct SDL_Window;
union SDL_Event;

#ifdef B_OS_WINDOWS
struct HWND__;
typedef HWND__ *HWND;
#endif

namespace b {

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

namespace b {

#ifdef B_OS_WINDOWS
    using WindowHandle = HWND;
#else
#error not implemented
#endif

    struct WindowOptions {
        bool rememberWindowPosition = false;
        bool useWin32ImmersiveDarkMode = true;
        bool startAsVisible = true;
    };

    static b::Vec2 getPrimaryMonitorSize();

    class Window {
    public:

//        double framerate { 0.0 };
//        double frametime { 0.0 };
//        uint64_t framecount { 0 };
//        MousePositionData mouse;
//        bool useWin32ImmersiveDarkMode = true;
//        widget_style style;

        Window();
        ~Window() noexcept;

        b::Application& app() const;

        void create(const std::string& title, const b::Vec2& size, std::optional<WindowOptions> options = {});

        void close();

        virtual void onAttach() {};
        virtual void onUpdate() {};
        virtual void onRender() {};
        virtual void onDetach() {};

//        template<typename T, typename... TArgs>
//        bool dispatchEvent(TArgs&&... args) {
//            if (!m_eventListener.isListening<T>()) {
//                return false;
//            }
//            m_eventbus->postpone<T>({ std::forward<TArgs>(args)... });
//            m_eventbus->process();
//            return true;
//        }
//
//        template<typename T>
//        void attachEventHandler(std::function<void(const T&)> listener) {
//            m_eventListener.listenToCallback(listener);
//        }

        void setWindowPositionJsonFilePath(const b::fs::path& filename);

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

        void focus();
        bool hasFocus() const;

        bool isOpen() const;

        void setTitle(const std::string& title);

        HWND getSystemHandle();

        // Prevent all move and assignment operations due to the SDL handle
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        void update();

        template<typename T, typename... TArgs>
        auto dispatchEvent(TArgs&&... args) {
            return m_eventbus.dispatchEvent<T>(std::forward<TArgs>(args)...);
        }

        inline static std::unordered_map<uint32_t, std::reference_wrapper<Window>> m_windowIDs;

    protected:
        virtual void processEvent(SDL_Event* event) {}

    private:

        bool loadCachedWindowState();
        bool writeCachedWindowState();

        void renderContent();
        void updateWin32DarkMode();
        void processWindowEvents();
        void renderErrorMessage(const std::string& error);

        SDL_Window* m_sdlWindow;
        b::fs::path m_windowPositionJsonFilePath;

//        b::Events::MouseMoveEvent m_mouseMoveEventData;
//        std::shared_ptr<b::event_bus> m_eventbus;

        b::EventBus m_eventbus;

        WindowOptions m_options {};
        WindowState m_lastWindowState {};

        bool m_win32IDMActive {};
    };

}