
#include "MazeHost.hpp"

void MazeHost::init(b::BaseWindow* window) {
    m_window = window;
    m_maze = std::make_unique<Maze>(
            static_cast<int>(std::floor(m_window->getSize().x / GRID_SIZE.x)),
            static_cast<int>(std::floor(m_window->getSize().y / GRID_SIZE.y)));
    m_mazeGenerator = std::make_unique<MazeGenerator>(*m_maze);
    m_mazeSolver = std::make_unique<MazeSolver>(*m_maze);

    b::log::info("Generating Maze...");
}

void MazeHost::update() {
    switch (m_state) {
        case MazeState::Generating:
            for (int i = 0; i < FRAME_SPEEDUP; i++) {
                if (m_mazeGenerator->step()) {
                    b::log::info("Finished generating");
                    b::log::info("Solving...");
                    m_state = MazeState::Solving;
                    m_mazeSolver->init();
                    break;
                }
            }
            break;

        case MazeState::Solving:
            if (m_mazeSolver->step()) {
                b::log::info("Finished solving");
                m_state = MazeState::Finished;
            }
            break;

        case MazeState::Finished:
            break;
    }
}

void MazeHost::render() {
    m_window->clear(sf::Color::Black);
    drawMaze();
    m_batchRenderer.render(*m_window);
}

void MazeHost::drawMaze() {
    m_mazeArea = { m_maze->m_width * GRID_SIZE.x, m_maze->m_height * GRID_SIZE.y };
    m_leftUpperMazeCorner = m_window->getSize() / 2.f - m_mazeArea / 2.f;
    m_batchRenderer.drawRect(m_leftUpperMazeCorner, m_mazeArea, sf::Color::White);

    if (m_state == MazeState::Solving) {
        drawBranch(m_mazeSolver->m_branch);
    }

    for (int x = 0; x < m_maze->m_width; x++) {
        for (int y = 0; y < m_maze->m_height; y++) {
            auto leftUpper = m_leftUpperMazeCorner + b::vec2(x * GRID_SIZE.x, y * GRID_SIZE.y);
            auto rightLower = leftUpper + GRID_SIZE;
            if (m_maze->m_walls[x][y].m_bottomWall && y < m_maze->m_height - 1) {
                m_batchRenderer.drawLine({ leftUpper.x, rightLower.y }, rightLower, sf::Color::Black, 1.f);
            }
            if (m_maze->m_walls[x][y].m_rightWall && x < m_maze->m_width - 1) {
                m_batchRenderer.drawLine({ rightLower.x, leftUpper.y }, rightLower, sf::Color::Black, 1.f);
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

void MazeHost::drawCell(int x, int y, sf::Color color) {
    auto leftUpper = m_leftUpperMazeCorner + b::vec2(x * GRID_SIZE.x, y * GRID_SIZE.y);
    m_batchRenderer.drawRect(leftUpper, GRID_SIZE, color, 0.f);
}

void MazeHost::drawBranch(Branch& branch) {
    for (int i = 0; i < branch.m_path.size(); i++) {
        auto& path = branch.m_path[i];
        drawCell(path.x, path.y, branch.m_isDeadEnd ? sf::Color::Red : sf::Color::Green);
        b::log::debug("Drawing branch: {} {}", path.x, path.y);
    }
}
