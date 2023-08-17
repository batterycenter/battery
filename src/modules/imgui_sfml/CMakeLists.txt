
# Usage of this module:
# battery_add_module(imgui_sfml)
# target_link_libraries(... PUBLIC battery::imgui_sfml)

# Download the Dear ImGui User interface library from Github
battery_add_module(imgui)           # This should set IMGUI_DIR
set(IMGUI_INCLUDE_DIR ${IMGUI_DIR})
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
set(IMGUI_SFML_IMGUI_DEMO ON CACHE BOOL "" FORCE)

battery_add_module(sfml)

# Download the SFML ImGui Backend library from Github: Latest commit as of May 6, 2023
battery_add_package("gh:SFML/imgui-sfml#ea890431d2c47e02f84b08d1467a881b88732102")
b_add_alias_library(battery::imgui_sfml ImGui-SFML)

b_set_target_ide_folder(ImGui-SFML "battery/modules")