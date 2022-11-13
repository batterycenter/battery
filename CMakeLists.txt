cmake_minimum_required(VERSION 3.16)
project(Battery VERSION 0.1.1 LANGUAGES C CXX)

include(cmake/util.cmake)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

##################################
# Options: Library configuration #
##################################

option(BUILD_EXAMPLES "Build examples" on)
option(BATTERY_STATIC_RUNTIME "Use statically linked runtime" off)
option(BATTERY_SHARED_LIB "Build shared library instead of static" off)

if (MSVC)
    option(BATTERY_USE_PRECOMPILED_HEADER "Using a precompiled header to speed up compile time" on)
else()
    option(BATTERY_USE_PRECOMPILED_HEADER "Using a precompiled header to speed up compile time" off)
endif()


################
# Dependencies #
################

if (NOT WIN32)
    find_package(X11 QUIET)
    if (NOT X11_FOUND)
        message(FATAL_ERROR "X11 development libraries not found! Please install them using your system's equivalent to 'sudo apt install libx11-dev'")
    endif()

    find_package(OpenGL QUIET)
    if (NOT OpenGL_FOUND)
        message(FATAL_ERROR "OpenGL development libraries not found! Please install them using your system's equivalent to 'sudo apt install libgl1-mesa-dev'")
    endif()

    find_package(OpenAL QUIET)
    if (NOT OpenAL_FOUND)
        message(FATAL_ERROR "OpenAL development libraries not found! Please install them using your system's equivalent to 'sudo apt install libopenal-dev'")
    endif()

    include(cmake/FindUDev.cmake)
    if (NOT UDEV_FOUND)
        message(FATAL_ERROR "UDEV development libraries not found! Please install them using your system's equivalent to 'sudo apt install libudev-dev'")
    endif()

    include(cmake/FindVORBIS.cmake)
    if (NOT VORBIS_FOUND)
        message(FATAL_ERROR "VORBIS development libraries not found! Please install them using your system's equivalent to 'sudo apt install libvorbis-dev'")
    endif()

    include(cmake/FindFLAC.cmake)
    if (NOT FLAC_FOUND)
        message(FATAL_ERROR "FLAC development libraries not found! Please install them using your system's equivalent to 'sudo apt install libflac-dev'")
    endif()

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 gtk+-3.0)
    if (NOT GTK3_FOUND)
        message(FATAL_ERROR "GTK3 development libraries not found! Please install them using your system's equivalent to 'sudo apt install libgtk-3-dev'")
    endif()
endif()

get_github_dependency(clip "modules/clip/CMakeLists.txt" "https://github.com/dacap/clip.git" v1.5)
get_github_dependency(glm "modules/glm/CMakeLists.txt" "https://github.com/g-truc/glm.git" 0.9.9.8)
get_github_dependency(imgui "modules/imgui/imgui.cpp" "https://github.com/ocornut/imgui.git" v1.88)
get_github_dependency(imgui-sfml "modules/imgui-sfml/CMakeLists.txt" "https://github.com/eliasdaler/imgui-sfml.git" v2.5)
get_github_dependency(implot "modules/implot/implot.cpp" "https://github.com/epezent/implot.git" v0.14)
get_github_dependency(SFML "modules/SFML/CMakeLists.txt" "https://github.com/SFML/SFML.git" 2.6.x)
get_github_dependency(spdlog "modules/spdlog/CMakeLists.txt" "https://github.com/gabime/spdlog.git" v1.10.0)
get_github_dependency(cpplocate "modules/cpplocate/CMakeLists.txt" "https://github.com/cginternals/cpplocate.git" v2.3.0)
get_github_dependency(nativefiledialog "modules/nativefiledialog/src/include/nfd.h" "https://github.com/mlabbe/nativefiledialog.git" release_116)
get_github_dependency(BatteryUI "modules/BatteryUI/CMakeLists.txt" "https://github.com/HerrNamenlos123/BatteryUI.git" master)

# SFML
set(BUILD_SHARED_LIBS FALSE)
set(SFML_USE_STATIC_STD_LIBS TRUE)
add_subdirectory(modules/SFML)
if (BATTERY_STATIC_RUNTIME)
    use_static_runtime(sfml-graphics)
    use_static_runtime(sfml-main)
    use_static_runtime(sfml-window)
    use_static_runtime(sfml-system)
    use_static_runtime(sfml-audio)
    use_static_runtime(sfml-network)
endif()

