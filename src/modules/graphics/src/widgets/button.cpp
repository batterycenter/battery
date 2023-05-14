#pragma once

#include "battery/graphics/widgets/button.hpp"
#include "battery/graphics/sfml.hpp"

namespace b::widgets {

    void button::operator()() {
        widget_builder builder(style);

//        builder.add_numeric_rule("border-radius")
//                .add_case(b::unit::UNITLESS, [this](auto value) { return value; })
//                .add_case(b::unit::PIXEL, [this](auto value) { return value; })
//                .add_case(b::unit::PERCENT, [this](auto value) { return value / 100.f * actual_size.y; })
////                .add_case(b::unit::EM, [this](auto value) { return b::get_current_font_size() * value; })
//                .push(ImGuiStyleVar_FrameRounding);
//
//        builder.add_numeric_rule("border-width")
//                .add_case(b::unit::UNITLESS, [this](auto value) { return value; })
//                .add_case(b::unit::PIXEL, [this](auto value) { return value; })
//                .add_case(b::unit::PERCENT, [this](auto value) { return value / 100.f * actual_size.y; })
////                .add_case(b::unit::EM, [this](auto value) { return b::get_current_font_size() * value; })
//                .push(ImGuiStyleVar_FrameBorderSize);
//
//        builder.add_vec2_rule("padding")
//                .add_case_x(b::unit::UNITLESS, [this](auto value) { return value; })
//                .add_case_x(b::unit::PIXEL, [this](auto value) { return value; })
//                .add_case_x(b::unit::PERCENT, [this](auto value) { return value / 100.f * actual_size.x; })
////                .add_case_x(b::unit::EM, [this](auto value) { return b::get_current_font_size() * value; })
//                .add_case_y(b::unit::UNITLESS, [this](auto value) { return value; })
//                .add_case_y(b::unit::PIXEL, [this](auto value) { return value; })
//                .add_case_y(b::unit::PERCENT, [this](auto value) { return value / 100.f * actual_size.y; })
////                .add_case_y(b::unit::EM, [this](auto value) { return b::get_current_font_size() * value; })
//                .push(ImGuiStyleVar_FramePadding);
//
//        builder.add_color_rule("border-color").push(ImGuiCol_Border);
//        builder.add_color_rule("text-color").push(ImGuiCol_Text);
//        builder.add_color_rule("button-color").push(ImGuiCol_Button);
//        builder.add_color_rule("button-color-hover").push(ImGuiCol_ButtonHovered);
//        builder.add_color_rule("button-color-active").push(ImGuiCol_ButtonActive);

        if (custom_implementation) {
            std::tie(clicked, hovered, held) = custom_implementation();
        }
        else {
            set_cursor_position();
            clicked = ImGui::Button(get_identifier().c_str(), desired_size());
        }

        held = ImGui::IsItemActive();
        hovered = ImGui::IsItemHovered();
        actual_size = ImGui::GetItemRectSize();
    }

}