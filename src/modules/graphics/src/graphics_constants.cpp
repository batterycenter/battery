
#include "battery/graphics/graphics_constants.hpp"

namespace b {

    sf::Color constants::battery_default_background_color() {
        return sf::Color(20, 20, 20, 255);
    }

    std::pair<std::string, float> constants::battery_default_font() {
        return { "roboto-regular", 18.f };
    }

}
