battery_add_example(Example_MazeSolver
        src/App.cpp
        src/main.cpp
        src/MazeGenerator.cpp
        src/MazeHost.cpp
        src/MazeSolver.cpp)

battery_add_module(graphics)
target_link_libraries(Example_MazeSolver battery::graphics)
b_precompile_headers(Example_MazeSolver include/pch.hpp)