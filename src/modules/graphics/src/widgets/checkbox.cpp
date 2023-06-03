
#include "battery/graphics/widgets/checkbox.hpp"

namespace b::widgets {

    void checkbox::operator()() {
        basePushStyle();

        baseSetCursorPositionToMinBB();
        ImGui::Checkbox(baseGetIdentifier().c_str(), &state);
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();

        basePopStyle();
    }

}