
#include "battery/graphics/widgets/rect.hpp"

namespace b::widgets {

    void rect::operator()() {
        base_push_style();

        auto size = base_get_bb_size();
        if (size.x != 0 && size.y != 0) {
            base_set_cursor_position_to_min_bb();
            ImVec2 p0 = ImGui::GetCursorScreenPos();
            ImGui::InvisibleButton(base_get_identifier().c_str(), size);
            ImGui::GetWindowDrawList()->AddRectFilled(
                    p0,
                    p0 + size,
                    ImGui::GetColorU32(fill_color.color().Value / 255.f),
                    ImGui::GetStyle().FrameRounding,
                    flags);
            actual_position = ImGui::GetItemRectMin();
            actual_size = ImGui::GetItemRectSize();
        }

        base_pop_style();
    }

}