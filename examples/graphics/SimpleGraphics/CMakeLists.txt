b_add_example(Example_SimpleGraphics
        src/main.cpp
        src/App.cpp)

#b_add_module(graphics)
target_link_libraries(Example_SimpleGraphics PUBLIC battery::core)
b_embed(Example_SimpleGraphics ui/main.lua)