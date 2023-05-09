#pragma once

#include "battery/graphics/widgets/button.hpp"
#include "battery/graphics/widget_builder.hpp"
#include "battery/graphics/sfml.hpp"
#include "battery/graphics/font_stack.hpp"

namespace b::widgets {

    void button::operator()(const std::function<void()>& callback) {
        widget_builder builder(style);

        builder.add_numeric_rule("button-border-radius")
                .add_case(b::unit::UNITLESS, [this](auto value) { return value; })
                .add_case(b::unit::PIXEL, [this](auto value) { return value; })
                .add_case(b::unit::PERCENT, [this](auto value) { return value / 100.f * actual_size.y; })
                .add_case(b::unit::EM, [this](auto value) { return b::get_current_font_size() * value; })
                .push(ImGuiStyleVar_FrameRounding);

        builder.add_numeric_rule("button-border-width")
                .add_case(b::unit::UNITLESS, [this](auto value) { return value; })
                .add_case(b::unit::PIXEL, [this](auto value) { return value; })
                .add_case(b::unit::PERCENT, [this](auto value) { return value / 100.f * actual_size.y; })
                .add_case(b::unit::EM, [this](auto value) { return b::get_current_font_size() * value; })
                .push(ImGuiStyleVar_FrameBorderSize);

        builder.add_vec2_rule("button-padding")
                .add_case_x(b::unit::UNITLESS, [this](auto value) { return value; })
                .add_case_x(b::unit::PIXEL, [this](auto value) { return value; })
                .add_case_x(b::unit::PERCENT, [this](auto value) { return value / 100.f * actual_size.x; })
                .add_case_x(b::unit::EM, [this](auto value) { return b::get_current_font_size() * value; })
                .add_case_y(b::unit::UNITLESS, [this](auto value) { return value; })
                .add_case_y(b::unit::PIXEL, [this](auto value) { return value; })
                .add_case_y(b::unit::PERCENT, [this](auto value) { return value / 100.f * actual_size.y; })
                .add_case_y(b::unit::EM, [this](auto value) { return b::get_current_font_size() * value; })
                .push(ImGuiStyleVar_FramePadding);

        builder.add_color_rule("button-border-color").push(ImGuiCol_Border);
        builder.add_color_rule("button-background-color").push(ImGuiCol_Button);
        builder.add_color_rule("button-hover-color").push(ImGuiCol_ButtonHovered);
        builder.add_color_rule("button-active-color").push(ImGuiCol_ButtonActive);

        if (sameline) {
            ImGui::SameLine();
        }

        clicked = ImGui::Button(get_identifier().c_str(), { size.x, size.y });
        held = ImGui::IsItemActive();
        hovered = ImGui::IsItemHovered();
        actual_size = ImGui::GetItemRectSize();
        builder.pop();

        if (callback && clicked) {
            callback();
        }
    }

}