#pragma once

#include "battery/graphics/sfml.hpp"

namespace b {

    ImVec4 color_hex(const std::string& hex);

    std::string color_hex(ImVec4 color);

}