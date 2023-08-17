cmake_minimum_required(VERSION 3.16)
project({{project_name}} LANGUAGES C CXX)

# Add the CPM.cmake package manager
include(cmake/CPM.cmake)

# Download the battery library
CPMAddPackage("gh:batterycenter/battery#master")

# Define the executable and all the source files
b_add_executable(${PROJECT_NAME}
        src/main.cpp
)

# Include directory
target_include_directories(${PROJECT_NAME} PUBLIC include)

# What libraries to link to
target_link_libraries(${PROJECT_NAME} PUBLIC battery::core)

# Enable precompiled headers for faster compilation
b_precompile_headers(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/include/pch.hpp")

# What project to start Visual Studio with
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Embed any resource files at build time (including hot-reload support)
# battery_embed(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/resources/test.txt" TEXT)