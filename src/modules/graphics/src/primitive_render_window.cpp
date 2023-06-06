
#include "battery/graphics/primitive_render_window.hpp"

namespace b {

    void PrimitiveRenderWindow::drawLine(const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& color, float thickness, LineCap cap) {
        sf::RectangleShape line;
        line.setFillColor(color);
        line.setSize({ b::distance(point1, point2), thickness });
        line.setOrigin({ 0, thickness / 2.0f });
        line.setPosition(point1);
        line.setRotation(sf::radians(std::atan2f(point2.y - point1.y, point2.x - point1.x)));
        m_sfmlWindow.draw(line);

        if (cap == LineCap::Round) {
            sf::CircleShape circle;
            circle.setFillColor(color);
            circle.setRadius(thickness / 2.0f);
            circle.setOrigin({ thickness / 2.0f, thickness / 2.0f });
            circle.setPosition(point1);
            m_sfmlWindow.draw(circle);
            circle.setPosition(point2);
            m_sfmlWindow.draw(circle);
        }
    }

} // namespace b