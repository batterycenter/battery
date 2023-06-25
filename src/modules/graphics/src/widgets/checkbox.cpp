
#include "battery/graphics/widgets/checkbox.hpp"

namespace b::widgets {

    void checkbox::operator()() {
        basePushStyle();

        baseSetCursorPositionToMinBB();
        ImGui::Checkbox(baseGetIdentifier().c_str(), &state);
        actualPosition = ImGui::GetItemRectMin();
        actualSize = ImGui::GetItemRectSize();

        basePopStyle();
    }

}