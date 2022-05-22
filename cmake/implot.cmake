
set(IMPLOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/implot)
add_library(implot STATIC)

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
