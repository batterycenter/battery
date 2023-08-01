
#include "battery/graphics/mouse.hpp"

namespace b {

    void MousePositionData::updatePositionData(const b::Vec2& mousePos) {
        posPrevious = pos;
        pos = mousePos;
        posDelta = pos - posPrevious;
    }

    void MousePositionData::setScrollDelta(const b::Vec2& scrollDelta) {
        this->scrollDelta = scrollDelta;
    }

    void MousePositionData::updateButtonData() {
        leftButtonPressedPrevious = leftButtonPressed;
        rightButtonPressedPrevious = rightButtonPressed;
        mouseWheelPressedPrevious = mouseWheelPressed;
        xButton1PressedPrevious = xButton1Pressed;
        xButton2PressedPrevious = xButton2Pressed;

        leftButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        rightButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
        mouseWheelPressed = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
        xButton1Pressed = sf::Mouse::isButtonPressed(sf::Mouse::XButton1);
        xButton2Pressed = sf::Mouse::isButtonPressed(sf::Mouse::XButton2);
    }

    void MousePositionData::freezeData() {
        posPrevious = pos;
        posDelta = b::Vec2(0, 0);
        scrollDelta = b::Vec2(0, 0);
        leftButtonPressedPrevious = leftButtonPressed;
        rightButtonPressedPrevious = rightButtonPressed;
        mouseWheelPressedPrevious = mouseWheelPressed;
        xButton1PressedPrevious = xButton1Pressed;
        xButton2PressedPrevious = xButton2Pressed;
    }

} // namespace b