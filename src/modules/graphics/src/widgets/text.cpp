
#include "battery/graphics/widgets/text.hpp"

namespace b::widgets {

    void text::operator()() {
        base_push_style();

        text_container.native_window_border = false;

        text_container.left = this->left;
        text_container.right = this->right;
        text_container.top = this->top;
        text_container.bottom = this->bottom;
        text_container.width = this->width;
        text_container.height = this->height;

        auto textSize = ImGui::CalcTextSize(label.c_str());
        // If the width not defined, set it to the width of the text
        if (text_container.width.unit() == b::unit::NONE && (text_container.left.unit() == b::unit::NONE || text_container.right.unit() == b::unit::NONE)) {
            text_container.width = textSize.x;
            this->width = textSize.x;
        }
        // If the height not defined, set it to the height of the text
        if (text_container.height.unit() == b::unit::NONE && (text_container.top.unit() == b::unit::NONE || text_container.bottom.unit() == b::unit::NONE)) {
            text_container.height = textSize.y;
            this->height = textSize.y;
        }

        base_set_cursor_position_to_min_bb();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);
        text_container([this, &textSize]() {

            if (alignh == alignh::CENTER) {
                ImGui::SetCursorPosX((text_container.actual_size.x - textSize.x) / 2);
            } else if (alignh == alignh::RIGHT) {
                ImGui::SetCursorPosX(text_container.actual_size.x - textSize.x);
            }

            if (alignv == alignv::CENTER) {
                ImGui::SetCursorPosY((text_container.actual_size.y - textSize.y) / 2);
            } else if (alignv == alignv::BOTTOM) {
                ImGui::SetCursorPosY(text_container.actual_size.y - textSize.y);
            }

            auto cursorpos = ImGui::GetCursorPos();
            ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_Text), "%s", label.c_str());
            actual_position = ImGui::GetItemRectMin();
            actual_size = ImGui::GetItemRectSize();

            if (underline && !hyperlink_hovered) {
                ImVec2 min = ImGui::GetItemRectMin();
                ImVec2 max = ImGui::GetItemRectMax();
                max.y -= underline_offset;
                min.y = max.y;
                ImGui::GetWindowDrawList()->AddLine(min, max, ImGui::GetColorU32(ImGuiCol_Text), 1.0f);
            }

            if (hyperlink) {
                ImGui::SetCursorPos(cursorpos);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
                hyperlink_clicked = ImGui::InvisibleButton(base_get_identifier().c_str(), textSize);
                hyperlink_hovered = ImGui::IsItemHovered();
                if (hyperlink_hovered) {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::PopStyleVar();
            }
        });
        ImGui::PopStyleVar(3);

        base_pop_style();
    }

}