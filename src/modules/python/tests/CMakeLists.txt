
b_add_test(battery_python_tests)
target_link_libraries(battery_python_tests battery::core battery::python)
target_compile_definitions(battery_python_tests PRIVATE TESTDATA_FOLDER="${CMAKE_CURRENT_LIST_DIR}/testdata")

target_sources(battery_python_tests PRIVATE
    python.cpp
    main.cpp)

b_set_target_ide_folder(battery_python_tests "battery/tests")
battery_copy_python_runtime_files_after_build(battery_python_tests)