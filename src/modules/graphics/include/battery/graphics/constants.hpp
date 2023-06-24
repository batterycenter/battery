#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/core/log.hpp"

namespace b {

    // We use static functions instead of static global variables to have defined lifetime, the ability
    // to catch first-time constructor exceptions and avoid static initialization order fiascos
    // https://isocpp.org/wiki/faq/ctors#static-init-order
    namespace Constants {
        sf::Color DefaultWindowBackgroundColor();
        std::pair<b::string, double> DefaultFont();
        b::vec2 DefaultWindowSize();
        b::string DefaultWindowTitle();
    } // namespace Constants

} // namespace b