#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class window : public base_widget {
    public:
        int flags = ImGuiWindowFlags_None;

        std::optional<ImVec2> position;
        std::optional<ImVec2> size;

        ImVec2 actual_position;
        ImVec2 actual_size;

        window() : base_widget("Battery Window") {}
        explicit window(const std::string& name, int flags = 0) : base_widget(name), flags(flags) {}

        void operator()(const std::function<void()>& callback);
    };

}