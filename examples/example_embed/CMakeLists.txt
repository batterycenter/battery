cmake_minimum_required(VERSION 3.16)
project(example_embed LANGUAGES CXX)

set(BATTERY_FEATURES_GRAPHICS off CACHE BOOL "Disable Battery's graphics features" FORCE)

battery_add_executable(example_embed src/main.cpp)
target_link_libraries(example_embed Battery::Battery)

battery_embed(example_embed "resources/message.txt" TEXT)
battery_embed(example_embed "resources/banner.txt" TEXT)
