
battery_add_executable(Example_GraphicsExample
        src/main.cpp
        src/App.cpp)
target_include_directories(Example_GraphicsExample PRIVATE include)
target_link_libraries(Example_GraphicsExample battery::core battery::graphics)