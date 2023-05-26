
battery_add_executable(Example_FileDialog src/main.cpp)

battery_add_module(filedialog)
target_link_libraries(Example_FileDialog battery::core battery::filedialog)
