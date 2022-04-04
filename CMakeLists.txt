cmake_minimum_required(VERSION 3.13)
project(Battery VERSION 0.1.1 LANGUAGES CXX)




# Options
option(DISABLE_LOGGING "Strip all logging commands" OFF)




# Add the Battery library
add_library(Battery "")
target_compile_features(Battery PUBLIC cxx_std_20)
set_target_properties(Battery PROPERTIES CXX_EXTENSIONS OFF)



# Set default build to release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()



# Conan dependencies
include(${CMAKE_BINARY_DIR}/../conanbuildinfo.cmake)
conan_basic_setup()




# Preprocessor
if (DISABLE_LOGGING)
    target_compile_definitions(Battery PRIVATE DISABLE_LOGGING=1)
endif()

if (WIN32)
    target_compile_definitions(Battery PRIVATE
       WIN32_LEAN_AND_MEAN      # Prevents Windows.h from adding unnecessary includes
       NOMINMAX                 # Prevents Windows.h from defining min/max as macros 
    )
endif()




# Include directories
target_include_directories(Battery PUBLIC "include")
target_include_directories(Battery PUBLIC "include/clip")



# Source files
file(GLOB_RECURSE COMMON_SRC "src/Battery/*")
list(APPEND COMMON_SRC "src/clip/clip_none.cpp" "src/clip/clip.cpp")

if (WIN32)
    list(APPEND PLATFORM_SRC "src/clip/clip_win.cpp")
else()
    list(APPEND PLATFORM_SRC "src/clip/clip_x11.cpp")
endif()

target_sources(Battery PUBLIC ${COMMON_SRC} ${PLATFORM_SRC})



# Linking
target_link_libraries(Battery PUBLIC ${CONAN_LIBS})
