
# Usage of this module:
# battery_add_module(graphics)
# target_link_libraries(... battery::graphics)

battery_add_library(battery_graphics STATIC ALIAS battery::graphics)
battery_add_source_files(battery_graphics
        src/graphics_application.cpp)

battery_set_ide_folder(battery_graphics "battery/modules")

battery_add_module(sfml)
battery_add_module(imgui)
battery_add_module(imgui_sfml)
target_link_libraries(battery_graphics
        battery::sfml::graphics
        battery::sfml::window
        battery::sfml::system
        battery::sfml::audio
        battery::sfml::network
        battery::imgui
        battery::imgui_sfml
        battery::core
    )

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
