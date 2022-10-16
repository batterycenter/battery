
set(IMGUI_DIR_ $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/modules/imgui/>$<INSTALL_INTERFACE:>)
add_library(imgui STATIC)
add_library(imgui::imgui ALIAS imgui)

target_sources(imgui PRIVATE
        ${IMGUI_DIR_}/imgui_demo.cpp
        ${IMGUI_DIR_}/imgui_draw.cpp
        ${IMGUI_DIR_}/imgui_tables.cpp
        ${IMGUI_DIR_}/imgui_widgets.cpp
        ${IMGUI_DIR_}/imgui.cpp
)

target_include_directories(imgui PUBLIC
    $<BUILD_INTERFACE:${IMGUI_DIR_}/>
    $<INSTALL_INTERFACE:/>
)

install(
    TARGETS imgui
    EXPORT imguiTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)

install(
    EXPORT imguiTargets 
    DESTINATION "lib/cmake/imgui"
    NAMESPACE imgui::
)

# Install headers
install(
    DIRECTORY "${IMGUI_DIR_}/"
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.h"
)

install(
    FILES "cmake/imguiConfig.cmake"
    DESTINATION "lib/cmake/imgui"
)
