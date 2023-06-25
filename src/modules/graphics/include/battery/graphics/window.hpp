#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/graphics/widget_style.hpp"
#include "battery/graphics/font_stack.hpp"
#include "battery/graphics/widgets/all.hpp"
#include "battery/graphics/context.hpp"
#include "battery/graphics/constants.hpp"
#include "battery/graphics/primitive_render_window.hpp"
#include "battery/graphics/batch_renderer.hpp"
#include "battery/core/resource_loader.hpp"
#include "battery/eventbus.hpp"

namespace b {

    namespace Events {
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

    class Window : public sf::RenderWindow {
    public:
        double framerate { 0.0 };
        double frametime { 0.0 };
        uint64_t framecount { 0 };
        bool useWin32ImmersiveDarkMode = true;
        widget_style style;

        BatchRenderer batchRenderer;
        sf::RenderWindow& sfmlWindow = *this;

        Window() : m_eventbus(std::make_shared<b::event_bus>()), m_eventListener(m_eventbus) {};
        ~Window() noexcept;

        void create(const b::vec2& mode, const b::string& title, std::uint32_t style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings(0, 0, 8));
        void create(sf::VideoMode mode, const b::string& title, std::uint32_t style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings(0, 0, 8), bool silenceJsonWarning = false);
        void create(const b::string& title, std::uint32_t style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings(0, 0, 8));
        virtual void onAttach() = 0;
        virtual void onUpdate() = 0;
        virtual void onDetach() = 0;
        virtual void definePythonBindings(b::py::module& module) {}

        template<typename T, typename... TArgs>
        bool dispatchEvent(TArgs&&... args) {
            if (!m_eventListener.isListening<T>()) {
                return false;
            }
            m_eventbus->postpone<T>({ std::forward<TArgs>(args)... });
            m_eventbus->process();
            return true;
        }

        template<typename T>
        void attachEventHandler(std::function<void(const T&)> listener) {
            m_eventListener.listenToCallback(listener);
        }

        void pyInit(py::function python_ui_loop);
        void rememberWindowPositionJsonFile(const b::fs::path& filename);
        void setDefaultWindowSize(const b::vec2& size);

        void showInTaskbar();
        void hideFromTaskbar();
        void maximize();
        void minimize();
        void restore();
        bool isMaximized();
        bool isMinimized();

        b::vec2 getMousePos();
        b::vec2 getMousePosPrev();
        b::vec2 getMouseDelta();

        template<typename T>
        void setPythonUiScriptResource(const T& script) {
            m_uiScriptResourceLoader = std::make_unique<b::ResourceLoader>(script, [this](const auto& resource) {
                m_uiScriptResource = resource;
                m_uiScriptLoaded = false;
            });
        }

        // Prevent all move and assignment operations due to the reference
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        void invoke_update();

    private:
        void renderErrorMessage(const b::string& error);

        py::function m_pythonUiLoopCallback;
        b::Resource m_uiScriptResource;
        std::unique_ptr<b::ResourceLoader> m_uiScriptResourceLoader;
        bool m_uiScriptLoaded = true;

        std::optional<b::string> m_errorMessage;
        b::widgets::panel m_errorPanelWidget;
        b::widgets::text m_errorTextWidget;
        bool m_firstWindowCreation = true;

        std::shared_ptr<b::event_bus> m_eventbus;
        b::event_listener m_eventListener;

        sf::Clock m_deltaClock;
        bool m_win32IDMActive = !useWin32ImmersiveDarkMode;

        b::fs::path m_windowPositionJsonFile;
        b::vec2 m_defaultWindowSize = Constants::DefaultWindowSize();

        b::vec2 m_mousePos;
        b::vec2 m_mousePosPrev;
        b::vec2 m_mouseDelta;

        struct WindowState {
            b::vec2 size;
            b::vec2 position;
            bool maximized = false;
        } m_lastWindowState;
    };

    template<b::derived_from<b::PyContext> T, b::template_string_literal ContextName>
    class PyWindow : public Window {
    public:
        T context;

        PyWindow() {
            context.m_initPyWindowFunc = [this](const b::py::function& python_ui_loop) {
                pyInit(python_ui_loop);
            };
        }

        void definePythonBindings(b::py::module& module) override {
            T::defineParentPythonClass(module);
            context.definePythonClass(module);
            module.attr(ContextName.value) = &context;
        }
    };

}