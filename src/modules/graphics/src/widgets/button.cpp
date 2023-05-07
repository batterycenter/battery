#pragma once

#include "battery/graphics/widgets/button.hpp"
#include "battery/graphics/sfml.hpp"

namespace b::widgets {

    void button::operator()(const std::function<void()>& callback) {

        if (sameline) {
            ImGui::SameLine();
        }

        style.push();
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, b::property_stack::get("button-border-radius", 0.f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, b::property_stack::get("button-border-width", 1.f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, b::property_stack::get("button-padding", ImVec2()));
        ImGui::PushStyleColor(ImGuiCol_Border, b::property_stack::get("button-border-color", ImVec4()) / 255);
        ImGui::PushStyleColor(ImGuiCol_Button, b::property_stack::get("button-background-color", ImVec4()) / 255);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, b::property_stack::get("button-hover-color", ImVec4()) / 255);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, b::property_stack::get("button-active-color", ImVec4()) / 255);

        float width = b::property_stack::get("button-width", 0.f);
        float height = b::property_stack::get("button-height", 0.f);

        clicked = ImGui::Button(get_identifier().c_str(), { width, height });
        held = ImGui::IsItemActive();
        hovered = ImGui::IsItemHovered();

        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        style.pop();

        if (callback && clicked) {
            callback();
        }
    }

}