#pragma once

#include "battery/graphics/unit_property.hpp"

namespace b {

    class property_stack {
    public:

        static void push(const std::string& property_name, const unit_property& property_value);
        static void pop();

        static std::optional<unit_property> get(const std::string& property_name);

        template<typename T>
        static T get(const std::string& prop, T defaultValue) {
            auto value = get(prop);
            if (value.has_value()) {
                return (T)value.value();
            }
            return defaultValue;
        }

    private:
        property_stack() = default;

        static void push_stylevar(ImGuiStyleVar_ stylevar_enum, const std::string& property_name, const unit_property& property_value);
        static void push_color(ImGuiCol_ color_enum, const std::string& property_name, const unit_property& property_value);

        inline static std::vector<std::pair<std::string, unit_property>> properties;
    };

}