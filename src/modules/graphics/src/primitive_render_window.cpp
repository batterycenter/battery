
#include "battery/graphics/primitive_render_window.hpp"
#include <cmath>

namespace b {

    void PrimitiveRenderWindow::drawLine(const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& color, float thickness, LineCap cap) {
        sf::RectangleShape line;
        line.setFillColor(color);
        line.setSize({ b::distance(point1, point2), thickness });
        line.setOrigin({ 0, thickness / 2.0f });
        line.setPosition(point1);
        line.setRotation(sf::radians(atan2f(point2.y - point1.y, point2.x - point1.x)));
        sfmlWindow.draw(line);

        if (cap == LineCap::Round) {
            sf::CircleShape circle;
            circle.setFillColor(color);
            circle.setRadius(thickness / 2.0f);
            circle.setOrigin({ thickness / 2.0f, thickness / 2.0f });
            circle.setPosition(point1);
            sfmlWindow.draw(circle);
            circle.setPosition(point2);
            sfmlWindow.draw(circle);
        }
    }

    void PrimitiveRenderWindow::drawRect(const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& fillColor, float outlineThickness, const sf::Color& outlineColor) {
        sf::RectangleShape rect;
        rect.setFillColor(fillColor);
        rect.setOutlineColor(fillColor);
        rect.setOutlineThickness(1.0f);
        rect.setOrigin({ 0, 0 });
        rect.setSize({ point2.x - point1.x, point2.y - point1.y });
        rect.setPosition(point1);
        rect.setOutlineColor(outlineColor);
        rect.setOutlineThickness(outlineThickness);
        sfmlWindow.draw(rect);
    }

} // namespace b