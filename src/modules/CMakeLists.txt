
# Add all bundled modules
battery_clear_available_modules()
battery_make_module_available(eventbus ${CMAKE_CURRENT_LIST_DIR}/eventbus)
battery_make_module_available(graphics ${CMAKE_CURRENT_LIST_DIR}/graphics)
battery_make_module_available(python ${CMAKE_CURRENT_LIST_DIR}/python)
battery_make_module_available(reproc++ ${CMAKE_CURRENT_LIST_DIR}/reproc++)
battery_make_module_available(sfml ${CMAKE_CURRENT_LIST_DIR}/sfml)
battery_make_module_available(spdlog ${CMAKE_CURRENT_LIST_DIR}/spdlog)
battery_make_module_available(toml11 ${CMAKE_CURRENT_LIST_DIR}/toml11)
battery_make_module_available(tray ${CMAKE_CURRENT_LIST_DIR}/tray)
battery_make_module_available(utfcpp ${CMAKE_CURRENT_LIST_DIR}/utfcpp)

# This does not immediately add the module, it only makes it available
# CMakeLists.txt in the module directory will only be processed when the module is added
