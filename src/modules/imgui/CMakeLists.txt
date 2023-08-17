
# Usage of this module:
# battery_add_module(imgui)
# target_link_libraries(... battery::imgui)

# Pull latest commit from GitHub from May 9, 2923 -> docking branch
battery_add_package("gh:ocornut/imgui#031148dc56d70158b3ad84d9be95b04bb3f5baaf")

set(ENABLE_CLANG_TIDY OFF)  # Disable clang-tidy for this subdirectory: ImGui is a third-party library

# ImGui does not have CMake support
b_add_library(battery_imgui STATIC ALIAS battery::imgui)
target_include_directories(battery_imgui PUBLIC ${imgui_SOURCE_DIR})
b_add_source_files(battery_imgui
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        )
set(IMGUI_DIR ${imgui_SOURCE_DIR} CACHE INTERNAL "" FORCE)      # Needed by imgui-sfml

b_set_target_ide_folder(battery_imgui "battery/modules")