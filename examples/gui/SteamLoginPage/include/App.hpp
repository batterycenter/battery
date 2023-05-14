#pragma once

#include "battery/core/all.hpp"
#include "battery/graphics/windowed_application.hpp"
#include "battery/graphics/widgets/all.hpp"

class MainWindow : public b::window {
public:
    struct Context {

        std::function<void()> handle_window_dragging;
        bool titlebar_hovered = false;
        ImVec2 window_size;

        inline static void define_python_types(b::py::module& module) {
            auto submodule = module.def_submodule("MainWindow");
            b::py::class_<Context>(submodule, "Context")
                    .def("handle_window_dragging", [](Context& self) { self.handle_window_dragging(); })
                    .def_readwrite("titlebar_hovered", &Context::titlebar_hovered)
                    .def_readwrite("window_size", &Context::window_size);
        }
    } context;

    template<typename... Args>
    MainWindow(Args... args) : b::window(args...) {}
    ~MainWindow() = default;

    inline static MainWindow* get() {
        return dynamic_cast<MainWindow*>(b::windowed_application::get()->get_window(0).get());
    }

    inline static b::py::function ui_loop;
    std::unique_ptr<b::resource_loader> main_py_loader;
    bool main_py_loaded = false;
    std::optional<std::string> init_error;
    std::optional<std::string> loop_error;
    b::resource main_py;

    void handle_window_dragging();
    void ui();

    void setup() override;
    void update() override;
    void cleanup() override;
};

class App : public b::windowed_application {
public:
    struct Context {

        std::function<void()> stop_application;

        inline static void define_python_types(b::py::module& module) {
            auto submodule = module.def_submodule("App");
            b::py::class_<Context>(submodule, "Context")
                    .def("stop_application", [](Context& self) { self.stop_application(); });
        }
    } context;

    App() = default;
    ~App() = default;

    inline static App* get() {
        return dynamic_cast<App*>(b::windowed_application::get());
    }

    void setup() override {
        register_window(std::make_shared<MainWindow>(
                u8"Battery SteamLoginPage Example",
                sf::Vector2u(1280, 720),
                sf::Style::None));

        context.stop_application = [this]() { stop_application(); };
    }

    void update() override {}
    void cleanup() override {}
};
