
#include "battery/graphics/widgets/rect.hpp"

namespace b::widgets {

    void rect::operator()() {
        basePushStyle();

        auto size = baseGetBBSize();
        if (size.x != 0 && size.y != 0) {
            baseSetCursorPositionToMinBB();
            auto p0 = ImGui::GetCursorScreenPos();
            ImGui::InvisibleButton(baseGetIdentifier().c_str(), size);
            ImGui::GetWindowDrawList()->AddRectFilled(
                    p0,
                    p0 + size,
                    ImGui::GetColorU32(fill_color.color().Value / 255.f),
                    ImGui::GetStyle().FrameRounding,
                    flags);
            actual_position = ImGui::GetItemRectMin();
            actual_size = ImGui::GetItemRectSize();
        }

        basePopStyle();
    }

}