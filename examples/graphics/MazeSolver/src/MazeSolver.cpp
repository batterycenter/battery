
#include "MazeSolver.hpp"

void MazeSolver::init() {
    for (int x = 0; x < m_maze.m_width; x++) {
        for (int y = 0; y < m_maze.m_height; y++) {
            m_maze.m_walls[x][y].identifier = 0;
        }
    }
    m_startPosition = b::vec2(b::randomInt(m_maze.m_width), m_maze.m_height - 1);
    m_targetPosition = b::vec2(b::randomInt(m_maze.m_width), 0);

    m_branch = Branch();
    m_branch.m_path.push_back(m_startPosition);
}

void stepBranch(Branch& branch) {
    b::vec2 currentPosition = branch.m_path.back();


}

bool MazeSolver::step() {
    stepBranch(m_branch);
    return false;
}
