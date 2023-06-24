
#include "MazeSolver.hpp"

void MazeSolver::init() {
    for (int x = 0; x < m_maze.m_width; x++) {
        for (int y = 0; y < m_maze.m_height; y++) {
            m_maze.m_walls[x][y].identifier = 0;
        }
    }
    m_startPosition = sf::Vector2i(b::randomInt(m_maze.m_width), m_maze.m_height - 1);
    m_targetPosition = sf::Vector2i(b::randomInt(m_maze.m_width), 0);

    m_branch = Branch();
    m_branch.m_path.push_back(m_startPosition);
}

void stepBranch(Branch& branch) {
    sf::Vector2i currentPosition = branch.m_path.back();


}

bool MazeSolver::step() {
    stepBranch(m_branch);
    return false;
}
