#pragma once

#include "Maze.hpp"
#include "Branch.hpp"

class MazeSolver {
public:
    MazeSolver(Maze& maze) : m_maze(maze) {}

    void init();
    bool step();

    Branch m_branch;

private:
    Maze& m_maze;
    b::vec2 m_startPosition;
    b::vec2 m_targetPosition;
};
