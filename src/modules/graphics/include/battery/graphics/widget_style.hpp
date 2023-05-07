#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/graphics/unit_property.hpp"

namespace b {

    class widget_style {
    public:
        widget_style() = default;

        unit_property& operator[](const std::string& str);

        void push();
        void pop();

    private:
        bool pushed = false;
        std::unordered_map<std::string, unit_property> properties;
    };

}