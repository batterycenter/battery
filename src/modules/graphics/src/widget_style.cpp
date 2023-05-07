
#include "battery/graphics/widget_style.hpp"
#include "battery/graphics/property_stack.hpp"

namespace b {

    unit_property& widget_style::operator[](const std::string& str) {
        return properties[str];
    }

    void widget_style::push() {
        if (pushed) throw std::logic_error("Cannot push b::widgets::style property: Push/Pop mismatch!");

        for (auto& [key, property] : properties) {
            property_stack::push(key, property);
        }
        pushed = true;
    }

    void widget_style::pop() {
        if (!pushed) throw std::logic_error("Cannot pop b::widgets::style property: Push/Pop mismatch!");

        for (auto& [key, property] : properties) {
            property_stack::pop();
        }
        pushed = false;
    }

}