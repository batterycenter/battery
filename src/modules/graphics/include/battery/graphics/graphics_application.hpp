#pragma once

#include "battery/core/application.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

namespace b {

    class graphics_application : public b::application {
    public:
        sf::RenderWindow window;

        graphics_application(const std::u8string& title, sf::Vector2u size);
        virtual ~graphics_application() = default;

        void app_setup() final override;
        void app_update() final override;
        void app_cleanup() final override;

        virtual void setup() = 0;
        virtual void update() = 0;
        virtual void cleanup() = 0;

    private:
        sf::Clock deltaClock;
        std::u8string title;
        sf::Vector2u size;
    };

}