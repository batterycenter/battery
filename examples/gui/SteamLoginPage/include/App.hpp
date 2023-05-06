#pragma once

#include "battery/core/all.hpp"
#include "battery/graphics/graphics_application.hpp"

inline const auto APPLICATION_NAME = u8"Battery_SimpleGraphicsExample";
inline const sf::Vector2u DEFAULT_WINDOW_SIZE = { 800, 600 };

class App : public b::graphics_application {
public:
    App() : b::graphics_application(APPLICATION_NAME, DEFAULT_WINDOW_SIZE) {}
    ~App() = default;

    sf::Font font;
    sf::Texture battery;

    sf::Vector2<float> position = { 100.0f, 100.0f };
    sf::Vector2<float> velocity = { 1.0f, 1.0f };

    void setup() override;
    void update() override;
    void cleanup() override;
};
