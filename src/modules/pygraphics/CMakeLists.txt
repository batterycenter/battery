
# Usage of this module:
# battery_add_module(pygraphics)
# target_link_libraries(... PUBLIC battery::pygraphics)

b_add_library(battery_pygraphics STATIC ALIAS battery::pygraphics)
b_add_source_files(battery_pygraphics
        src/window.cpp
        )

b_set_ide_folder(battery_pygraphics "battery/modules")

battery_add_module(python)
battery_add_module(graphics)

target_link_libraries(battery_graphics PUBLIC 
        battery::core
        battery::python
        battery::graphics
    )

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
