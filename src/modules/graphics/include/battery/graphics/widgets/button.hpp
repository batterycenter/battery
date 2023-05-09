#pragma once

#include "battery/graphics/widgets/base_widget.hpp"
#include "battery/graphics/widget_style.hpp"

namespace b::widgets {

class button : public b::widgets::base_widget {
    public:

        bool clicked = false;		// Single trigger when clicked
        bool held = false;
        bool hovered = false;

        ImVec2 size;
        ImVec2 actual_size;
        widget_style style;

        bool sameline = false;

        button() : base_widget("Button") {}
        explicit button(const std::string& name) : base_widget(name) {}

        void operator()(const std::function<void()>& callback = nullptr) override;

    protected:
        std::function<std::tuple<bool, bool, bool>()> custom_implementation {};
    };

}
