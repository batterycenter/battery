cmake_minimum_required(VERSION 3.16)
project(battery_embed LANGUAGES CXX)

set(BATTERY_FEATURES_GRAPHICS off CACHE BOOL "Disable Battery's graphics features" FORCE)
set(BATTERY_BUILD_EXAMPLES off CACHE BOOL "Don't build battery examples" FORCE) # TODO: Remove this when battery examples are finished
add_subdirectory(.. battery_embed-battery)

battery_add_executable(battery_embed battery_embed.cpp)
target_link_libraries(battery_embed Battery::Battery)