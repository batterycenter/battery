#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/graphics/widget_style.hpp"
#include "battery/graphics/font_stack.hpp"
#include "battery/graphics/widgets/all.hpp"

namespace b {

    class window : public sf::RenderWindow {
    public:

        double m_framerate { 0.0 };
        double m_frametime { 0.0 };
        uint64_t m_framecount { 0 };
        bool m_useWin32ImmersiveDarkMode = true;
        widget_style style;

        sf::RenderWindow& m_sfmlWindow = *this;          // This is a reference to the base class

        window();
        window(std::optional<b::string> title, std::optional<sf::Vector2u> size, uint32_t style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
        virtual ~window() = default;

        virtual void setup() = 0;
        virtual void update() = 0;
        virtual void cleanup() = 0;

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

        sf::Clock m_deltaClock;
        bool m_win32IDMActive = !m_useWin32ImmersiveDarkMode;
        friend class application;
    };

}