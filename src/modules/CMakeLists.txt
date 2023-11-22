
# Add all bundled modules
b_clear_available_modules()
#battery_make_module_available(check ${CMAKE_CURRENT_LIST_DIR}/check)
b_make_module_available(cli11 ${CMAKE_CURRENT_LIST_DIR}/cli11)
#battery_make_module_available(eventbus ${CMAKE_CURRENT_LIST_DIR}/eventbus)
#b_make_module_available(graphics ${CMAKE_CURRENT_LIST_DIR}/graphics)
b_make_module_available(imgui ${CMAKE_CURRENT_LIST_DIR}/imgui)
#b_make_module_available(imgui_sfml ${CMAKE_CURRENT_LIST_DIR}/imgui_sfml)
#b_make_module_available(json ${CMAKE_CURRENT_LIST_DIR}/json)
#b_make_module_available(magic_enum ${CMAKE_CURRENT_LIST_DIR}/magic_enum)
b_make_module_available(magnum ${CMAKE_CURRENT_LIST_DIR}/magnum)
#b_make_module_available(network ${CMAKE_CURRENT_LIST_DIR}/network)
#b_make_module_available(pygraphics ${CMAKE_CURRENT_LIST_DIR}/pygraphics)
#b_make_module_available(python ${CMAKE_CURRENT_LIST_DIR}/python)
#b_make_module_available(qrcode ${CMAKE_CURRENT_LIST_DIR}/qrcode)
#b_make_module_available(sfml ${CMAKE_CURRENT_LIST_DIR}/sfml)
#b_make_module_available(toml11 ${CMAKE_CURRENT_LIST_DIR}/toml11)

# This does not immediately add the module, it only makes it available
# Each module's CMakeLists.txt in its subdirectory will only be called
# when the module is added using b_add_module(<module_name>)
