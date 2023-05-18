#pragma once

#include "battery/graphics/sfml.hpp"

namespace b {

    ImColor color_hex(const std::string& hex);

    std::string color_hex(ImColor color);

}