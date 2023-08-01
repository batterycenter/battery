
#include "battery/graphics/widgets/text.hpp"

namespace b::widgets {

    void text::operator()() {
        basePushStyle();

        text_container.native_window_border = false;

        text_container.left = this->left;
        text_container.right = this->right;
        text_container.top = this->top;
        text_container.bottom = this->bottom;
        text_container.width = this->width;
        text_container.height = this->height;

        auto textSize = ImGui::CalcTextSize(label.c_str());
        // If the width not defined, set it to the width of the text
        if (text_container.width.unit() == b::Unit::NONE && (text_container.left.unit() == b::Unit::NONE || text_container.right.unit() == b::Unit::NONE)) {
            text_container.width = textSize.x;
            this->width = textSize.x;
        }
        // If the height not defined, set it to the height of the text
        if (text_container.height.unit() == b::Unit::NONE && (text_container.top.unit() == b::Unit::NONE || text_container.bottom.unit() == b::Unit::NONE)) {
            text_container.height = textSize.y;
            this->height = textSize.y;
        }

        baseSetCursorPositionToMinBB();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, b::Vec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, b::Vec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);
        text_container([this, &textSize]() {

            if (alignh == AlignH::Center) {
                ImGui::SetCursorPosX((text_container.actualSize.x - textSize.x) / 2);
            } else if (alignh == AlignH::Right) {
                ImGui::SetCursorPosX(text_container.actualSize.x - textSize.x);
            }

            if (alignv == AlignV::Center) {
                ImGui::SetCursorPosY((text_container.actualSize.y - textSize.y) / 2);
            } else if (alignv == AlignV::Bottom) {
                ImGui::SetCursorPosY(text_container.actualSize.y - textSize.y);
            }

            auto cursorpos = ImGui::GetCursorPos();
            ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_Text), "%s", label.c_str());
            actualPosition = ImGui::GetItemRectMin();
            actualSize = ImGui::GetItemRectSize();

            if (underline && !hyperlink_hovered) {
                auto min = ImGui::GetItemRectMin();
                auto max = ImGui::GetItemRectMax();
                max.y -= underline_offset;
                min.y = max.y;
                ImGui::GetWindowDrawList()->AddLine(min, max, ImGui::GetColorU32(ImGuiCol_Text), 1.0);
            }

            if (hyperlink) {
                ImGui::SetCursorPos(cursorpos);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
                hyperlink_clicked = ImGui::InvisibleButton(baseGetIdentifier().c_str(), textSize);
                hyperlink_hovered = ImGui::IsItemHovered();
                if (hyperlink_hovered) {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::PopStyleVar();
            }
        });
        ImGui::PopStyleVar(3);

        basePopStyle();
    }

}