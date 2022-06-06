
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/imgui-sfml)
add_library(imgui_sfml STATIC)
add_library(imgui_sfml::imgui_sfml ALIAS imgui_sfml)

target_sources(imgui_sfml PRIVATE
        ${IMGUI_DIR}/imgui-SFML.cpp
)

target_include_directories(imgui_sfml PUBLIC
    $<BUILD_INTERFACE:${IMGUI_DIR}/>
    $<INSTALL_INTERFACE:/>
)

target_link_libraries(imgui_sfml SFML::Main)
target_link_libraries(imgui_sfml SFML::Window)
target_link_libraries(imgui_sfml SFML::Graphics)
target_link_libraries(imgui_sfml SFML::System)
target_link_libraries(imgui_sfml imgui::imgui)

install(
    TARGETS imgui_sfml
    EXPORT imgui_sfmlTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)

install(
    EXPORT imgui_sfmlTargets 
    DESTINATION "lib/cmake/imgui_sfml"
    NAMESPACE imgui_sfml::
)

# Install headers
install(
    DIRECTORY "${IMGUI_DIR}/"
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.h"
)

install(
    FILES "cmake/imgui_sfmlConfig.cmake"
    DESTINATION "lib/cmake/imgui_sfml"
)
