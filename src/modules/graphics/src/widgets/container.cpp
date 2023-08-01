
#include "battery/graphics/widgets/container.hpp"

namespace b::widgets {

    void container::operator()() {
        this->operator()({});
    }

    void container::operator()(const std::function<void()>& callback) {
        basePushStyle();

        baseSetCursorPositionToMinBB();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, b::Vec2(0, 0));
        ImGui::BeginChild(baseGetIdentifier().c_str(), baseGetBBSize(), native_window_border, flags);
        ImGui::PopStyleVar();

        basePopStyle();

        if (callback) {
            children_style.push();
            callback();
            children_style.pop();
        }

        ImGui::EndChild();
        actualPosition = ImGui::GetItemRectMin();
        actualSize = ImGui::GetItemRectSize();
    }

}