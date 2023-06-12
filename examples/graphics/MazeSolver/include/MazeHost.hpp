#pragma once

#include "pch.hpp"
#include "Maze.hpp"
#include "MazeGenerator.hpp"

inline const ImVec2 GRID_SIZE = ImVec2(10, 10);
inline const int FRAME_SPEEDUP = 1;

class MazeHost {
public:
    MazeHost() = default;

    void init(ImVec2 windowSize);
    void update();
    void render(b::BaseWindow& window);

private:
    std::unique_ptr<Maze> m_maze;
    std::unique_ptr<MazeGenerator> m_mazeGenerator;
};