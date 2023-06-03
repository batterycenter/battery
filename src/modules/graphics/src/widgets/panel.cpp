
#include "battery/graphics/widgets/panel.hpp"

namespace b::widgets {

    void panel::operator()(const std::function<void()>& callback) {
        base_push_style();

        ImGui::SetNextWindowPos(base_get_bb_min());
        ImGui::SetNextWindowSize(base_get_bb_size());

        if (!always_open) {
            ImGui::Begin(base_get_identifier().c_str(), &is_open, flags);
        } else {
            ImGui::Begin(base_get_identifier().c_str(), nullptr, flags);
        }
        titlebar_hovered = ImGui::IsItemHovered();
        actual_position = ImGui::GetWindowPos();
        actual_size = ImGui::GetWindowSize();

        base_pop_style();

        if (callback) {
            children_style.push();
            callback();
            children_style.pop();
        }

        ImGui::End();
    }

}