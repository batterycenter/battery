#pragma once

#include "pch.hpp"

struct Cell {
    int identifier = 0;
    bool m_bottomWall = false;
    bool m_rightWall = false;
};

struct Maze {
    Maze(int width, int height) {
        m_width = width;
        m_height = height;
        m_walls.resize(width);
        for (int i = 0; i < width; i++) {
            m_walls[i].resize(height);
        }
    }

    int m_width;
    int m_height;
    std::vector<std::vector<Cell>> m_walls;
};