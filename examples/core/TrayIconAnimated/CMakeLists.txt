battery_add_example(Example_TrayIconAnimated src/main.cpp)
battery_add_module(tray)
target_link_libraries(Example_TrayIconAnimated battery::tray)
target_compile_definitions(Example_TrayIconAnimated PUBLIC EXAMPLE_ROOT="${CMAKE_CURRENT_LIST_DIR}")