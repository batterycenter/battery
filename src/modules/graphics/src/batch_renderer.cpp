
#include "battery/graphics/batch_renderer.hpp"

namespace b {

    BatchRenderer::BatchRenderer() {
        m_va.setPrimitiveType(sf::PrimitiveType::Triangles);
    }

    void BatchRenderer::drawLine(const b::Vec2& pos1, const b::Vec2& pos2, const b::Color& color, double thickness) {
        auto span = pos2 - pos1;
        m_rectShape.setPosition(pos1);
        m_rectShape.setOrigin(b::Vec2(0, thickness));
        m_rectShape.setSize(b::Vec2(span.length(), thickness));
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


    void BatchRenderer::drawRect(const b::Vec2& pos, const b::Vec2& size, const b::Color& fillColor,
                                 double outlineThickness, const b::Color& outlineColor) {
        m_rectShape.setPosition(pos);
        m_rectShape.setOrigin({});
        m_rectShape.setSize(size);
        m_rectShape.setFillColor(fillColor);
        m_rectShape.setOutlineThickness(static_cast<float>(outlineThickness));
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
        if (m_rectShape.getOutlineColor() != sf::Color::Transparent && m_rectShape.getOutlineThickness() > 0.0) {
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

    void BatchRenderer::draw(sf::RenderTarget& target, const sf::RenderStates& state) const {
        target.draw(m_va, state);
    }

    void BatchRenderer::clear() {
        m_va.clear();
    }

    void BatchRenderer::appendSimpleRect(const b::Vec2& pos1, const b::Vec2& pos2, const b::Vec2& pos3,
                                         const b::Vec2& pos4, const b::Color& color) {
        m_va.append(sf::Vertex(pos1, color));
        m_va.append(sf::Vertex(pos2, color));
        m_va.append(sf::Vertex(pos3, color));
        m_va.append(sf::Vertex(pos3, color));
        m_va.append(sf::Vertex(pos4, color));
        m_va.append(sf::Vertex(pos1, color));
    }

    void BatchRenderer::appendSimpleLine(const b::Vec2& pos1, const b::Vec2& pos2,
                                         const b::Color& color, double thickness) {
        auto dir = pos2 - pos1;
        auto normDir = dir.normalized(true);
        auto perp = dir.perpendicular().normalized(true);
        auto vert1 = pos1 - perp * thickness / 2.0 - normDir * thickness / 2.0;
        auto vert2 = pos1 + perp * thickness / 2.0 - normDir * thickness / 2.0;
        auto vert3 = pos2 - perp * thickness / 2.0 + normDir * thickness / 2.0;
        auto vert4 = pos2 + perp * thickness / 2.0 + normDir * thickness / 2.0;
        appendSimpleRect(vert1, vert2, vert4, vert3, color);
    }

}