#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/graphics/widget_style.hpp"
#include "battery/graphics/font_stack.hpp"

namespace b {

    class window : public sf::RenderWindow {
    public:

        double framerate { 0.0 };
        double frametime { 0.0 };
        uint64_t framecount { 0 };
        widget_style style;
        bool win32_use_immersive_dark_mode = true;

        window(const std::u8string& title, sf::Vector2u size, uint32_t style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
        virtual ~window() = default;

        sf::RenderWindow& getWindow() { return *this; }

        virtual void setup() = 0;
        virtual void update() = 0;
        virtual void cleanup() = 0;

    private:
        void _update();

        sf::Clock deltaClock;
        bool win32_idm_used = !win32_use_immersive_dark_mode;
        friend class windowed_application;
    };

}