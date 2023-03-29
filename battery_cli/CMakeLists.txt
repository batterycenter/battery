cmake_minimum_required(VERSION 3.16)
project(battery_cli VERSION 0.1.1 LANGUAGES C CXX)

set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(.. battery)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/**")
battery_add_executable(battery_cli ALIAS battery::cli ${HEADERS}
	src/app.cpp
	src/console.cpp
	src/main.cpp
	src/Project.cpp
	src/ProjectGenerator.cpp
	src/scripts.cpp)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADERS})

battery_set_output_name(battery_cli "b")
target_include_directories(battery_cli PUBLIC include)
target_link_libraries(battery_cli battery::core)

battery_precompile_headers(battery_cli "${CMAKE_CURRENT_LIST_DIR}/include/pch.hpp")

battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/../version.txt" TEXT)

battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/gitignore.txt" TEXT)
battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/battery.toml" TEXT)
battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/_version.txt" TEXT)
battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/CMakeLists.txt" TEXT)
battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/cmake/CPM.cmake" TEXT)
battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/src/main.cpp" TEXT)
battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/include/pch.hpp" TEXT)