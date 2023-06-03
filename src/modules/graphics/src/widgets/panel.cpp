
#include "battery/graphics/widgets/panel.hpp"

namespace b::widgets {

    void panel::operator()(const std::function<void()>& callback) {
        basePushStyle();

        ImGui::SetNextWindowPos(baseGetBBMin());
        ImGui::SetNextWindowSize(baseGetBBSize());

        if (!always_open) {
            ImGui::Begin(baseGetIdentifier().c_str(), &is_open, flags);
        } else {
            ImGui::Begin(baseGetIdentifier().c_str(), nullptr, flags);
        }
        titlebar_hovered = ImGui::IsItemHovered();
        actual_position = ImGui::GetWindowPos();
        actual_size = ImGui::GetWindowSize();

        basePopStyle();

        if (callback) {
            children_style.push();
            callback();
            children_style.pop();
        }

        ImGui::End();
    }

}