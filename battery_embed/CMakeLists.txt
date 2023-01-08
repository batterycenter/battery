cmake_minimum_required(VERSION 3.16)
project(battery_embed LANGUAGES CXX)

add_subdirectory(.. battery)

battery_add_executable(battery_embed battery_embed.cpp)
target_link_libraries(battery_embed battery::core)