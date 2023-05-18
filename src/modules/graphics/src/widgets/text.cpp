
#include "battery/graphics/widgets/text.hpp"

namespace b::widgets {

    void text::operator()() {
        base_push_style();

        base_set_cursor_position_to_min_bb();
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_Text), "%s", label.c_str());
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();

        base_pop_style();
    }

}