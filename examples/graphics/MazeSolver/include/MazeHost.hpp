#pragma once

#include "pch.hpp"
#include "Maze.hpp"
#include "MazeGenerator.hpp"
#include "MazeSolver.hpp"

inline const b::vec2 GRID_SIZE = { 150, 150 };
inline const int FRAME_SPEEDUP = 50;

enum class MazeState {
    Generating,
    Solving,
    Finished
};

class MazeHost {
public:
    MazeHost() = default;

    void init(b::BaseWindow* window);
    void update();
    void render();

private:
    void drawMaze();
    void drawCell(int x, int y, sf::Color color);
    void drawBranch(Branch& branch);

private:
    b::BaseWindow* m_window;

    b::vec2 m_mazeArea;
    b::vec2 m_leftUpperMazeCorner;

    b::BatchRenderer m_batchRenderer;
    std::unique_ptr<Maze> m_maze;
    std::unique_ptr<MazeGenerator> m_mazeGenerator;
    std::unique_ptr<MazeSolver> m_mazeSolver;
    MazeState m_state = MazeState::Generating;
};