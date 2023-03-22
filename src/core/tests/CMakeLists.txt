
battery_add_test(battery_core_tests)
target_link_libraries(battery_core_tests battery::core)
target_compile_definitions(battery_core_tests PRIVATE TESTDATA_FOLDER="${CMAKE_CURRENT_LIST_DIR}/testdata")

target_sources(battery_core_tests PRIVATE
    cache.cpp
    filesystem.cpp
    main.cpp
    process.cpp
    string.cpp
    thread.cpp
    time.cpp
    tray_icon.cpp)

set_target_properties(battery_core_tests PROPERTIES FOLDER "battery/tests")
set_target_properties(gtest PROPERTIES FOLDER "battery/tests")