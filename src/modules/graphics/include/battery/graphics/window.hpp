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

        window(const std::u8string& title, sf::Vector2u size);
        virtual ~window() = default;

        sf::RenderWindow& getWindow() { return *this; }

        virtual void setup() = 0;
        virtual void update() = 0;
        virtual void cleanup() = 0;

    private:
        void _update();

        sf::Clock deltaClock;
        friend class windowed_application;
    };

}