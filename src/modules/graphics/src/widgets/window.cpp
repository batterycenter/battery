
#include "battery/graphics/widgets/window.hpp"

namespace b::widgets {

    void window::operator()(const std::function<void()>& callback) {
        widget_builder builder(style);

        if (position.has_value()) {
            ImGui::SetNextWindowPos(*position);
        }

        if (size.has_value()) {
            ImGui::SetNextWindowSize(*size);
        }

        if (border_width.has_value()) { ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, border_width.value()); }
        builder.add_color_rule("window-titlebar-color").push(ImGuiCol_TitleBg);
        builder.add_color_rule("window-titlebar-color-active").push(ImGuiCol_TitleBgActive);
        builder.add_color_rule("window-titlebar-color-collapsed").push(ImGuiCol_TitleBgCollapsed);
        builder.add_color_rule("window-background-color").push(ImGuiCol_WindowBg);

        if (!always_open) {
            ImGui::Begin(get_identifier().c_str(), &is_open, flags);
        } else {
            ImGui::Begin(get_identifier().c_str(), nullptr, flags);
        }
        titlebar_hovered = ImGui::IsItemHovered();
        actual_position = ImGui::GetWindowPos();
        actual_size = ImGui::GetWindowSize();

        if (border_width.has_value()) { ImGui::PopStyleVar(); }

        children_style.push();
        if (callback) {
            callback();
        }
        children_style.pop();

        ImGui::End();
        builder.pop();

    }

}