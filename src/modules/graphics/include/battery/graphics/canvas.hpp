#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/graphics/mouse.hpp"

namespace b {

    class Canvas : public sf::RenderTexture {
    public:
        MousePositionData mouse;

        Canvas();

    };

}