#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/graphics/widget_style.hpp"
#include "battery/graphics/font_stack.hpp"
#include "battery/graphics/widgets/all.hpp"
#include "battery/graphics/context.hpp"
#include "battery/core/resource_loader.hpp"
#include "battery/eventbus.hpp"

namespace b {

    namespace events {
        struct WindowCloseEvent {};
        struct WindowResizeEvent : public sf::Event::SizeEvent {};
        struct WindowLostFocusEvent {};
        struct WindowGainedFocusEvent {};
        struct TextEnteredEvent : public sf::Event::TextEvent {};
        struct KeyPressEvent : public sf::Event::KeyEvent {};
        struct KeyReleaseEvent : public sf::Event::KeyEvent {};
        struct MouseWheelScrollEvent : public sf::Event::MouseWheelScrollEvent {};
        struct MouseButtonPressEvent : public sf::Event::MouseButtonEvent {};
        struct MouseButtonReleaseEvent : public sf::Event::MouseButtonEvent {};
        struct MouseMoveEvent : public sf::Event::MouseMoveEvent {};
        struct MouseEnteredWindowEvent {};
        struct MouseLeftWindowEvent {};
        struct JoystickButtonPressEvent : public sf::Event::JoystickButtonEvent {};
        struct JoystickButtonReleaseEvent : public sf::Event::JoystickButtonEvent {};
        struct JoystickMoveEvent : public sf::Event::JoystickMoveEvent {};
        struct JoystickConnectEvent : public sf::Event::JoystickConnectEvent {};
        struct JoystickDisconnectEvent : public sf::Event::JoystickConnectEvent {};
        struct TouchBeginEvent : public sf::Event::TouchEvent {};
        struct TouchMoveEvent : public sf::Event::TouchEvent {};
        struct TouchEndEvent : public sf::Event::TouchEvent {};
        struct SensorChangeEvent : public sf::Event::SensorEvent {};
    }

    class basic_window : public sf::RenderWindow {
    public:

        double m_framerate { 0.0 };
        double m_frametime { 0.0 };
        uint64_t m_framecount { 0 };
        bool m_useWin32ImmersiveDarkMode = true;
        widget_style style;

        sf::RenderWindow& m_sfmlWindow = *this;          // This is a reference to the base class

        basic_window() : m_eventbus(std::make_shared<b::event_bus>()), m_eventListener(m_eventbus) {};
        virtual ~basic_window() = default;

        void create(const sf::Vector2u& mode, const b::string& title, std::uint32_t style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
        void create(sf::VideoMode mode, const b::string& title, std::uint32_t style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
        virtual void attach() = 0;
        virtual void update() = 0;
        virtual void detach() = 0;
        virtual void defineContextPythonTypes(b::py::module& module) = 0;

        template<typename T, typename... TArgs>
        bool dispatchEvent(TArgs&&... args) {
            if (std::find(m_registeredEvents.begin(), m_registeredEvents.end(), typeid(T).name()) == m_registeredEvents.end()) {
                return false;
            }
            m_eventbus->postpone<T>({ std::forward<TArgs>(args)... });
            m_eventbus->process();
            return true;
        }

        template<typename T>
        void listenEvent(std::function<void(const T&)> listener) {
            m_eventListener.listenToCallback(listener);
            m_registeredEvents.emplace_back(typeid(T).name());
        }

        void pyInit(py::function python_ui_loop);

        template<typename T>
        void setPythonUiScriptResource(const T& script) {
            m_uiScriptResourceLoader = std::make_unique<b::resource_loader>(script, [this](const auto& resource) {
                m_uiScriptResource = resource;
                m_uiScriptLoaded = false;
            });
        }

        // Prevent all move and assignment operations due to the reference
        basic_window(const basic_window&) = delete;
        basic_window& operator=(const basic_window&) = delete;
        basic_window(basic_window&&) = delete;
        basic_window& operator=(basic_window&&) = delete;

        void invoke_update();

    private:
        void render_error_message(const b::string& error);

        py::function m_pythonUiLoopCallback;
        b::resource m_uiScriptResource;
        std::unique_ptr<b::resource_loader> m_uiScriptResourceLoader;
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
    };

    template<b::derived_from<b::context> T, b::string_literal ContextName>
    class window : public basic_window {
    public:
        T m_context;

        window() = default;

        void defineContextPythonTypes(b::py::module& module) override {
            m_context.define_python_types(module);
            module.attr(ContextName.value) = m_context;
            module.attr("init") = [this](const b::py::function& python_ui_loop) {
                pyInit(python_ui_loop);
            };
        }
    };

}