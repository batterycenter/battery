battery_add_example(Example_SimpleGraphics
        src/main.cpp
        src/App.cpp)

battery_add_module(graphics)
target_link_libraries(Example_SimpleGraphics PUBLIC battery::graphics)