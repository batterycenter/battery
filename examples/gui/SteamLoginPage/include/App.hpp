#pragma once

#include "battery/core/all.hpp"
#include "battery/graphics/windowed_application.hpp"

#include "battery/graphics/widgets/button.hpp"
#include "battery/graphics/widgets/window.hpp"

class MainWindow : public b::window {
public:
    MainWindow(const std::u8string& title, sf::Vector2u size) : b::window(title, size) {}
    ~MainWindow() = default;

    sf::Font font;
    sf::Texture battery;

    sf::Vector2<float> position = { 100.0f, 100.0f };
    sf::Vector2<float> velocity = { 1.0f, 1.0f };

    b::widgets::window window;
    b::widgets::button button;

    void ui();

    void setup() override;
    void update() override;
    void cleanup() override;
};

class App : public b::windowed_application {
public:
    App() = default;
    ~App() = default;

    void setup() override {
        register_window(std::make_shared<MainWindow>(u8"Battery_SimpleGraphicsExample", sf::Vector2u(800, 600)));
    }

    void update() override {}
    void cleanup() override {}
};
