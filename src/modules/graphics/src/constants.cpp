
#include "battery/graphics/constants.hpp"

namespace b {

    sf::Color Constants::DefaultWindowBackgroundColor() {
        return { 20, 20, 20, 255 };
    }

    std::pair<b::string, double> Constants::DefaultFont() {
        return { "roboto-regular", 18.f };
    }

    b::Vec2 Constants::DefaultWindowSize() {
        return { 1280, 720 };
    }

    b::string Constants::DefaultWindowTitle() {
        return "Battery Window";
    }

} // namespace b
