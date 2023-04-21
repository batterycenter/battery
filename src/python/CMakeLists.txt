battery_add_library(battery_python STATIC ALIAS battery::python)
set_target_properties(battery_python PROPERTIES FOLDER "battery")
target_link_libraries(battery_python battery::core)

set(SOURCES
        src/python.cpp)

target_sources(battery_python PRIVATE ${SOURCES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" PREFIX "Source Files" FILES ${SOURCES})

if (MSVC)               # For Visual Studio we add all header files as sources so they're listed in the IDE
    file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/include/**)
    target_sources(battery_python PRIVATE ${HEADERS})
    source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADERS})
endif()

include(cmake/python.cmake)
battery_find_python()
battery_download_python_embedded_distro()

target_link_libraries(battery_python Python::Python pybind11::embed)

# Unit-testing
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
