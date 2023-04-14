
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

# Default minimum Python version, meant to be overridden by the user
if (NOT DEFINED PYTHON_MINIMUM_VERSION)
    set(PYTHON_MINIMUM_VERSION "3.8.0")
endif()

# Find Python
find_package(Python COMPONENTS Interpreter Development.Embed)
if (NOT Python_FOUND)
    message(FATAL_ERROR "Python not found. Please install it, or set Python_ROOT_DIR to the install location. Otherwise, refer to https://cmake.org/cmake/help/latest/module/FindPython.html")
endif()

# Check Python version
if (Python_VERSION VERSION_LESS PYTHON_MINIMUM_VERSION)
    message(FATAL_ERROR "Installed Python version is ${Python_VERSION}, while at least ${PYTHON_MINIMUM_VERSION} is required to build this project")
endif()

# Query Python architecture
execute_process(
    COMMAND ${Python_EXECUTABLE} -c "import sys; print('64' if sys.maxsize > 2**32 else '32')"
    OUTPUT_VARIABLE PYTHON_ARCH
    RESULT_VARIABLE PYTHON_ARCH_RESULT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if (NOT PYTHON_ARCH_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to determine Python architecture")
endif()
message(STATUS "Building for Python ${Python_VERSION} (${PYTHON_ARCH}-bit)")

# Download python bindings library
battery_add_package("gh:pybind/pybind11#v2.10.4")
target_link_libraries(battery_python Python::Python pybind11::embed)

# Download python embedded distribution
if (PYTHON_ARCH EQUAL 32)
    set(PYTHON_EMBED_URL "https://www.python.org/ftp/python/${Python_VERSION}/python-${Python_VERSION}-embed-win32.zip")
else()
    set(PYTHON_EMBED_URL "https://www.python.org/ftp/python/${Python_VERSION}/python-${Python_VERSION}-embed-amd64.zip")
endif()
battery_add_package(
    NAME battery_python_embed
    URL ${PYTHON_EMBED_URL}
)

# Copy runtime files to output directory after build
set(BATTERY_PYTHON_EMBED_SOURCE_DIR ${battery_python_embed_SOURCE_DIR} PARENT_SCOPE)
set(BATTERY_PYTHON_EMBED_SOURCE_DIR ${battery_python_embed_SOURCE_DIR})
function(battery_copy_python_runtime_files_after_build TARGET)
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${BATTERY_PYTHON_EMBED_SOURCE_DIR}" ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/${TARGET}
    )
endfunction()

# Unit-testing
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
