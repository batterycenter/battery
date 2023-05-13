
#include "App.hpp"
#include "resources/ui_main_py.hpp"

PYBIND11_EMBEDDED_MODULE(b, module) {
    b::define_imgui_python_types(module);
    b::define_widget_types(module);

    App::Context::define_python_types(module);
    MainWindow::Context::define_python_types(module);

    module.attr("app_context") = App::get()->context;
    module.attr("main_window_context") = MainWindow::get()->context;

    module.def("init", [&](b::py::function func) {
        MainWindow::ui_loop = func;
    });
}

// Move the parent SFML window if the user is dragging the title bar
void MainWindow::handle_window_dragging() {
    static bool dragWindow = false;
    static bool wasMouseDragging = false;
    bool mouseDragging = ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0f);

    if (mouseDragging && !wasMouseDragging && context.titlebar_hovered) {   // Start dragging
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
    context.handle_window_dragging = [this]() {
        handle_window_dragging();
    };
//    window.border_width = 0.f;
//    window.position = {0, 0};
//    window.name = "";
//    window.flags =
//            ImGuiWindowFlags_NoResize |
//            ImGuiWindowFlags_NoMove |
//            ImGuiWindowFlags_NoCollapse |
//            ImGuiWindowFlags_NoBringToFrontOnFocus;
//
//    window.style["window-titlebar-color"] = "#202227"_u;
//    window.style["window-titlebar-color-active"] = "#202227"_u;
//    window.style["window-titlebar-color-collapsed"] = "#202227"_u;
//    window.style["window-background-color"] = "#191a1e"_u;
//    window.style["ImGuiStyleVar_FramePadding"] = { "10px"_u, "10px"_u };
//
//    auto icon = b::resource::from_base64(b::constants::battery_icon_base64());
//    (void)img.img.loadFromMemory(icon.data(), icon.size());
//    img.size = { 32, 32 };

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

    try {
        if (ui_loop) {
            ui_loop();
        }
    }
    catch (const std::exception& e) {
        b::log::error("{}", e.what());
    }

    //ImGui::ShowDemoWindow();
}

void MainWindow::cleanup() {

}
