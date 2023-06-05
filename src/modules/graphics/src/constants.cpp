
#include "battery/graphics/constants.hpp"

namespace b {

    sf::Color Constants::DefaultWindowBackgroundColor() {
        return { 20, 20, 20, 255 };
    }

    std::pair<b::string, float> Constants::DefaultFont() {
        return { "roboto-regular", 18.f };
    }

    sf::Vector2u Constants::DefaultWindowSize() {
        return { 1280, 720 };
    }

    b::string Constants::DefaultWindowTitle() {
        return "Battery Window";
    }

} // namespace b
