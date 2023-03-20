cmake_minimum_required(VERSION 3.16)
project(battery_cli VERSION 0.1.1 LANGUAGES C CXX)

set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(.. battery)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/**")
battery_add_executable(battery_cli ALIAS battery::cli ${HEADERS}
	src/app.cpp
	src/cli_build.cpp
	src/cli_configure.cpp
	src/cli_new.cpp
	src/cli_run.cpp
	src/main.cpp
	src/scripts.cpp
	src/tools.cpp)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADERS})

battery_set_output_name(battery_cli "b")
target_include_directories(battery_cli PUBLIC include)
target_link_libraries(battery_cli battery::core)

battery_precompile_headers(battery_cli "${CMAKE_CURRENT_LIST_DIR}/include/pch.hpp")

battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/../version.txt" TEXT)