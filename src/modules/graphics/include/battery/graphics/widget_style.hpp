#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/graphics/unit_property.hpp"
#include "battery/graphics/property_stack.hpp"

namespace b {

    class widget_style {
    public:
        widget_style() = default;

        inline property_pack& operator[](const std::string& str) {
            return properties[str];
        }

        inline void push() {
            if (pushed) throw std::logic_error("Cannot push b::widgets::style property: Push/Pop mismatch!");
            pushed_properties = properties;
            for (auto& [key, property] : pushed_properties) {
                property_stack::push(key, property);
            }
            pushed = true;
        }

        inline void pop() {
            if (!pushed) throw std::logic_error("Cannot pop b::widgets::style property: Push/Pop mismatch!");
            for (auto& [key, property] : pushed_properties) {
                property_stack::pop();
            }
            pushed = false;
        }

    private:
        bool pushed = false;
        std::unordered_map<std::string, property_pack> properties;
        std::unordered_map<std::string, property_pack> pushed_properties;
    };

}