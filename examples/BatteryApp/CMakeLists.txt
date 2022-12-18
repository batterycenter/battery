cmake_minimum_required(VERSION 3.16)
project(BatteryApp LANGUAGES CXX)

battery_add_executable(BatteryApp
        src/BatteryApp.cpp
        src/main.cpp
        )

target_include_directories(BatteryApp PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

target_link_libraries(BatteryApp Battery::Battery)