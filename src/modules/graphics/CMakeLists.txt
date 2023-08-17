
# Usage of this module:
# battery_add_module(graphics)
# target_link_libraries(... battery::graphics)

b_add_library(battery_graphics STATIC ALIAS battery::graphics)
b_add_source_files(battery_graphics
        src/widgets/base_widget.cpp
        src/widgets/button.cpp
        src/widgets/button_ex.cpp
        src/widgets/canvas_host.cpp
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
        src/batch_renderer.cpp
        src/canvas.cpp
        src/color_hex.cpp
        src/font_stack.cpp
        src/mouse.cpp
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

b_set_ide_folder(battery_graphics "battery/modules")

battery_add_module(sfml)
battery_add_module(imgui)
battery_add_module(imgui_sfml)
battery_add_module(magic_enum)
battery_add_module(json)
battery_add_module(eventbus)
battery_add_module(python)

target_link_libraries(battery_graphics
        battery::core
        battery::sfml::graphics
        battery::sfml::window
        battery::sfml::system
        # battery::sfml::audio
        battery::sfml::network
        battery::imgui
        battery::imgui_sfml
        battery::magic_enum
        battery::json
        battery::eventbus
        battery::python
    )

target_compile_definitions(battery_graphics PUBLIC B_VEC_INTEGRATE_SFML)     # Define b::Vec conversion operators for SFML types
target_compile_definitions(battery_graphics PUBLIC B_VEC_INTEGRATE_IMGUI)    # Define b::Vec conversion operators for ImGui types
target_compile_definitions(battery_graphics PUBLIC B_COLOR_INTEGRATE_SFML)   # Define b::Color conversion operators for SFML types
target_compile_definitions(battery_graphics PUBLIC B_COLOR_INTEGRATE_IMGUI)  # Define b::Color conversion operators for ImGui types
target_compile_definitions(battery_graphics PUBLIC B_COLOR_INTEGRATE_NLOHMANN_JSON)  # Define b::Color json serialization

battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/default_themes.json DefaultThemesJson TEXT)

battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/inter-regular.ttf FontsInterRegular_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/inter-medium.ttf FontsInterMedium_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/inter-bold.ttf FontsInterBold_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/roboto-regular.ttf FontsRobotoRegular_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/roboto-medium.ttf FontsRobotoMedium_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/roboto-bold.ttf FontsRobotoBold_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/montserrat-regular.ttf FontsMontserratRegular_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/montserrat-medium.ttf FontsMontserratMedium_ttf BINARY)
battery_embed(battery_graphics ${CMAKE_CURRENT_LIST_DIR}/resources/fonts/montserrat-bold.ttf FontsMontserratBold_ttf BINARY)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
