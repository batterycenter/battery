#pragma once

#include "pch.hpp"
#include "Maze.hpp"

class MazeGenerator {
public:
    MazeGenerator(Maze& maze);

    bool step();

private:
    Maze& m_maze;
    int m_lastUnionedIdentifier = -1;
    size_t m_lastUnionedCount = 0;
};