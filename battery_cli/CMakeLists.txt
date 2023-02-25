cmake_minimum_required(VERSION 3.16)
project(battery_cli VERSION 0.1.1 LANGUAGES C CXX)

add_subdirectory(.. battery)

file(GLOB_RECURSE HEADER_FILES "include/*")
battery_add_executable(battery_cli
    src/app.cpp
    src/cli_new.cpp
    src/cli_configure.cpp
    src/cli_build.cpp
    src/cli_run.cpp
    src/scripts.cpp
    src/tools.cpp
    src/main.cpp
    ${HEADER_FILES})

battery_set_output_name(battery_cli "battery")
target_include_directories(battery_cli PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(battery_cli battery::core)

battery_precompile_headers(battery_cli "${CMAKE_CURRENT_LIST_DIR}/include/pch.hpp")

battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/../version.txt" TEXT)