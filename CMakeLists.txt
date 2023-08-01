cmake_minimum_required(VERSION 3.21)
project(battery VERSION 0.1.2)
enable_testing()

set(CMAKE_CXX_STANDARD 20)  # Default C++ version for non-specified targets and submodules
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT ${B_FIRST_CMAKE_RUN_COMPLETE})       # Message is just for user experience
    message("BATTERY: CMake is called for the first time, dependencies are automatically downloaded. This may take a few minutes...")
endif()

#option(ENABLE_CLANG_TIDY "Enable clang-tidy best-practices checks" ON)

include(cmake/tools.cmake)

option(BUILD_SHARED_LIBS "Build shared libraries instead of static ones" OFF)
option(BUILD_TESTS "Build all unit tests" ${B_IS_ROOT_PROJECT})
option(BUILD_EXAMPLES "Build examples" ${B_IS_ROOT_PROJECT})
option(BUILD_DOCS "Define the targets for building the documentation (not built automatically)" ${B_IS_ROOT_PROJECT})
option(B_PRODUCTION_MODE "Build the project in production mode" OFF)

add_subdirectory(deps)
add_subdirectory(src)

# add_subdirectory(battery_cli)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if (BUILD_DOCS)
    add_subdirectory(docs)
endif()

set(B_FIRST_CMAKE_RUN_COMPLETE TRUE CACHE INTERNAL "A flag showing that CMake has configured at least once.")
