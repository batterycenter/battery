
battery_add_executable(Example_TrayIconAnimated src/main.cpp)
target_link_libraries(Example_TrayIconAnimated battery::core)
target_compile_definitions(Example_TrayIconAnimated PUBLIC EXAMPLE_ROOT="${CMAKE_CURRENT_LIST_DIR}")