#pragma once

#include "battery/graphics/sfml.hpp"
#include "battery/core/vec.hpp"

namespace b {

    class BatchRenderer : public sf::Drawable {
    public:
        BatchRenderer();
//
//        void reserve(size_t numberOfTriangles) {
////        m_va.reserve(numberOfTriangles * 3);
//        }

        void drawLine(const b::vec2& pos1, const b::vec2& pos2, const sf::Color& color, double thickness);

        void drawRect(const b::vec2& pos,
                      const b::vec2& size,
                      sf::Color fillColor,
                      double outlineThickness = 0.f,
                      sf::Color outlineColor = sf::Color::Black);

        void draw(sf::RenderTarget& target, const sf::RenderStates& state) const override;
        void clear();

    private:
        void appendSimpleRect(const b::vec2& pos1,
                              const b::vec2& pos2,
                              const b::vec2& pos3,
                              const b::vec2& pos4,
                              sf::Color color);

        void appendSimpleLine(const b::vec2& pos1,
                              const b::vec2& pos2,
                              sf::Color color,
                              double thickness);

    private:
        sf::RectangleShape m_rectShape;
        sf::VertexArray m_va;
    };

}