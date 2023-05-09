
#include "App.hpp"

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

    login_button.style["button-gradient-color-left"] = "#06bfff"_u;
    login_button.style["button-gradient-color-right"] = "#2d73ff"_u;
}

void MainWindow::update() {

    window.size = {(float) getWindow().getSize().x, (float) getWindow().getSize().y};
    window([this]() {
        drag_window();
        button([]() { b::log::info("Button pressed"); });
        login_button();
    });
    if (!window.is_open) {
        getWindow().close();
    }

    ImGui::ShowDemoWindow();
}

void MainWindow::cleanup() {

}
