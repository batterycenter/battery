cmake_minimum_required(VERSION 3.16)
project(battery_cli LANGUAGES CXX)

set(BATTERY_FEATURES_GRAPHICS off CACHE BOOL "" FORCE)
add_subdirectory(.. battery)

battery_add_executable(battery_cli main.cpp)
target_link_libraries(battery_cli Battery)

battery_embed(battery_cli "${CMAKE_CURRENT_LIST_DIR}/../version.txt" "CMAKE_CURRENT_BINARY_DIR/version.txt")