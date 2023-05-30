
#include "App.hpp"
#include "battery/qrcode/qrcode.hpp"
#include "resources/ui_main_py.hpp"

// Resources can be embedded like this, or with battery_embed()
const b::string steam_logo_png = "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"
                                   "PSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4KICA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIgogICAgeG1sbnM6eG1wTU09Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9tbS8iCiAgICB4bWxuczpzdEV2dD0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL3NUeXBlL1Jlc291cmNlRXZlbnQjIgogICAgeG1sbnM6ZGM9Imh0dHA6Ly9wdXJsLm9yZy9kYy9lbGVtZW50cy8xLjEvIgogICAgeG1sbnM6R0lNUD0iaHR0cDovL3d3dy5naW1wLm9yZy94bXAvIgogICAgeG1sbnM6dGlmZj0iaHR0cDovL25zLmFkb2JlLmNvbS90aWZmLzEuMC8iCiAgICB4bWxuczp4bXA9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC8iCiAgIHhtcE1NOkRvY3VtZW50SUQ9ImdpbXA6ZG9jaWQ6Z2ltcDo1N2ZlMGQ3Ny0xY2E1LTQ3MWUtYWNiMy1kZjZhMjJmZmM3YmMiCiAgIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6MDNjNjE4ZDItZDEzNC00ZTM4LTk2OTctMDI5NmU4NmE0MDZiIgogICB4bXBNTTpPcmlnaW5hbERvY3VtZW50SUQ9InhtcC5kaWQ6YWY0Zjc1YmUtNGI5Yi00OTkxLWJjMzYtZGI5MzUxMWZkOGI3IgogICBkYzpGb3JtYXQ9ImltYWdlL3BuZyIKICAgR0lNUDpBUEk9IjIuMCIKICAgR0lNUDpQbGF0Zm9ybT0iV2luZG93cyIKICAgR0lNUDpUaW1lU3RhbXA9IjE2ODQwNTIwNzY0MjMyODEiCiAgIEdJTVA6VmVyc2lvbj0iMi4xMC4zMiIKICAgdGlmZjpPcmllbnRhdGlvbj0iMSIKICAgeG1wOkNyZWF0b3JUb29sPSJHSU1QIDIuMTAiCiAgIHhtcDpNZXRhZGF0YURhdGU9IjIwMjM6MDU6MTRUMTA6MTQ6MzQrMDI6MDAiCiAgIHhtcDpNb2RpZnlEYXRlPSIyMDIzOjA1OjE0VDEwOjE0OjM0KzAyOjAwIj4KICAgPHhtcE1NOkhpc3Rvcnk+CiAgICA8cmRmOlNlcT4KICAgICA8cmRmOmxpCiAgICAgIHN0RXZ0OmFjdGlvbj0ic2F2ZWQiCiAgICAgIHN0RXZ0OmNoYW5nZWQ9Ii8iCiAgICAgIHN0RXZ0Omluc3RhbmNlSUQ9InhtcC5paWQ6YmZlYmExM2ItZTRkMi00MDE0LThmNzktNThkMDZhOWVmOTg0IgogICAgICBzdEV2dDpzb2Z0d2FyZUFnZW50PSJHaW1wIDIuMTAgKFdpbmRvd3MpIgogICAgICBzdEV2dDp3aGVuPSIyMDIzLTA1LTE0VDEwOjE0OjM2Ii8+CiAgICA8L3JkZjpTZXE+CiAgIDwveG1wTU06SGlzdG9yeT4KICA8L3JkZjpEZXNjcmlwdGlvbj4KIDwvcmRmOlJERj4KPC94OnhtcG1ldGE+CiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAKICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIAogICAgICAgICAgICAgICAgICAgICAgICAgICAKPD94cGFja2V0IGVuZD0idyI/PiZpnawAAAAGYktHRAD/AAAAADMnfPMAAAAJcEhZcwAALiMAAC4jAXilP3YAAAAHdElNRQfnBQ4IDiRDOeE+AAAAGXRFWHRDb21tZW50AENyZWF0ZWQgd2l0aCBHSU1QV4EOFwAACENJREFUeNrtW11sFNcVvnfWf0vB5s9uSjE2YJPQH1VIbQk/Tas8BApthNQqUlupitqoKqrIQ5W3JkqqlgdeCo3ov4papakqtaqiNrQliBBCLSBA5JIi3BhIsMEYr3d3ZndnZ+bee77Th4yRWby7s2t7cAJH8sus58493z33nO+cc68Q9+TuFhnnxwbOn092dnevlFJ2WJbVnkgkkkIIQUQeAykwj10bGnpnzdq13gcCAMdxViSTya2WZT0kpfyMlLJHSmlVeoeZwcwXmfk0gNd93/9Xa2vr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";

const b::string URL = "https://youtu.be/dQw4w9WgXcQ";

PYBIND11_EMBEDDED_MODULE(b, module) {
    b::define_python_types(module);
    b::define_widget_types(module);

    App::Context::define_python_types(module);
    MainWindow::Context::define_python_types(module);

    module.attr("app_context") = &App::get().context;
    module.attr("main_window_context") = &MainWindow::get().context;

    module.def("init_main_window", [](b::py::function callback) { MainWindow::get().init(callback); });
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
        m_window.setPosition(m_window.getPosition() + sf::Vector2i((int)delta.x, (int)delta.y));
    }

    wasMouseDragging = mouseDragging;
}

void MainWindow::setup() {
    context.handle_window_dragging = [this]() { handle_window_dragging(); };

    auto icon = b::resource::from_base64(steam_logo_png);
    (void)context.steam_logo.loadFromMemory(icon.data(), icon.size());

    py_ui_loader = std::make_unique<b::resource_loader>(resources::ui_main_py, [this](auto resource) {
        load_py_script(resource);
    });

    auto qrcode = b::qrcode::QrCode::encodeText(URL.c_str(), b::qrcode::QrCode::Ecc::HIGH);
    context.qrcode_texture = b::qrcode::to_texture(qrcode);
}

void MainWindow::update() {
    context.window_size = { (float)m_window.getSize().x, (float)m_window.getSize().y };

    if (context.steam_hyperlink_clicked || context.help_hyperlink_clicked || context.create_account_hyperlink_clicked) {
        b::log::info("Opening URL: {}", URL);
        b::open_url_in_default_browser(URL);
    }
}

void MainWindow::cleanup() {

}
