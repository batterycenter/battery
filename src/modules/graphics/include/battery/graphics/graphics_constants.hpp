#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/core/log.hpp"

namespace b {

    // These constants are defined as static getter functions instead of variables,
    // to avoid a 'static initialization order fiasco' https://isocpp.org/wiki/faq/ctors#static-init-order
    struct graphics_constants {
        static sf::Color battery_default_background_color();
        static std::pair<b::string, float> battery_default_font();
        static sf::Vector2u default_window_size();
        static b::string default_window_title();
    };

}