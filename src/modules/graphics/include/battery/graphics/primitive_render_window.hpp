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

        sf::RenderWindow& m_sfmlWindow = *this;

        void drawLine(const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& color = sf::Color::Black, float thickness = 1.0f, LineCap cap = LineCap::Square);
        void drawRect(const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& fillColor = sf::Color::Black, float outlineThickness = 1.0f, const sf::Color& outlineColor = sf::Color::Black);
    };

} // namespace b