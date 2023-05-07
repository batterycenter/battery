
#include "battery/graphics/property_stack.hpp"
#include "battery/graphics/sfml.hpp"
#include "magic_enum.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

namespace b {

    void property_stack::push(const std::string& property_name, const unit_property& property_value) {
        properties.emplace_back(property_name, property_value);

        auto stylevar = magic_enum::enum_cast<ImGuiStyleVar_>(property_name);
        if (stylevar.has_value()) {     // If the property is named after a ImGuiStyleVar_, push it as such
            push_stylevar(stylevar.value(), property_name, property_value);
            return;
        }

        auto stylecol = magic_enum::enum_cast<ImGuiCol_>(property_name);
        if (stylecol.has_value()) {     // Otherwise, If the property is named after a ImGuiCol_, push it as such
            push_color(stylecol.value(), property_name, property_value);
            return;
        }

        // None of the above apply (Not an ImGuiStyleVar_ or ImGuiCol_)
        if (property_name == "ImGuiItemWidth") {
            ImGui::PushItemWidth((float)property_value);
        }

        // Otherwise, nothing is pushed
    }

    void property_stack::pop() {
        if (properties.empty()) throw std::logic_error("Cannot pop property from property_stack: Stack is empty! Probably a style push/pop mismatch!");

        auto& [property, value] = properties.back();

        auto stylevar = magic_enum::enum_cast<ImGuiStyleVar_>(property);
        if (stylevar.has_value()) {
            switch (value.unit()) {
                case b::unit_property::unit::UNITLESS:
                case b::unit_property::unit::PIXEL:
                case b::unit_property::unit::PERCENT:
                case b::unit_property::unit::VEC2:
                    ImGui::PopStyleVar();
                    break;

                default:
                    throw std::logic_error(fmt::format("Cannot pop style color {}: Property value is not float or ImVec2!", property));
            }
        }

        auto stylecol = magic_enum::enum_cast<ImGuiCol_>(property);
        if (stylecol.has_value()) {
            switch (value.unit()) {
                case b::unit_property::unit::COLOR_HEX:
                    ImGui::PopStyleColor();
                    break;

                default:
                    throw std::logic_error(fmt::format("Cannot pop style color {}: Property value is not a color!", property));
            }
        }

        // None of the above apply (Not an ImGuiStyleVar_)
        if (property == "ImGuiItemWidth") {
            ImGui::PushItemWidth((float)value);
        }

        properties.pop_back();
    }

    std::optional<unit_property> property_stack::get(const std::string& property_name) {
        for (auto iter = properties.rbegin(); iter != properties.rend(); ++iter) {
            auto& [key, value] = *iter;
            if (key == property_name) {
                return value;
            }
        }
        return std::nullopt;
    }

    void property_stack::push_stylevar(ImGuiStyleVar_ stylevar_enum, const std::string& property_name, const unit_property& property_value) {
        switch (property_value.unit()) {
            case b::unit_property::unit::UNITLESS:
            case b::unit_property::unit::PIXEL:
            case b::unit_property::unit::PERCENT:
                ImGui::PushStyleVar(stylevar_enum, (float)property_value);
                break;

            case b::unit_property::unit::VEC2:
                ImGui::PushStyleVar(stylevar_enum, (ImVec2)property_value);
                break;

            default:
                throw std::logic_error(fmt::format("Cannot push style color {}: Property value is not float or ImVec2!", property_name));
        }
    }

    static ImVec4 adjust_range(ImVec4 color) {
        return ImVec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f);
    }

    void property_stack::push_color(ImGuiCol_ color_enum, const std::string& property_name, const unit_property& property_value) {
        switch (property_value.unit()) {
            case b::unit_property::unit::COLOR_HEX:
                ImGui::PushStyleColor(color_enum, adjust_range((ImVec4)property_value));
                break;

            default:
                throw std::logic_error(fmt::format("Cannot push style color {}: Property value is not a Color!", property_name));
        }
    }

}