#pragma once

#include "battery/core/all.hpp"
#include "battery/graphics/windowed_application.hpp"

#include "battery/graphics/widgets/fancy_button.hpp"
#include "battery/graphics/widgets/window.hpp"
#include "battery/graphics/widgets/dummy.hpp"
#include "battery/graphics/widgets/text.hpp"
#include "battery/graphics/widgets/input.hpp"
#include "battery/graphics/widgets/image.hpp"

class MainWindow : public b::window {
public:
    template<typename... Args>
    MainWindow(Args... args) : b::window(args...) {}
    ~MainWindow() = default;

    inline static MainWindow* get() {
        return dynamic_cast<MainWindow*>(b::windowed_application::get()->get_window(0).get());
    }

    inline static b::py::function ui_loop;
    std::unique_ptr<b::resource_loader> main_py_loader;
    bool main_py_loaded = false;
    b::resource main_py;

    b::widgets::window window;
    b::widgets::button button;
    b::widgets::fancy_button login_button;
    b::widgets::image img;

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
