
set(IMPLOT_DIR $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/modules/implot/>$<INSTALL_INTERFACE:>)
add_library(implot STATIC)
add_library(implot::implot ALIAS implot)

target_sources(implot PRIVATE
        ${IMPLOT_DIR}/implot_demo.cpp
        ${IMPLOT_DIR}/implot_items.cpp
        ${IMPLOT_DIR}/implot.cpp
)

target_include_directories(implot PUBLIC
    $<BUILD_INTERFACE:${IMPLOT_DIR}/>
    $<INSTALL_INTERFACE:/>
)

target_link_libraries(implot PRIVATE imgui)

install(
    TARGETS implot
    EXPORT implotTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)

install(
    EXPORT implotTargets 
    DESTINATION "lib/cmake/implot"
    NAMESPACE implot::
)

# Install headers
install(
    DIRECTORY "${IMPLOT_DIR}/"
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.h"
)

install(
    FILES "cmake/implotConfig.cmake"
    DESTINATION "lib/cmake/implot"
)
