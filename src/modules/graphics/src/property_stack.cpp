
#include "battery/graphics/property_stack.hpp"
#include "battery/graphics/sfml.hpp"
#include "magic_enum.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

namespace b {

    void property_stack::push(const std::string& property_name, const property_pack& property_value) {
        properties.emplace_back(property_name, property_value);

        auto stylevar = magic_enum::enum_cast<ImGuiStyleVar_>(property_name);
        if (stylevar.has_value()) {     // If the property is named after a ImGuiStyleVar_, push it as such
            pushpop_stylevar(stylevar.value(), property_name, property_value, true);
            return;
        }

        auto stylecol = magic_enum::enum_cast<ImGuiCol_>(property_name);
        if (stylecol.has_value()) {     // Otherwise, If the property is named after a ImGuiCol_, push it as such
            pushpop_color(stylecol.value(), property_name, property_value, true);
            return;
        }

        // None of the above apply (Not an ImGuiStyleVar_ or ImGuiCol_)
        if (property_name == "ImGuiItemWidth") {
            if (property_value.get_properties().size() != 1)
                throw std::invalid_argument(fmt::format("Cannot push ImGuiItemWidth: Nested properties are not supported!"));

            if (property_value.get_properties()[0].unit() == b::unit::COLOR_HEX)
                throw std::invalid_argument(fmt::format("Cannot push ImGuiItemWidth: Property value is not of type float!"));

            ImGui::PushItemWidth((float)property_value.get_properties()[0]);
        }

        // Otherwise, nothing is pushed to ImGui
    }

    void property_stack::push(const std::string& property_name, const unit_property& property_value) {
        push(property_name, property_pack(property_value));
    }

    void property_stack::pop() {
        if (properties.empty()) throw std::logic_error("Cannot pop property from property_stack: Stack is empty! Probably a style push/pop mismatch!");
        auto [property_name, property_value] = properties.back();
        properties.pop_back();

        // Pushes that resulted in an exception did not actually push anything
        // Thus, we do the exact same as for pushing, but without checking exceptions

        auto stylevar = magic_enum::enum_cast<ImGuiStyleVar_>(property_name);
        if (stylevar.has_value()) {     // If the property is named after a ImGuiStyleVar_, pop it as such
            pushpop_stylevar(stylevar.value(), property_name, property_value, false);
            return;
        }

        auto stylecol = magic_enum::enum_cast<ImGuiCol_>(property_name);
        if (stylecol.has_value()) {     // Otherwise, If the property is named after a ImGuiCol_, pop it as such
            pushpop_color(stylecol.value(), property_name, property_value, false);
            return;
        }

        // None of the above apply (Not an ImGuiStyleVar_ or ImGuiCol_)
        if (property_name == "ImGuiItemWidth") {
            if (property_value.get_properties().size() != 1)
                throw std::invalid_argument(fmt::format("Cannot pop ImGuiItemWidth: Nested properties are not supported!"));

            if (property_value.get_properties()[0].unit() == b::unit::COLOR_HEX)
                throw std::invalid_argument(fmt::format("Cannot pop ImGuiItemWidth: Property value is not of type float!"));

            ImGui::PopItemWidth();
        }

        // Otherwise, nothing is popped from ImGui
    }

    void property_stack::clear() {
        while (!properties.empty()) {
            pop();
        }
    }

    std::optional<property_pack> property_stack::get(const std::string& property_name) {
        for (auto iter = properties.rbegin(); iter != properties.rend(); ++iter) {
            auto& [key, value] = *iter;
            if (key == property_name) {
                return value;
            }
        }
        return std::nullopt;
    }

    void property_stack::pushpop_stylevar(ImGuiStyleVar_ stylevar_enum, const std::string& property_name, const property_pack& property_value, bool push) {
        if (property_value.get_properties().size() == 1) {  // Single property
            if (property_value.get_properties()[0].unit() == b::unit::COLOR_HEX)
                throw std::invalid_argument(fmt::format("Cannot push/pop style var {}: Property value is not of type float!", property_name));

            if (push) ImGui::PushStyleVar(stylevar_enum, (float)property_value.get_properties()[0]);
            else ImGui::PopStyleVar();
        }
        else if (property_value.get_properties().size() == 2) {  // ImVec2
            if (property_value.get_properties()[0].unit() == b::unit::COLOR_HEX)
                throw std::invalid_argument(fmt::format("Cannot push/pop style var {}: Either nested property value is not of type float!", property_name));

            if (push) ImGui::PushStyleVar(stylevar_enum, ImVec2((float)property_value.get_properties()[0], (float)property_value.get_properties()[1]));
            else ImGui::PopStyleVar();
        }
        else {
            throw std::invalid_argument(fmt::format("Cannot push/pop style var {}: Property value is neither of type float nor ImVec2!", property_name));
        }
    }

    void property_stack::pushpop_color(ImGuiCol_ color_enum, const std::string& property_name, const property_pack& property_value, bool push) {
        if (property_value.get_properties().size() == 1) {  // Single property
            if (property_value.get_properties()[0].unit() != b::unit::COLOR_HEX)
                throw std::invalid_argument(fmt::format("Cannot push/pop style color {}: Property value is not a color!", property_name));

            if (push) ImGui::PushStyleColor(color_enum, (ImVec4)property_value.get_properties()[0] / 255.f);
            else ImGui::PopStyleColor();
        }
        else {
            throw std::invalid_argument(fmt::format("Cannot push/pop style color {}: Colors do not support nested properties!", property_name));
        }
    }

}