#pragma once

#include "battery/graphics/sfml.hpp"

namespace b {

    class BatchRenderer {
    public:
        BatchRenderer();
//
//        void reserve(size_t numberOfTriangles) {
////        m_va.reserve(numberOfTriangles * 3);
//        }

        void drawLine(const sf::Vector2f& pos1, const sf::Vector2f& pos2, const sf::Color& color, float thickness);
        void drawRect(sf::Vector2f pos, sf::Vector2f size, sf::Color fillColor, float outlineThickness = 0.f, sf::Color outlineColor = sf::Color::Black);

        void render(sf::RenderTarget& target);

    private:
        void appendSimpleRect(sf::Vector2f pos1, sf::Vector2f pos2, sf::Vector2f pos3, sf::Vector2f pos4, sf::Color color);
        void appendSimpleLine(sf::Vector2f pos1, sf::Vector2f pos2, sf::Color color, float thickness);

    private:
        sf::RectangleShape m_rectShape;
        sf::VertexArray m_va;
    };

}