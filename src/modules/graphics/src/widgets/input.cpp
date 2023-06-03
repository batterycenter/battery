
#include "battery/graphics/widgets/input.hpp"

namespace b::widgets {

    void input::operator()() {
        basePushStyle();

        buffer = content;
        buffer.resize(buffer_size);
        baseSetCursorPositionToMinBB();

        if (baseGetBBSize().x != 0) {
            ImGui::PushItemWidth(baseGetBBSize().x);
        }

        changed = ImGui::InputTextWithHint(baseGetIdentifier().c_str(), hint.c_str(), buffer.data(), buffer.size(), flags);
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();
        active = ImGui::IsItemActive();
        content = buffer.data();    // Re-parse as a C-String

        basePopStyle();
    }

}