#pragma once

#include "battery/core/all.hpp"
#include "battery/graphics/windowed_application.hpp"

#include "battery/graphics/widgets/fancy_button.hpp"
#include "battery/graphics/widgets/window.hpp"

class MainWindow : public b::window {
public:
    template<typename... Args>
    MainWindow(Args... args) : b::window(args...) {}
    ~MainWindow() = default;

    b::widgets::window window;
    b::widgets::button button;
    b::widgets::fancy_button login_button;

    void drag_window();
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
        register_window(std::make_shared<MainWindow>(
                u8"Battery SteamLoginPage Example",
                sf::Vector2u(1280, 720),
                sf::Style::None));
    }

    void update() override {}
    void cleanup() override {}
};
