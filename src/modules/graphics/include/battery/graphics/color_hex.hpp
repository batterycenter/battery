#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/core/log.hpp"

namespace b {

    ImColor color_hex(const b::string& hex);

    b::string color_hex(ImColor color);

}