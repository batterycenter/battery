
#include "battery/graphics/widgets/rect.hpp"

namespace b::widgets {

    void rect::operator()() {
        basePushStyle();

        auto size = baseGetBBSize();
        if (size.x != 0 && size.y != 0) {
            baseSetCursorPositionToMinBB();
            b::Vec2 p0 = ImGui::GetCursorScreenPos();
            ImGui::InvisibleButton(baseGetIdentifier().c_str(), size);
            ImGui::GetWindowDrawList()->AddRectFilled(
                    p0,
                    p0 + size,
                    ImGui::GetColorU32(fill_color.color().Value / 255.f),
                    ImGui::GetStyle().FrameRounding,
                    flags);
            actualPosition = ImGui::GetItemRectMin();
            actualSize = ImGui::GetItemRectSize();
        }

        basePopStyle();
    }

}