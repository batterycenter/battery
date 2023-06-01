#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/graphics/widget_style.hpp"
#include "battery/graphics/font_stack.hpp"
#include "battery/graphics/widgets/all.hpp"
#include "battery/eventbus.hpp"

namespace b {

    namespace events {

        struct WindowCloseEvent {};

        struct WindowResizeEvent {
            sf::Vector2u size;
        };
    }

    class window : public sf::RenderWindow {
    public:

        double m_framerate { 0.0 };
        double m_frametime { 0.0 };
        uint64_t m_framecount { 0 };
        bool m_useWin32ImmersiveDarkMode = true;
        widget_style style;

        sf::RenderWindow& m_sfmlWindow = *this;          // This is a reference to the base class

        window() : m_eventbus(std::make_shared<b::event_bus>()), m_eventListener(m_eventbus) {};
        virtual ~window() = default;

        void create(const sf::Vector2u& mode, const b::string& title, std::uint32_t style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
        void create(sf::VideoMode mode, const b::string& title, std::uint32_t style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
        virtual void attach() = 0;
        virtual void update() = 0;
        virtual void detach() = 0;

        template<typename T, typename... TArgs>
        bool dispatchEvent(TArgs&&... args) {
            m_eventbus->postpone<T>({ std::forward<TArgs>(args)... });
            m_eventbus->process();
            return std::find(m_registeredEvents.begin(), m_registeredEvents.end(), typeid(T).name()) != m_registeredEvents.end();
        }

        template<typename T>
        void listenEvent(std::function<void(const T&)> listener) {
            m_eventListener.listenToCallback(listener);
            m_registeredEvents.push_back(typeid(T).name());
        }

        void init(py::function python_ui_loop);
        void load_py_script(const b::resource& script);

        // Prevent all move and assignment operations due to the reference
        window(const window&) = delete;
        window& operator=(const window&) = delete;
        window(window&&) = delete;
        window& operator=(window&&) = delete;

    private:
        void _update();
        void render_error_message(const b::string& error);

        py::function m_pythonUiLoopCallback;
        b::resource m_uiScriptResource;
        bool m_uiScriptLoaded = true;
        std::optional<b::string> m_errorMessage;
        b::widgets::window m_errorWindowWidget;
        b::widgets::text m_errorTextWidget;
        bool m_firstWindowCreation = true;

        std::vector<b::string> m_registeredEvents;
        std::shared_ptr<b::event_bus> m_eventbus;
        b::event_listener m_eventListener;

        sf::Clock m_deltaClock;
        bool m_win32IDMActive = !m_useWin32ImmersiveDarkMode;
        friend class application;
    };

}