#pragma once

#include "battery/graphics/sfml.hpp"

namespace b {

    enum class imgui_style_property_type {
        NONE = 0,
        STYLEVAR_FLOAT,
        STYLEVAR_VEC2,
        COLOR,
        ITEM_WIDTH
    };

    template<imgui_style_property_type type>
    class imgui_style_property {
    public:

        imgui_style_property() = default;

        template<typename = std::enable_if_t<type == imgui_style_property_type::STYLEVAR_FLOAT ||
                                             type == imgui_style_property_type::STYLEVAR_VEC2>>
        imgui_style_property(ImGuiStyleVar_ stylevar_enum) : stylevar_enum(stylevar_enum) {}

        template<typename = std::enable_if_t<type == imgui_style_property_type::STYLEVAR_FLOAT>>
        imgui_style_property(ImGuiStyleVar_ stylevar_enum, float value) : stylevar_enum(stylevar_enum), v_float(value) {}

        template<typename = std::enable_if_t<type == imgui_style_property_type::STYLEVAR_VEC2>>
        imgui_style_property(ImGuiStyleVar_ stylevar_enum, ImVec2 value) : stylevar_enum(stylevar_enum), v_vec2(value) {}

        template<typename = std::enable_if_t<type == imgui_style_property_type::COLOR>>
        imgui_style_property(ImGuiCol_ color_enum) : color_enum(color_enum) {}

        template<typename = std::enable_if_t<type == imgui_style_property_type::COLOR>>
        imgui_style_property(ImGuiCol_ color_enum, ImVec4 color) : color_enum(color_enum), v_color(color) {}

        template<typename = std::enable_if_t<type == imgui_style_property_type::ITEM_WIDTH>>
        imgui_style_property(float value) : v_float(value) {}

        template<typename = std::enable_if_t<type == imgui_style_property_type::STYLEVAR_FLOAT ||
                                             type == imgui_style_property_type::ITEM_WIDTH>>
        void operator=(const std::optional<float>& value) {
            v_float = value;
        }

        template<typename = std::enable_if_t<type == imgui_style_property_type::STYLEVAR_VEC2>>
        void operator=(const std::optional<ImVec2>& value) {
            v_vec2 = value;
        }

        template<typename = std::enable_if_t<type == imgui_style_property_type::COLOR>>
        void operator=(const std::optional<ImVec4>& value) {
            v_color = value;
        }

        // Return type of getter depends on enum type
        typename std::conditional<type == imgui_style_property_type::STYLEVAR_VEC2, std::optional<ImVec2>,
                 std::conditional<type == imgui_style_property_type::COLOR, std::optional<ImVec4>, std::optional<float>>>::type
        get() {
            switch(type) {
                case imgui_style_property_type::STYLEVAR_FLOAT:
                case imgui_style_property_type::ITEM_WIDTH:
                    return v_float;
                case imgui_style_property_type::STYLEVAR_VEC2:
                    return v_vec2;
                case imgui_style_property_type::COLOR:
                    return v_color;
                default:
                    return std::nullopt;
            }
        }

        void push() {
            switch(type) {
                case imgui_style_property_type::STYLEVAR_FLOAT:
                    if (stylevar_enum < ImGuiStyleVar_COUNT && v_float.has_value()) {
                        ImGui::PushStyleVar(stylevar_enum, v_float.value());
                    }
                    break;

                case imgui_style_property_type::STYLEVAR_VEC2:
                    if (stylevar_enum < ImGuiStyleVar_COUNT && v_vec2.has_value()) {
                        ImGui::PushStyleVar(stylevar_enum, v_vec2.value());
                    }
                    break;

                case imgui_style_property_type::COLOR:
                    if (color_enum < ImGuiCol_COUNT && v_color.has_value()) {
                        ImGui::PushStyleColor(color_enum, v_color.value());
                    }
                    break;

                case imgui_style_property_type::ITEM_WIDTH:
                    if (v_float.has_value()) ImGui::PushItemWidth(v_float.value());
                    break;

                default:
                    break;
            }
        }

        void pop() {
            switch(type) {
                case imgui_style_property_type::STYLEVAR_FLOAT:
                    if (stylevar_enum < ImGuiStyleVar_COUNT && v_float.has_value()) {
                        ImGui::PopStyleVar();
                    }
                    break;

                case imgui_style_property_type::STYLEVAR_VEC2:
                    if (stylevar_enum < ImGuiStyleVar_COUNT && v_vec2.has_value()) {
                        ImGui::PopStyleVar();
                    }
                    break;

                case imgui_style_property_type::COLOR:
                    if (color_enum < ImGuiCol_COUNT && v_color.has_value()) {
                        ImGui::PopStyleColor();
                    }
                    break;

                case imgui_style_property_type::ITEM_WIDTH:
                    if (v_float.has_value()) ImGui::PopItemWidth();
                    break;

                default:
                    break;
            }
        }

    private:
        ImGuiStyleVar_ stylevar_enum = ImGuiStyleVar_COUNT;
        ImGuiCol_ color_enum = ImGuiCol_COUNT;

        std::optional<float> v_float {};
        std::optional<ImVec2> v_vec2 {};
        std::optional<ImVec4> v_color {};
    };

}
