
battery_add_executable(Example_SimpleGraphics
        src/main.cpp
        src/App.cpp)

battery_add_module(graphics)
target_link_libraries(Example_SimpleGraphics battery::core battery::graphics)