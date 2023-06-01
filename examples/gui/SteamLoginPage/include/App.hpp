#pragma once

#include "battery/core.hpp"
#include "battery/graphics.hpp"

class MainWindow : public b::window {
public:
    std::unique_ptr<b::resource_loader> py_ui_loader;       // TODO: Try to make resource_loader copyable again (segfault bc of a moving lambda function or so)

    void handle_window_dragging();

    struct Context {

        std::function<void()> handle_window_dragging;
        bool titlebar_hovered = false;
        ImVec2 window_size;
        sf::Texture steam_logo;
        sf::Texture qrcode_texture;

        bool steam_hyperlink_clicked = false;
        bool help_hyperlink_clicked = false;
        bool create_account_hyperlink_clicked = false;

        inline static void define_python_types(b::py::module& module) {
            auto submodule = module.def_submodule("MainWindow");
            b::py::class_<Context>(submodule, "Context")
                    .def("handle_window_dragging", [](Context& self) { self.handle_window_dragging(); })
                    .def_readwrite("titlebar_hovered", &Context::titlebar_hovered)
                    .def_readwrite("window_size", &Context::window_size)
                    .def_readwrite("steam_logo", &Context::steam_logo)
                    .def_readwrite("qrcode_texture", &Context::qrcode_texture)
                    .def_readwrite("steam_hyperlink_clicked", &Context::steam_hyperlink_clicked)
                    .def_readwrite("help_hyperlink_clicked", &Context::help_hyperlink_clicked)
                    .def_readwrite("create_account_hyperlink_clicked", &Context::create_account_hyperlink_clicked);
        }
    } context;

    void setup() override;
    void update() override;
    void cleanup() override;

    template<typename... Args> MainWindow(Args... args) : b::window(args...) {}
    ~MainWindow() = default;

    inline static MainWindow& get() { return dynamic_cast<MainWindow&>(*b::application::get().windows()[0].get()); }
};

class App : public b::application {
public:
    struct Context {

        bool py_test_property1 = false;
        int py_test_property2 = 0;

        inline static void define_python_types(b::py::module& module) {
            auto app = module.def_submodule("App");
            b::py::class_<Context>(app, "Context")
                .def_readwrite("py_test_property1", &Context::py_test_property1)
                .def_readwrite("py_test_property2", &Context::py_test_property2);

            app.def("stop_application", [](Context& self) { b::application::get().stop_application(); });
        }
    } context;

    App() = default;
    ~App() = default;

    void setup() override {
        this->add_window(new MainWindow("Battery SteamLoginPage Example", sf::Vector2u(1180, 720), sf::Style::None));
    }

    void update() override {}
    void cleanup() override {}

    inline static App& get() { return dynamic_cast<App&>(b::application::get()); }
};
