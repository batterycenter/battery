
#include "battery/graphics/widgets/text.hpp"

namespace b::widgets {

    void text::operator()() {
        widget_builder builder(style);

        builder.add_color_rule("text-color").push(ImGuiCol_Text);
        builder.add_color_rule("text-color-disabled").push(ImGuiCol_TextDisabled);
        builder.add_color_rule("text-color-selected").push(ImGuiCol_TextSelectedBg);

        auto color = b::property_stack::get_color("text-color");
        set_cursor_position();
        if (color.has_value()) {
            ImGui::TextColored(color.value() / 255.f, "%s", label.c_str());
        }
        else {
            ImGui::Text("%s", label.c_str());
        }
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();

        builder.pop();
    }

}