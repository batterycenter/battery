#pragma once

#include "battery/graphics/widget_style.hpp"
#include "nlohmann/json.hpp"

namespace b {

    void make_theme_available(const b::string& name, const nlohmann::json& data);

    void make_default_themes_available();

    void load_theme(const b::string& name);

    void update_themes();   // Call this for hotreload

    void apply_theme(const nlohmann::json& data);

}