#pragma once

#include "pch.hpp"
#include "Maze.hpp"
#include "MazeGenerator.hpp"
#include "MazeSolver.hpp"

inline const ImVec2 GRID_SIZE = ImVec2(150, 150);
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

    sf::Vector2f m_mazeArea;
    sf::Vector2f m_leftUpperMazeCorner;

    b::BatchRenderer m_batchRenderer;
    std::unique_ptr<Maze> m_maze;
    std::unique_ptr<MazeGenerator> m_mazeGenerator;
    std::unique_ptr<MazeSolver> m_mazeSolver;
    MazeState m_state = MazeState::Generating;
};