
#include "battery/graphics/styles.hpp"
#include "battery/graphics/color_hex.hpp"
#include "resources/default_style_json.hpp"
#include "magic_enum.hpp"

namespace b {

    void load_default_imgui_style() {
        auto style = nlohmann::json::parse(resources::default_style_json.str());
        load_default_imgui_style(style);
    }

    void load_default_imgui_style(const nlohmann::json& style) {
        for (auto& [key, value] : style.items()) {
            // If it is an ImGui StyleVar
            auto stylevar_enum = magic_enum::enum_cast<ImGuiStyleVar_>(key);
            if (stylevar_enum.has_value()) {
//                if (value.is_array()) {
//                    auto a = ImGui::GetStyle().FrameBorderSize;
//                    ImGui::PushStyleVar(stylevar_enum.value(), ImVec2(value[0], value[1]));
//                }
//                else {
//                    ImGui::PushStyleVar(stylevar_enum.value(), value);
//                }
//                continue;
                throw std::invalid_argument(fmt::format("ImGuiStyleVars are not yet implemented in the default style!"));
            }

            // If it is an ImGui Color
            auto color_enum = magic_enum::enum_cast<ImGuiCol_>(key);
            if (color_enum.has_value()) {
                auto colors = ImGui::GetStyle().Colors;
                colors[color_enum.value()] = color_hex(value) / 255;
                continue;
            }

            throw std::invalid_argument(fmt::format("Invalid key passed to push_default_imgui_style: {} is not a valid ImGui StyleVar or Color!", key));
        }
    }

    void push_default_battery_style(widget_style& style) {

        // Button widget
//        style["button-background-color"] = "#222222";
//        style["button-active-color"] = "#FF0000";
//        style["button-hover-color"] = "#229922";
//        style["button-border-radius"] = "20px";
//        style["button-border-width"] = "1px";
//        style["button-border-color"] = "#00FF00";
//        style["button-padding"] = ImVec2(50, 50);
//        style["button-width"] = "0px";
//        style["button-height"] = "0px";

    }

}