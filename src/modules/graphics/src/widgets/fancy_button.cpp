
#include "battery/graphics/widgets/fancy_button.hpp"
#include "imgui_internal.h"

namespace b::widgets {

    std::tuple<bool, bool, bool> draw(const char* label, const ImVec2& size_arg, ImVec4 bg1f, ImVec4 bg2f) {
        auto* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return { false, false, false };

        bg1f = bg1f / 255.f;
        bg2f = bg2f / 255.f;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

        const ImRect bb(pos, pos + size);
        ImGui::ItemSize(size, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id))
            return { false, false, false };

        ImGuiButtonFlags flags = ImGuiButtonFlags_None;
        if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
            flags |= ImGuiButtonFlags_Repeat;

        bool hovered, held;
        bool clicked = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

        // Render
        const bool is_gradient = bg1f != bg2f;
        if (held || hovered)
        {
            // Modify colors (ultimately this can be prebaked in the style)
            float h_increase = (held && hovered) ? 0.02f : 0.02f;
            float v_increase = (held && hovered) ? 0.20f : 0.07f;

            ImGui::ColorConvertRGBtoHSV(bg1f.x, bg1f.y, bg1f.z, bg1f.x, bg1f.y, bg1f.z);
            bg1f.x = ImMin(bg1f.x + h_increase, 1.0f);
            bg1f.z = ImMin(bg1f.z + v_increase, 1.0f);
            ImGui::ColorConvertHSVtoRGB(bg1f.x, bg1f.y, bg1f.z, bg1f.x, bg1f.y, bg1f.z);
            if (is_gradient)
            {
                ImGui::ColorConvertRGBtoHSV(bg2f.x, bg2f.y, bg2f.z, bg2f.x, bg2f.y, bg2f.z);
                bg2f.z = ImMin(bg2f.z + h_increase, 1.0f);
                bg2f.z = ImMin(bg2f.z + v_increase, 1.0f);
                ImGui::ColorConvertHSVtoRGB(bg2f.x, bg2f.y, bg2f.z, bg2f.x, bg2f.y, bg2f.z);
            }
            else
            {
                bg2f = bg1f;
            }
        }
        ImGui::RenderNavHighlight(bb, id);

        int vert_start_idx = window->DrawList->VtxBuffer.Size;
        window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::ColorConvertFloat4ToU32(bg1f), g.Style.FrameRounding);
        int vert_end_idx = window->DrawList->VtxBuffer.Size;
        if (is_gradient)
            ImGui::ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vert_start_idx, vert_end_idx, bb.Min, bb.GetTR(), ImGui::ColorConvertFloat4ToU32(bg1f), ImGui::ColorConvertFloat4ToU32(bg2f));
        if (g.Style.FrameBorderSize > 0.0f)
            window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_Border), g.Style.FrameRounding, 0, g.Style.FrameBorderSize);

        if (g.LogEnabled)
            ImGui::LogSetNextTextDecoration("[", "]");
        ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
        return { clicked, held, hovered };
    }

    fancy_button::fancy_button() : button("Fancy Button") {
        this->custom_implementation = [this]() {
            auto color_left = b::property_stack::get("button-gradient-color-left", "#000000"_u.color());
            auto color_right = b::property_stack::get("button-gradient-color-right", "#FFFFFF"_u.color());
            return draw(get_identifier().c_str(), this->size, color_left, color_right);
        };
    }

    void fancy_button::operator()(const std::function<void()> &callback) {
        b::widgets::button::operator()(callback);
    }

}