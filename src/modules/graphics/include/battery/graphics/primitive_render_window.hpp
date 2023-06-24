#pragma once

#include "battery/graphics/sfml.hpp"

namespace b {

    enum class LineCap {
        Square,
        Round
    };

    class PrimitiveRenderWindow : public sf::RenderWindow {
    public:
        PrimitiveRenderWindow() = default;

        sf::RenderWindow& sfmlWindow = *this;

        void drawLine(const b::vec2& point1, const b::vec2& point2, const sf::Color& color = sf::Color::Black, double thickness = 1.0, LineCap cap = LineCap::Square);
        void drawRect(const b::vec2& point1, const b::vec2& point2, const sf::Color& fillColor = sf::Color::Black, double outlineThickness = 1.0, const sf::Color& outlineColor = sf::Color::Black);
    };

} // namespace b