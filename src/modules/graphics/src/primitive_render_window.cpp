
#include "battery/graphics/primitive_render_window.hpp"
#include <cmath>

namespace b {

    void PrimitiveRenderWindow::drawLine(const b::vec2& point1, const b::vec2& point2, const sf::Color& color, double thickness, LineCap cap) {
        sf::RectangleShape line;
        line.setFillColor(color);
        line.setSize(b::vec2(b::distance(point1, point2), thickness));
        line.setOrigin(b::vec2(0, thickness / 2.0));
        line.setPosition(point1);
        line.setRotation(sf::radians(atan2f(point2.y - point1.y, point2.x - point1.x)));
        sfmlWindow.draw(line);

        if (cap == LineCap::Round) {
            sf::CircleShape circle;
            circle.setFillColor(color);
            circle.setRadius(thickness / 2.0);
            circle.setOrigin(b::vec2(thickness / 2.0, thickness / 2.0));
            circle.setPosition(point1);
            sfmlWindow.draw(circle);
            circle.setPosition(point2);
            sfmlWindow.draw(circle);
        }
    }

    void PrimitiveRenderWindow::drawRect(const b::vec2& point1, const b::vec2& point2, const sf::Color& fillColor, double outlineThickness, const sf::Color& outlineColor) {
        sf::RectangleShape rect;
        rect.setFillColor(fillColor);
        rect.setOutlineColor(fillColor);
        rect.setOutlineThickness(1.0);
        rect.setOrigin({ 0, 0 });
        rect.setSize(b::vec2(point2.x - point1.x, point2.y - point1.y));
        rect.setPosition(point1);
        rect.setOutlineColor(outlineColor);
        rect.setOutlineThickness(outlineThickness);
        sfmlWindow.draw(rect);
    }

} // namespace b