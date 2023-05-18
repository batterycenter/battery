
#include "battery/graphics/widgets/input.hpp"

namespace b::widgets {

    void input::operator()() {
        base_push_style();

        buffer = content;
        buffer.resize(buffer_size);
        base_set_cursor_position_to_min_bb();

        if (base_get_bb_size().x != 0) {
            ImGui::PushItemWidth(base_get_bb_size().x);
        }

        changed = ImGui::InputTextWithHint(base_get_identifier().c_str(), hint.c_str(), buffer.data(), buffer.size(), flags);
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();
        active = ImGui::IsItemActive();
        content = buffer.data();    // Re-parse as a C-String

        base_pop_style();
    }

}