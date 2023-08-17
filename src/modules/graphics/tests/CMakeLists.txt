
b_add_test(battery_graphics_tests)
b_set_target_ide_folder(battery_graphics_tests "battery/tests")

target_link_libraries(battery_graphics_tests PUBLIC battery::graphics)

target_sources(battery_graphics_tests PRIVATE
    main.cpp)
