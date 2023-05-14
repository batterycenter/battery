
#include "battery/graphics/widgets/image.hpp"

namespace b::widgets {

    void image::operator()() {
        widget_builder builder(style);

        builder.add_color_rule("text-color").push(ImGuiCol_Text);
        builder.add_color_rule("text-color-disabled").push(ImGuiCol_TextDisabled);
        builder.add_color_rule("text-color-selected").push(ImGuiCol_TextSelectedBg);
        builder.add_color_rule("border-color").push(ImGuiCol_Border);

        builder.add_numeric_rule("border-radius")
                .add_case(b::unit::UNITLESS, [this](auto value) { return value; })
                .add_case(b::unit::PIXEL, [this](auto value) { return value; })
                .add_case(b::unit::PERCENT, [this](auto value) { return value / 100.f * actual_size.y; })
//                .add_case(b::unit::EM, [this](auto value) { return b::get_current_font_size() * value; })
                .push(ImGuiStyleVar_FrameRounding);

        builder.add_numeric_rule("border-width")
                .add_case(b::unit::UNITLESS, [this](auto value) { return value; })
                .add_case(b::unit::PIXEL, [this](auto value) { return value; })
                .add_case(b::unit::PERCENT, [this](auto value) { return value / 100.f * actual_size.y; })
//                .add_case(b::unit::EM, [this](auto value) { return b::get_current_font_size() * value; })
                .push(ImGuiStyleVar_FrameBorderSize);

        builder.add_vec2_rule("padding")
                .add_case_x(b::unit::UNITLESS, [this](auto value) { return value; })
                .add_case_x(b::unit::PIXEL, [this](auto value) { return value; })
                .add_case_x(b::unit::PERCENT, [this](auto value) { return value / 100.f * actual_size.x; })
//                .add_case_x(b::unit::EM, [this](auto value) { return b::get_current_font_size() * value; })
                .add_case_y(b::unit::UNITLESS, [this](auto value) { return value; })
                .add_case_y(b::unit::PIXEL, [this](auto value) { return value; })
                .add_case_y(b::unit::PERCENT, [this](auto value) { return value / 100.f * actual_size.y; })
//                .add_case_y(b::unit::EM, [this](auto value) { return b::get_current_font_size() * value; })
                .push(ImGuiStyleVar_FramePadding);

        if (sameline) {
            ImGui::SameLine();
        }

        ImVec2 _size = size;
        if (_size.x == 0) _size.x = (float)src.getSize().x;
        if (_size.y == 0) _size.y = (float)src.getSize().y;
        ImGui::Image(src, _size);
        actual_size = ImGui::GetItemRectSize();
    }

}