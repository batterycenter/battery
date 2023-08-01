#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/core/vec.hpp"
#include "battery/core/color.hpp"

namespace b {

    class BatchRenderer : public sf::Drawable {
    public:
        BatchRenderer();
//
//        void reserve(size_t numberOfTriangles) {
////        m_va.reserve(numberOfTriangles * 3);
//        }

        void drawLine(const b::Vec2& pos1, const b::Vec2& pos2, const b::Color& color, double thickness);

        void drawRect(const b::Vec2& pos,
                      const b::Vec2& size,
                      const b::Color& fillColor,
                      double outlineThickness = 0.f,
                      const b::Color& outlineColor = sf::Color::Black);

        void draw(sf::RenderTarget& target, const sf::RenderStates& state) const override;
        void clear();

    private:
        void appendSimpleRect(const b::Vec2& pos1,
                              const b::Vec2& pos2,
                              const b::Vec2& pos3,
                              const b::Vec2& pos4,
                              const b::Color& color);

        void appendSimpleLine(const b::Vec2& pos1,
                              const b::Vec2& pos2,
                              const b::Color& color,
                              double thickness);

    private:
        sf::RectangleShape m_rectShape;
        sf::VertexArray m_va;
    };

}