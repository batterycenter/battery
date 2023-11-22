b_add_example(Example_MazeSolver
        src/App.cpp
        src/main.cpp
        src/MazeGenerator.cpp
        src/MazeHost.cpp
        src/MazeSolver.cpp)

#b_add_module(graphics)
#target_link_libraries(Example_MazeSolver PUBLIC battery::graphics)
#b_precompile_headers(Example_MazeSolver include/pch.hpp)