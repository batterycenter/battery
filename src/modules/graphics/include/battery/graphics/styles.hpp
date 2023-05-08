#pragma once

#include "battery/graphics/widget_style.hpp"
#include "nlohmann/json.hpp"

namespace b {

    void load_default_imgui_style();
    void load_default_imgui_style(const nlohmann::json& style);

    void push_default_battery_style(widget_style& style);

}