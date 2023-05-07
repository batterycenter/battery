
# Usage of this module:
# battery_add_module(graphics)
# target_link_libraries(... battery::graphics)

battery_add_library(battery_graphics STATIC ALIAS battery::graphics)
battery_add_source_files(battery_graphics
        src/widgets/base_widget.cpp
        src/widgets/button.cpp
        src/widgets/window.cpp

        src/constants.cpp
        src/property_stack.cpp
        src/styles.cpp
        src/unit_property.cpp
        src/widget_style.cpp
        src/window.cpp
        src/windowed_application.cpp
        )

battery_set_ide_folder(battery_graphics "battery/modules")

battery_add_module(sfml)
battery_add_module(imgui)
battery_add_module(imgui_sfml)
battery_add_module(magic_enum)
target_link_libraries(battery_graphics
        battery::sfml::graphics
        battery::sfml::window
        battery::sfml::system
        battery::sfml::audio
        battery::sfml::network
        battery::imgui
        battery::imgui_sfml
        battery::magic_enum
        battery::core
    )

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
