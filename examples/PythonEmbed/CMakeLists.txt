
battery_add_executable(Example_PythonEmbed src/main.cpp)
target_link_libraries(Example_PythonEmbed battery::core battery::python)
battery_embed(Example_PythonEmbed ${CMAKE_CURRENT_LIST_DIR}/resources/script.py TEXT)
battery_copy_python_runtime_files_after_build(Example_PythonEmbed)