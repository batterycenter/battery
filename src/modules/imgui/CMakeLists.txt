
# Usage of this module:
# battery_add_module(imgui)
# target_link_libraries(... PUBLIC battery::imgui)

set(ENABLE_CLANG_TIDY OFF)  # Disable clang-tidy for this subdirectory: ImGui is a third-party library

# ImGui does not have CMake support
b_add_library(battery_imgui STATIC ALIAS battery::imgui)
target_include_directories(battery_imgui PUBLIC ${B_ROOT_DIR}/deps/imgui)
b_add_source_files(battery_imgui
        ${B_ROOT_DIR}/deps/imgui/imgui.cpp
        ${B_ROOT_DIR}/deps/imgui/imgui_demo.cpp
        ${B_ROOT_DIR}/deps/imgui/imgui_draw.cpp
        ${B_ROOT_DIR}/deps/imgui/imgui_widgets.cpp
        ${B_ROOT_DIR}/deps/imgui/imgui_tables.cpp
)

b_set_target_ide_folder(battery_imgui "battery/modules")