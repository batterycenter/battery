
#include "MazeGenerator.hpp"

MazeGenerator::MazeGenerator(Maze& maze) : m_maze(maze) {
    for (int x = 0; x < m_maze.m_width; x++) {
        for (int y = 0; y < m_maze.m_height; y++) {
            m_maze.m_walls[x][y].identifier = x + y * m_maze.m_width;
            m_maze.m_walls[x][y].m_bottomWall = true;
            m_maze.m_walls[x][y].m_rightWall = true;
        }
    }
}

static size_t performUnion(int identifier, int newIdentifier, std::vector<std::vector<Cell>>& walls) {
    size_t count = 0;
    for (int x = 0; x < walls.size(); x++) {
        for (int y = 0; y < walls[x].size(); y++) {
            if (walls[x][y].identifier == identifier) {
                walls[x][y].identifier = newIdentifier;
            }
            if (walls[x][y].identifier == newIdentifier) {
                count++;
            }
        }
    }
    return count;
}

bool MazeGenerator::step() {

    int x = b::randomInt(0, m_maze.m_width - 1);
    int y = b::randomInt(0, m_maze.m_height - 1);
    auto& cell = m_maze.m_walls[x][y];

    int direction = b::randomInt(0, 3);
    switch (direction) {
    case 0:     // Right neighbor
        if (x < m_maze.m_width - 2) {
            auto& neighbor = m_maze.m_walls[x + 1][y];
            if (cell.identifier == neighbor.identifier) {
                return step();
            }
            cell.m_rightWall = false;
            m_lastUnionedCount = performUnion(neighbor.identifier, cell.identifier, m_maze.m_walls);
            m_lastUnionedIdentifier = cell.identifier;
        }
        break;

    case 1:     // Bottom neighbor
        if (y < m_maze.m_height - 2) {
            auto& neighbor = m_maze.m_walls[x][y + 1];
            if (cell.identifier == neighbor.identifier) {
                return step();
            }
            cell.m_bottomWall = false;
            m_lastUnionedCount = performUnion(neighbor.identifier, cell.identifier, m_maze.m_walls);
            m_lastUnionedIdentifier = cell.identifier;
        }
        break;

    case 2:     // Left neighbor
        if (x > 2) {
            auto& neighbor = m_maze.m_walls[x - 1][y];
            if (cell.identifier == neighbor.identifier) {
                return step();
            }
            neighbor.m_rightWall = false;
            m_lastUnionedCount = performUnion(neighbor.identifier, cell.identifier, m_maze.m_walls);
            m_lastUnionedIdentifier = cell.identifier;
        }
        break;

    case 3:     // Top neighbor
        if (y > 2) {
            auto& neighbor = m_maze.m_walls[x][y - 1];
            if (cell.identifier == neighbor.identifier) {
                return step();
            }
            neighbor.m_bottomWall = false;
            m_lastUnionedCount = performUnion(neighbor.identifier, cell.identifier, m_maze.m_walls);
            m_lastUnionedIdentifier = cell.identifier;
        }
        break;

    default:
        break;
    }

    return m_lastUnionedCount == m_maze.m_width * m_maze.m_height;
}
