#pragma once

#include "battery/graphics/sfml.hpp"

namespace b {

    class MousePositionData {
    public:
        MousePositionData() = default;

        b::Vec2 pos;
        b::Vec2 posPrevious;
        b::Vec2 posDelta;

        b::Vec2 scrollDelta;

        bool leftButtonPressed = false;
        bool leftButtonPressedPrevious = false;
        bool rightButtonPressed = false;
        bool rightButtonPressedPrevious = false;
        bool mouseWheelPressed = false;
        bool mouseWheelPressedPrevious = false;
        bool xButton1Pressed = false;
        bool xButton1PressedPrevious = false;
        bool xButton2Pressed = false;
        bool xButton2PressedPrevious = false;

        void updatePositionData(const b::Vec2& mousePos);
        void setScrollDelta(const b::Vec2& scrollDelta);
        void updateButtonData();
        void freezeData();
    };

} // namespace b