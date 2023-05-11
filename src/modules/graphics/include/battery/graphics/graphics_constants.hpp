#pragma once

#include "battery/graphics/sfml.hpp"

namespace b {

    // These constants are defined as static getter functions instead of variables,
    // to avoid a 'static initialization order fiasco' https://isocpp.org/wiki/faq/ctors#static-init-order
    struct constants {
        static sf::Color battery_default_background_color();
        static std::pair<std::string, float> battery_default_font();
    };

}