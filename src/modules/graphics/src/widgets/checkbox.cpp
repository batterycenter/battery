
#include "battery/graphics/widgets/checkbox.hpp"

namespace b::widgets {

    void checkbox::operator()() {
        base_push_style();

        base_set_cursor_position_to_min_bb();
        ImGui::Checkbox(base_get_identifier().c_str(), &state);
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();

        base_pop_style();
    }

}