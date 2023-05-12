
#include "App.hpp"
#include "resources/ui_main_py.hpp"




PYBIND11_EMBEDDED_MODULE(widgets, m) {
    b::py::class_<b::widget_style>(m, "widget_style")
            .def(b::py::init<>())
            .def("__getitem__", &b::widget_style::get)
            .def("__setitem__", &b::widget_style::set)
            .def("push", &b::widget_style::push)
            .def("pop", &b::widget_style::pop);

    b::py::class_<b::widgets::fancy_button>(m, "fancy_button")
            .def(b::py::init<>())
            .def_readwrite("style", &b::widgets::button::style)
            .def_readwrite("clicked", &b::widgets::button::clicked)
            .def_readwrite("actual_size", &b::widgets::button::actual_size)
            .def_readwrite("held", &b::widgets::button::held)
            .def_readwrite("hovered", &b::widgets::button::hovered)
            .def_readwrite("sameline", &b::widgets::button::sameline)
            .def_readwrite("size", &b::widgets::button::size)
            .def_readwrite("name", &b::widgets::button::name)
            .def("__call__", &b::widgets::fancy_button::operator());

    m.def("vspace", &b::widgets::vspace);
    m.def("hspace", &b::widgets::hspace);
    m.def("dummy", &b::widgets::dummy);
    m.def("render_image", [&]() {
        MainWindow::get()->img();
    });

    b::widgets::window::define_python_class(m);
    b::widgets::button::define_python_class(m);
    b::widgets::text::define_python_class(m);
    b::widgets::input::define_python_class(m);
    b::widgets::image::define_python_class(m);

    m.def("init_ui", [&](b::py::function func) {
        MainWindow::ui_loop = func;
    });
}

// Move the parent SFML window if the user is dragging the title bar
void MainWindow::drag_window() {
    static bool dragWindow = false;
    static bool wasMouseDragging = false;
    bool mouseDragging = ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0f);

    if (mouseDragging && !wasMouseDragging && window.titlebar_hovered) {   // Start dragging
        dragWindow = true;
    }
    if (!mouseDragging) {  // Stop dragging
        dragWindow = false;
    }

    if (dragWindow) {
        ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
        getWindow().setPosition(getWindow().getPosition() + sf::Vector2i((int)delta.x, (int)delta.y));
    }

    wasMouseDragging = mouseDragging;
}

void MainWindow::setup() {
    window.border_width = 0.f;
    window.position = {0, 0};
    window.name = "";
    window.flags =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoBringToFrontOnFocus;

    window.style["window-titlebar-color"] = "#202227"_u;
    window.style["window-titlebar-color-active"] = "#202227"_u;
    window.style["window-titlebar-color-collapsed"] = "#202227"_u;
    window.style["window-background-color"] = "#191a1e"_u;
    window.style["ImGuiStyleVar_FramePadding"] = { "10px"_u, "10px"_u };

    auto icon = b::resource::from_base64(b::constants::battery_icon_base64());
    (void)img.img.loadFromMemory(icon.data(), icon.size());
    img.size = { 32, 32 };

    main_py_loader = std::make_unique<b::resource_loader>(resources::ui_main_py, [this](auto resource) {
        main_py = std::move(resource);
        main_py_loaded = true;
        b::log::info("Loaded main.py");
    });

    b::load_font("roboto-medium-20", "roboto-medium", 20.f);
}

void MainWindow::update() {

    if (main_py_loaded) {
        try {
            b::py::exec(main_py.as_string());
        }
        catch (const std::exception& e) {
            b::log::error("{}", e.what());
        }
        main_py_loaded = false;
    }

    window([this]() {
        b::push_font("roboto-medium-20");
        drag_window();
        try {
            if (ui_loop) {
                ui_loop();
            }
        }
        catch (const std::exception &e) {
            b::log::error("{}", e.what());
        }
        b::pop_font();
    });

    if (!window.is_open) {
        getWindow().close();
    }

    //ImGui::ShowDemoWindow();
}

void MainWindow::cleanup() {

}
