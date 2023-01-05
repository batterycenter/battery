cmake_minimum_required(VERSION 3.16)
project(battery_embed LANGUAGES CXX)

set(BATTERY_FEATURES_GRAPHICS off CACHE BOOL "" FORCE)
add_subdirectory(.. battery)

battery_add_executable(battery_embed battery_embed.cpp)
target_link_libraries(battery_embed Battery)