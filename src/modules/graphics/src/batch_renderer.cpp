
#include "battery/graphics/batch_renderer.hpp"

namespace b {

    BatchRenderer::BatchRenderer() {
        m_va.setPrimitiveType(sf::PrimitiveType::Triangles);
    }

    void BatchRenderer::drawLine(const sf::Vector2f& pos1, const sf::Vector2f& pos2, const sf::Color& color, float thickness) {
        auto span = pos2 - pos1;
        m_rectShape.setPosition(pos1);
        m_rectShape.setOrigin(sf::Vector2f(0, thickness / 2.f));
        m_rectShape.setSize(sf::Vector2f(span.length(), thickness));
        m_rectShape.setFillColor(color);
        m_rectShape.setOutlineThickness({});
        m_rectShape.setOutlineColor({});
        m_rectShape.setRotation(sf::radians(std::atan2(span.y, span.x)));

        auto transform = m_rectShape.getTransform();
        if (m_rectShape.getFillColor() != sf::Color::Transparent) {
            appendSimpleRect(transform.transformPoint(m_rectShape.getPoint(0)),
                             transform.transformPoint(m_rectShape.getPoint(1)),
                             transform.transformPoint(m_rectShape.getPoint(2)),
                             transform.transformPoint(m_rectShape.getPoint(3)),
                             m_rectShape.getFillColor());
        }
    }


    void BatchRenderer::drawRect(sf::Vector2f pos, sf::Vector2f size, sf::Color fillColor, float outlineThickness, sf::Color outlineColor) {
        m_rectShape.setPosition(pos);
        m_rectShape.setOrigin({});
        m_rectShape.setSize(size);
        m_rectShape.setFillColor(fillColor);
        m_rectShape.setOutlineThickness(outlineThickness);
        m_rectShape.setOutlineColor(outlineColor);
        m_rectShape.setRotation({});

        auto transform = m_rectShape.getTransform();
        if (m_rectShape.getFillColor() != sf::Color::Transparent) {
            appendSimpleRect(transform.transformPoint(m_rectShape.getPoint(0)),
                             transform.transformPoint(m_rectShape.getPoint(1)),
                             transform.transformPoint(m_rectShape.getPoint(2)),
                             transform.transformPoint(m_rectShape.getPoint(3)),
                             m_rectShape.getFillColor());
        }
        if (m_rectShape.getOutlineColor() != sf::Color::Transparent && m_rectShape.getOutlineThickness() > 0.0f) {
            appendSimpleLine(transform.transformPoint(m_rectShape.getPoint(0)),
                             transform.transformPoint(m_rectShape.getPoint(1)),
                             m_rectShape.getOutlineColor(),
                             m_rectShape.getOutlineThickness());
            appendSimpleLine(transform.transformPoint(m_rectShape.getPoint(1)),
                             transform.transformPoint(m_rectShape.getPoint(2)),
                             m_rectShape.getOutlineColor(),
                             m_rectShape.getOutlineThickness());
            appendSimpleLine(transform.transformPoint(m_rectShape.getPoint(2)),
                             transform.transformPoint(m_rectShape.getPoint(3)),
                             m_rectShape.getOutlineColor(),
                             m_rectShape.getOutlineThickness());
            appendSimpleLine(transform.transformPoint(m_rectShape.getPoint(3)),
                             transform.transformPoint(m_rectShape.getPoint(0)),
                             m_rectShape.getOutlineColor(),
                             m_rectShape.getOutlineThickness());
        }
    }

    void BatchRenderer::render(sf::RenderTarget& target) {
        target.draw(m_va);
        m_va.clear();
    }

    void BatchRenderer::appendSimpleRect(sf::Vector2f pos1, sf::Vector2f pos2, sf::Vector2f pos3, sf::Vector2f pos4, sf::Color color) {
        m_va.append(sf::Vertex(pos1, color));
        m_va.append(sf::Vertex(pos2, color));
        m_va.append(sf::Vertex(pos3, color));
        m_va.append(sf::Vertex(pos3, color));
        m_va.append(sf::Vertex(pos4, color));
        m_va.append(sf::Vertex(pos1, color));
    }

    void BatchRenderer::appendSimpleLine(sf::Vector2f pos1, sf::Vector2f pos2, sf::Color color, float thickness) {
        auto dir = pos2 - pos1;
        auto normDir = dir.length() > 0 ? dir.normalized() : sf::Vector2f(0, 0);
        auto perp = dir.length() > 0 ? sf::Vector2f(dir.y, -dir.x).normalized() : sf::Vector2f(0, 0);
        auto vert1 = pos1 - perp * thickness / 2.f - normDir * thickness / 2.f;
        auto vert2 = pos1 + perp * thickness / 2.f - normDir * thickness / 2.f;
        auto vert3 = pos2 - perp * thickness / 2.f + normDir * thickness / 2.f;
        auto vert4 = pos2 + perp * thickness / 2.f + normDir * thickness / 2.f;
        appendSimpleRect(vert1, vert2, vert4, vert3, color);
    }

}