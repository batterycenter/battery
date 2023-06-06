
# Usage of this module:
# battery_add_module(graphics)
# target_link_libraries(... battery::graphics)

battery_add_library(battery_graphics STATIC ALIAS battery::graphics)
battery_add_source_files(battery_graphics
        src/widgets/base_widget.cpp
        src/widgets/button.cpp
        src/widgets/checkbox.cpp
        src/widgets/checkbox_ex.cpp
        src/widgets/container.cpp
        src/widgets/dummy.cpp
        src/widgets/button_ex.cpp
        src/widgets/grid.cpp
        src/widgets/image.cpp
        src/widgets/input.cpp
        src/widgets/rect.cpp
        src/widgets/text.cpp
        src/widgets/panel.cpp

        src/application.cpp
        src/color_hex.cpp
        src/font_stack.cpp
        src/primitive_render_window.cpp
        src/constants.cpp
        src/property_pack.cpp
        src/property_stack.cpp
        src/sfml.cpp
        src/styles.cpp
        src/unit_property.cpp
        src/widget_builder.cpp
        src/window.cpp
        )

battery_set_ide_folder(battery_graphics "battery/modules")

battery_add_module(sfml)
battery_add_module(imgui)
battery_add_module(imgui_sfml)
battery_add_module(magic_enum)
battery_add_module(json)
battery_add_module(python)
battery_add_module(eventbus)

target_link_libraries(battery_graphics
        battery::core
        battery::sfml::graphics
        battery::sfml::window
        battery::sfml::system
        battery::sfml::audio
        battery::sfml::network
        battery::imgui
        battery::imgui_sfml
        battery::magic_enum
        battery::json
        battery::python
        battery::eventbus
    )

battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/default_themes.json default_themes_json TEXT)

battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/inter-regular.ttf fonts_inter_regular_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/inter-medium.ttf fonts_inter_medium_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/inter-bold.ttf fonts_inter_bold_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/roboto-regular.ttf fonts_roboto_regular_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/roboto-medium.ttf fonts_roboto_medium_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/roboto-bold.ttf fonts_roboto_bold_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/montserrat-regular.ttf fonts_montserrat_regular_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/montserrat-medium.ttf fonts_montserrat_medium_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/montserrat-bold.ttf fonts_montserrat_bold_ttf BINARY)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
