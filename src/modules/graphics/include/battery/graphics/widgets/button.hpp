#pragma once

#include "battery/graphics/widgets/base_widget.hpp"
#include "battery/graphics/widget_style.hpp"

namespace b::widgets {

class button : public b::widgets::base_widget {
    public:

        bool clicked = false;		// Single trigger when clicked
        bool held = false;
        bool hovered = false;

        bool sameline = false;
        widget_style style {};

        button() : base_widget("Button") {}
        explicit button(const std::string& name) : base_widget(name) {}

        void operator()(const std::function<void()>& callback = nullptr) override;
    };

}
