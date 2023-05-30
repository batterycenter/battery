cmake_minimum_required(VERSION 3.16)
project(battery_cli VERSION 0.1.1 LANGUAGES C CXX)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/**")
battery_add_executable(battery_cli ALIAS battery::cli ${HEADERS}
	src/console.cpp
	src/main.cpp
	src/Project.cpp
	src/ProjectGenerator.cpp)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADERS})

battery_set_output_name(battery_cli "b")
target_include_directories(battery_cli PUBLIC include)
target_link_libraries(battery_cli battery::core)

battery_add_module(toml11)
target_link_libraries(battery_cli battery::toml11)

battery_precompile_headers(battery_cli "${CMAKE_CURRENT_LIST_DIR}/include/pch.hpp")

battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/../version.txt" "battery_cli/version" TEXT)

battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/gitignore.txt" "battery_cli/templates/gitignore" TEXT)
battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/battery.toml" "battery_cli/templates/battery_toml" TEXT)
battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/_version.txt" "battery_cli/templates/version_txt" TEXT)
battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/CMakeLists.txt" "battery_cli/templates/CMakeLists_txt" TEXT)
battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/cmake/CPM.cmake" "battery_cli/templates/CPM_cmake" TEXT)
battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/src/main.cpp" "battery_cli/templates/main_cpp" TEXT)
battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/resources/templates/include/pch.hpp" "battery_cli/templates/pch_hpp" TEXT)