cmake_minimum_required(VERSION 3.16)
project(example_embed LANGUAGES CXX)

set(BATTERY_FEATURES_GRAPHICS off CACHE BOOL "" FORCE)       # Use Battery in CLI mode

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../.. Battery)    # Now, include Battery library

battery_add_executable(example_embed    # You must list all files that should be visible in the IDE
    src/main.cpp
    resources/banner.txt
    resources/message.txt
    )           

target_link_libraries(example_embed Battery)

battery_embed(example_embed "resources/message.txt" TEXT)
battery_embed(example_embed "resources/banner.txt" TEXT)
