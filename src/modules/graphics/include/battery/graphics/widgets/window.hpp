#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class window : public base_widget {
    public:
        int flags = ImGuiWindowFlags_None;

        window() : base_widget("Battery Window") {}
        explicit window(const std::string& name, int flags = 0) : base_widget(name), flags(flags) {}

        void operator()(const std::function<void()>& callback);
    };

}