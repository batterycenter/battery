
#include "battery/graphics/widgets/container.hpp"

namespace b::widgets {

    void container::operator()() {
        this->operator()({});
    }

    void container::operator()(const std::function<void()>& callback) {
        basePushStyle();

        baseSetCursorPositionToMinBB();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::BeginChild(baseGetIdentifier().c_str(), baseGetBBSize(), native_window_border, flags);
        ImGui::PopStyleVar();

        basePopStyle();

        if (callback) {
            children_style.push();
            callback();
            children_style.pop();
        }

        ImGui::EndChild();
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();
    }

}