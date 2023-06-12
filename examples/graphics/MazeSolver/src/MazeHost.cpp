
#include "MazeHost.hpp"

void MazeHost::init(ImVec2 windowSize) {
    m_maze = std::make_unique<Maze>(
            static_cast<int>(std::floor(windowSize.x / GRID_SIZE.x)),
            static_cast<int>(std::floor(windowSize.y / GRID_SIZE.y)));
    m_mazeGenerator = std::make_unique<MazeGenerator>(*m_maze);
}

void MazeHost::update() {
    for (int i = 0; i < FRAME_SPEEDUP; i++) {
        if (m_mazeGenerator->step()) {
            b::log::error("Finished");
            break;
        }
    }
}

void MazeHost::render(b::BaseWindow& window) {
    window.clear(sf::Color::Black);

    b::log::debug("Framerate: {} FPS", window.m_framerate);

    auto mazeArea = sf::Vector2(m_maze->m_width * GRID_SIZE.x, m_maze->m_height * GRID_SIZE.y);
    auto leftAreaCorner = window.getSize() / 2.f - mazeArea / 2.f;
    auto rightAreaCorner = window.getSize() / 2.f + mazeArea / 2.f;
    window.drawRect(leftAreaCorner, rightAreaCorner, sf::Color::White);

    for (int x = 0; x < m_maze->m_width; x++) {
        for (int y = 0; y < m_maze->m_height; y++) {
            auto leftUpper = leftAreaCorner + sf::Vector2(x * GRID_SIZE.x, y * GRID_SIZE.y);
            auto rightLower = leftUpper + GRID_SIZE;
            if (m_maze->m_walls[x][y].m_bottomWall && y < m_maze->m_height - 1) {
                window.drawLine({ leftUpper.x, rightLower.y }, rightLower, sf::Color::Black, 1.f);
            }
            if (m_maze->m_walls[x][y].m_rightWall && x < m_maze->m_width - 1) {
                window.drawLine({ rightLower.x, leftUpper.y }, rightLower, sf::Color::Black, 1.f);
            }
//            sf::Font font;
//            (void)font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
//            sf::Text text(font, std::to_string(m_maze->m_walls[x][y].identifier), 20);
//            text.setPosition({ leftUpper.x + 10, leftUpper.y + 10 });
//            text.setFillColor(sf::Color::Black);
//            window.draw(text);
        }
    }
}
