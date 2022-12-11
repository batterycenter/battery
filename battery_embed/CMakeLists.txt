cmake_minimum_required(VERSION 3.16)
project(battery_embed LANGUAGES CXX)

include(../cmake/util.cmake)
battery_add_executable(battery_embed battery_embed.cpp CLI11.hpp)
target_include_directories(battery_embed PUBLIC ../include)
