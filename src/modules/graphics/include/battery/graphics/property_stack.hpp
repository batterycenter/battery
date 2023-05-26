#pragma once

#include "battery/graphics/property_pack.hpp"

namespace b {

    class property_stack {
    public:

        static void push(const b::string& property_name, const property_pack& property_value);
        static void pop();
        static void clear();

        static std::optional<property_pack> get(const b::string& property_name);
        static std::optional<ImColor> get_color(const b::string& property_name);

        template<typename T>
        static T get(const b::string& prop, T defaultValue) {
            auto value = get(prop);
            if (value.has_value()) {
                if (value.value().get_properties().size() == 1) {
                    return static_cast<T>(value.value().get_properties()[0]);
                }
            }
            return defaultValue;
        }

    private:
        property_stack() = default;

        static void pushpop_stylevar(ImGuiStyleVar_ stylevar_enum, const b::string& property_name, const property_pack& property_value, bool push);
        static void pushpop_color(ImGuiCol_ color_enum, const b::string& property_name, const property_pack& property_value, bool push);

        inline static std::vector<std::pair<b::string, property_pack>> properties;
    };

}