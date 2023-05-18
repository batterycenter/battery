#pragma once

#include "battery/graphics/widgets/button.hpp"
#include "battery/graphics/sfml.hpp"

namespace b::widgets {

    void button::operator()() {
        base_push_style();

        base_set_cursor_position_to_min_bb();
        if (custom_implementation) {
            std::tie(clicked, hovered, held) = custom_implementation();
        }
        else {
            clicked = ImGui::Button(base_get_identifier().c_str(), base_get_bb_size());
        }

        held = ImGui::IsItemActive();
        hovered = ImGui::IsItemHovered();
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();

        base_pop_style();
    }

}