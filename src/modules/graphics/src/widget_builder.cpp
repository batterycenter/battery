
#include "battery/graphics/widget_builder.hpp"
#include "battery/graphics/property_stack.hpp"
#include "magic_enum.hpp"

namespace b::widgets {

    widget_builder::numeric_rule::~numeric_rule() {
        if (pushed) {
            ImGui::PopStyleVar();
            pushed--;
        }
    }

    widget_builder::numeric_rule& widget_builder::numeric_rule::add_case(enum b::unit unit, const std::function<float(float)>& transformer) {
        cases.emplace_back(unit, transformer);
        return *this;
    }

    std::optional<float> widget_builder::numeric_rule::get_result() {
        auto property = property_stack::get(tag);
        if (property) {
            if (property.value().get_properties().size() == 1) {
                for (auto& [unit, transformer] : cases) {           // Apply all cases
                    if (property.value().get_properties()[0].unit() == unit) {
                        return transformer(property.value().get_properties()[0].numeric());
                    }
                }
            } else {
                throw std::runtime_error(fmt::format(
                        "Failed to apply widget_builder rule {}: Loaded property pack has {} values, expected 1!",
                        tag, property.value().get_properties().size()));
            }
        }
        return {};
    }

    void widget_builder::numeric_rule::push(ImGuiStyleVar_ style_var) {
        if (auto result = get_result()) {
            ImGui::PushStyleVar(style_var, result.value());
            pushed++;
        }
    }





    widget_builder::vec2_rule::~vec2_rule() {
        while (pushed) {
            ImGui::PopStyleVar();
            pushed--;
        }
    }

    widget_builder::vec2_rule& widget_builder::vec2_rule::add_case_x(enum b::unit unit, const std::function<float(float)>& transformer) {
        cases_x.emplace_back(unit, transformer);
        return *this;
    }

    widget_builder::vec2_rule& widget_builder::vec2_rule::add_case_y(enum b::unit unit, const std::function<float(float)>& transformer) {
        cases_y.emplace_back(unit, transformer);
        return *this;
    }

    std::optional<ImVec2> widget_builder::vec2_rule::get_result() {
        auto property = property_stack::get(tag);
        if (property) {
            if (property.value().get_properties().size() != 2) {
                throw std::runtime_error(fmt::format(
                        "Failed to apply widget_builder rule {}: Loaded property pack has {} values, expected 2!",
                        tag, property.value().get_properties().size()));
            }

            ImVec2 result(0, 0);
            for (auto& [unit, transformer] : cases_x) {
                if (property.value().get_properties()[0].unit() == unit) {
                    result.x = transformer(property.value().get_properties()[0].numeric());
                }
            }
            for (auto& [unit, transformer] : cases_y) {
                if (property.value().get_properties()[1].unit() == unit) {
                    result.y = transformer(property.value().get_properties()[1].numeric());
                }
            }
            return result;
        }
        return {};
    }

    void widget_builder::vec2_rule::push(ImGuiStyleVar_ style_var) {
        if (auto result = get_result()) {
            ImGui::PushStyleVar(style_var, result.value());
            pushed++;
        }
    }





    widget_builder::color_rule::~color_rule() {
        while (pushed) {
            ImGui::PopStyleColor();
            pushed--;
        }
    }

    std::optional<ImVec4> widget_builder::color_rule::get_result() {
        auto property = property_stack::get(tag);
        if (property) {
            if (property.value().get_properties().size() != 1) {
                throw std::runtime_error(fmt::format(
                        "Failed to apply widget_builder rule {}: Loaded property pack has {} values, expected 1!",
                        tag, property.value().get_properties().size()));
            }

            if (property.value().get_properties()[0].unit() != b::unit::COLOR_HEX) {
                throw std::runtime_error(fmt::format(
                        "Failed to apply widget_builder rule {}: Property type is {}, expected {}",
                        tag, magic_enum::enum_name(property.value().get_properties()[0].unit()), magic_enum::enum_name(b::unit::COLOR_HEX)));
            }

            return property.value().get_properties()[0].color();
        }
        return {};
    }

    void widget_builder::color_rule::push(ImGuiCol_ style_var) {
        if (auto result = get_result()) {
            ImGui::PushStyleColor(style_var, result.value());
            pushed++;
        }
    }






    widget_builder::widget_builder(widget_style& style) : style(style) {
        style.push();
    }

    widget_builder::~widget_builder() {
        pop();
    }

    void widget_builder::pop() {
        if (!released) {
            numeric_rules.clear();
            vec2_rules.clear();
            color_rules.clear();
            style.pop();
            released = true;
        }
    }

    widget_builder::numeric_rule& widget_builder::add_numeric_rule(const char* tag) {
        numeric_rules.emplace_back(tag);
        return numeric_rules.back();
    }

    widget_builder::vec2_rule& widget_builder::add_vec2_rule(const char* tag) {
        vec2_rules.emplace_back(tag);
        return vec2_rules.back();
    }

    widget_builder::color_rule& widget_builder::add_color_rule(const char* tag) {
        color_rules.emplace_back(tag);
        return color_rules.back();
    }

}