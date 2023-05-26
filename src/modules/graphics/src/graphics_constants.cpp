
#include "battery/graphics/graphics_constants.hpp"

namespace b {

    sf::Color graphics_constants::battery_default_background_color() {
        return sf::Color(20, 20, 20, 255);
    }

    std::pair<b::string, float> graphics_constants::battery_default_font() {
        return { "roboto-regular", 18.f };
    }

}
