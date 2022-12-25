cmake_minimum_required(VERSION 3.16)
project(battery_cli LANGUAGES CXX)

set(BATTERY_FEATURES_GRAPHICS off CACHE BOOL "" FORCE)
add_subdirectory(.. battery)

file(GLOB_RECURSE HEADER_FILES "include/*")
battery_add_executable(battery_cli 
    src/cli_new.cpp
    src/cli_configure.cpp
    src/cli_build.cpp
    src/cli_run.cpp
    src/tools.cpp
    src/main.cpp
    ${HEADER_FILES})

set_target_properties(battery_cli PROPERTIES OUTPUT_NAME "battery")     # Call the executable just 'battery' instead of 'battery_cli'
target_include_directories(battery_cli PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(battery_cli Battery)

battery_precompile_headers(battery_cli "${CMAKE_CURRENT_LIST_DIR}/include/pch.hpp")

battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/../version.txt" TEXT)