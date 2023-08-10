
battery_add_test(battery_core_tests)
battery_set_target_ide_folder(battery_core_tests "battery/tests")

target_link_libraries(battery_core_tests battery::core)
target_compile_definitions(battery_core_tests PRIVATE TESTDATA_FOLDER="${CMAKE_CURRENT_LIST_DIR}/testdata")

target_sources(battery_core_tests PRIVATE
    application.cpp
    base64.cpp
    bytearray.cpp
    cache.cpp
    filesystem.cpp
    lockfile.cpp
    main.cpp
    process.cpp
    string.cpp
    thread.cpp
    time.cpp
)