set(IMGUI_SFML_FIND_SFML false)
set(IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR}/modules/imgui)
include(cmake/imgui.cmake)
include(cmake/implot.cmake)
include(cmake/nativefiledialog.cmake)
include(cmake/glm.cmake)
add_subdirectory(modules/imgui-sfml)
add_subdirectory(modules/spdlog)
add_subdirectory(modules/clip)
add_subdirectory(modules/cpplocate)
add_subdirectory(modules/BatteryUI)

if (BATTERY_STATIC_RUNTIME)
    use_static_runtime(imgui)
    use_static_runtime(implot)
    use_static_runtime(nativefiledialog)
    use_static_runtime(ImGui-SFML)
    use_static_runtime(spdlog)
    use_static_runtime(clip)
    use_static_runtime(cpplocate)
endif()




#############################
# Static library definition #
#############################

file(GLOB_RECURSE HEADER_FILES "include/*")
file(GLOB_RECURSE SRC_FILES "src/*")

add_library(Battery STATIC ${SRC_FILES} ${HEADER_FILES} "cmake/BatteryConfig.cmake.in")
add_library(Battery::Battery ALIAS Battery)

target_compile_features(Battery PRIVATE cxx_std_17)
set_target_properties(Battery PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(Battery PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

if(BATTERY_STATIC_RUNTIME)
    use_static_runtime(Battery)
endif()

if(MSVC)
    set_source_files_properties(
            "src/serial/serial.cc" "src/serial/impl/win.cc" PROPERTIES COMPILE_FLAGS "/wd4244"
    )
    string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus /MP")
else()
    target_compile_options(clip PRIVATE -Wno-multichar)
    target_compile_options(Battery PRIVATE -Wno-psabi)
endif()

############################
# Preprocessor definitions #
############################

if (WIN32)
    target_compile_definitions(Battery PRIVATE
            WIN32_LEAN_AND_MEAN      # Prevents Windows.h from adding unnecessary includes
            NOMINMAX                 # Prevents Windows.h from defining min/max as macros
            _CRT_SECURE_NO_WARNINGS
            )
    target_compile_definitions(Battery PUBLIC
            UNICODE
            _UNICODE
            )
endif()






###########
# Linking #
###########

if (WIN32)
    target_link_libraries(Battery sfml-main)
endif()
target_link_libraries(Battery sfml-graphics)
target_link_libraries(Battery sfml-audio)
target_link_libraries(Battery sfml-network)
target_link_libraries(Battery sfml-system)
target_link_libraries(Battery sfml-window)

target_link_libraries(Battery glm)
target_link_libraries(Battery imgui)
target_link_libraries(Battery ImGui-SFML)
target_link_libraries(Battery implot)
target_link_libraries(Battery spdlog)
target_link_libraries(Battery clip)
target_link_libraries(Battery nativefiledialog)
target_link_libraries(Battery cpplocate)
target_link_libraries(Battery BatteryUI)

# TODO: clip examples are being built


#######################
# Precompiled Headers #
#######################

if (BATTERY_USE_PRECOMPILED_HEADER)
    target_precompile_headers(Battery PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_LIST_DIR}/include/Battery/pch.hpp>")
endif()





#######
# IDE #
#######

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" PREFIX "Source Files" FILES ${SRC_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/cmake" PREFIX "cmake" FILES "cmake/BatteryConfig.cmake.in")




############
# Examples #
############

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()



###########
# Install #
###########

#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
#    BatteryConfigVersion.cmake
#    VERSION ${PACKAGE_VERSION}
#    COMPATIBILITY AnyNewerVersion
#)
#
## Install binaries
#install(
#    TARGETS Battery glm spdlog clip
#    EXPORT BatteryTargets
#    LIBRARY DESTINATION "lib"
#    ARCHIVE DESTINATION "lib"
#    RUNTIME DESTINATION "bin"
#    INCLUDES DESTINATION "include"
#)
#
## Install headers
#install(
#    DIRECTORY include/
#    DESTINATION "include"
#    FILES_MATCHING PATTERN "*.h*"
#)
#
#install(
#    EXPORT BatteryTargets
#    DESTINATION "lib/cmake/Battery"
#    NAMESPACE Battery::
#)
#
## Install Targets
#configure_file(cmake/BatteryConfig.cmake.in BatteryConfig.cmake @ONLY)
#install(
#    FILES "${CMAKE_CURRENT_BINARY_DIR}/BatteryConfig.cmake"
#          "${CMAKE_CURRENT_BINARY_DIR}/BatteryConfigVersion.cmake"
#    DESTINATION lib/cmake/Battery
#)
#