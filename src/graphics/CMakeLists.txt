
# Battery core library
battery_add_library(battery_graphics STATIC ALIAS battery::graphics)

set(SOURCES
        src/test.cpp)

target_sources(battery_graphics PRIVATE ${SOURCES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" PREFIX "Source Files" FILES ${SOURCES})

if (MSVC)               # For Visual Studio we add all header files as sources so they're listed in the IDE
    file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/include/**)
    target_sources(battery_graphics PRIVATE ${HEADERS})
    source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADERS})
endif()

# Download dependencies
battery_add_package("gh:SFML/SFML#cbfa9cbb65c0974ade5c52b182e8c57975d4d693")    # SFML Framework: Latest commit (Apr 25, 2023)

# Link dependencies
target_link_libraries(battery_graphics
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
        SFML::Network
        battery::core
    )

# Unit-testing
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

set_target_properties(battery_graphics PROPERTIES FOLDER "battery")
set_target_properties(sfml-audio PROPERTIES FOLDER "battery/deps")
set_target_properties(sfml-graphics PROPERTIES FOLDER "battery/deps")
set_target_properties(sfml-network PROPERTIES FOLDER "battery/deps")
set_target_properties(sfml-system PROPERTIES FOLDER "battery/deps")
set_target_properties(sfml-window PROPERTIES FOLDER "battery/deps")
