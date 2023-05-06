
battery_add_executable(Example_CustomMain src/main.cpp)
target_link_libraries(Example_CustomMain battery::core_no_main)
