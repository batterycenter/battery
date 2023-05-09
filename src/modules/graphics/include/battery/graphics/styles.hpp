#pragma once

#include "battery/graphics/widget_style.hpp"
#include "nlohmann/json.hpp"

namespace b {

    void make_theme_available(const std::string& name, const nlohmann::json& data);

    void make_default_themes_available();

    void load_theme(const std::string& name);

    void apply_theme(const nlohmann::json& data);

}