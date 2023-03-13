cmake_minimum_required(VERSION 3.16)
project(battery VERSION 0.1.1)

#set(BATTERY_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} PARENT_SCOPE)
set(BATTERY_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

include(cmake/tools.cmake)
battery_check_required_compiler_features()

##################################
# Options: Library configuration #
##################################

# Options
option(BUILD_TESTS "Build all unit tests" ON)

enable_testing()

add_subdirectory(src/core)

#option(BATTERY_BUILD_EXAMPLES "Build examples" on)
#option(BATTERY_SHARED_LIB "Build shared library instead of static" off)
#
#if (MSVC)
#    option(BATTERY_USE_PRECOMPILED_HEADER "Using a precompiled header to speed up compile time" on)
#else()
#    option(BATTERY_USE_PRECOMPILED_HEADER "Using a precompiled header to speed up compile time" off)
#endif()


################
# Dependencies #
################

#battery_set_cache_variable(BUILD_SHARED_LIBS off)
#
#if (NOT WIN32 AND BATTERY_FEATURES_GRAPHICS)
#    find_package(PkgConfig REQUIRED)
#
#    battery_require_find_package(X11 X11_FOUND "Please install them using your system's equivalent to 'sudo apt install libx11-dev'")
#    battery_require_find_package(OpenGL OpenGL_FOUND "Please install them using your system's equivalent to 'sudo apt install libgl1-mesa-dev'")
#    battery_require_find_package(OpenAL OpenAL_FOUND "Please install them using your system's equivalent to 'sudo apt install libopenal-dev'")
#
#    include(cmake/FindUDev.cmake)
#    if (NOT UDEV_FOUND)
#        message(FATAL_ERROR "UDEV development libraries not found! Please install them using your system's equivalent to 'sudo apt install libudev-dev'")
#    endif()
#
#    include(cmake/FindVORBIS.cmake)
#    if (NOT VORBIS_FOUND)
#        message(FATAL_ERROR "VORBIS development libraries not found! Please install them using your system's equivalent to 'sudo apt install libvorbis-dev'")
#    endif()
#
#    include(cmake/FindFLAC.cmake)
#    if (NOT FLAC_FOUND)
#        message(FATAL_ERROR "FLAC development libraries not found! Please install them using your system's equivalent to 'sudo apt install libflac-dev'")
#    endif()
#
#    pkg_check_modules(GTK3 gtk+-3.0)
#    if (NOT GTK3_FOUND)
#        message(FATAL_ERROR "GTK3 development libraries not found! Please install them using your system's equivalent to 'sudo apt install libgtk-3-dev'")
#    endif()
#
#    pkg_check_modules(LIBAPPINDICATOR appindicator3-0.1)
#    if (NOT LIBAPPINDICATOR_FOUND)
#        message(FATAL_ERROR "libappindicator development libraries not found! Please install them using your system's equivalent to 'sudo apt install libappindicator3-dev'")
#    endif()
#endif()

#if (BATTERY_FEATURES_GRAPHICS)
#    battery_add_dependency(imgui)
#    battery_declare_dependency(SFML)
#    find_package(SFML COMPONENTS graphics CONFIG REQUIRED)
#
#    get_github_dependency(imgui-sfml "modules/imgui-sfml/CMakeLists.txt" "https://github.com/eliasdaler/imgui-sfml.git" v2.5)
#    get_github_dependency(implot "modules/implot/implot.cpp" "https://github.com/epezent/implot.git" v0.14)
#endif()

#get_github_dependency(clip "modules/clip/CMakeLists.txt" "https://github.com/dacap/clip.git" v1.5)
#get_github_dependency(toml11 "modules/toml11/CMakeLists.txt" "https://github.com/ToruNiina/toml11.git" v3.7.1)
#get_github_dependency(glm "modules/glm/CMakeLists.txt" "https://github.com/g-truc/glm.git" 0.9.9.8)
#get_github_dependency(cpplocate "modules/cpplocate/CMakeLists.txt" "https://github.com/cginternals/cpplocate.git" v2.3.0)
#get_github_dependency(nativefiledialog "modules/nativefiledialog/src/include/nfd.h" "https://github.com/mlabbe/nativefiledialog.git" release_116)
#get_github_dependency(BatteryUI "modules/BatteryUI/CMakeLists.txt" "https://github.com/HerrNamenlos123/BatteryUI.git" master)

# SFML and other graphics features
#if (BATTERY_FEATURES_GRAPHICS)
#    battery_set_cache_variable(IMGUI_SFML_FIND_SFML off)
#    set(IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR}/modules/imgui)
#    add_subdirectory(modules/imgui-sfml)
#    include(cmake/implot.cmake)
#endif()

#include(cmake/nativefiledialog.cmake)
#include(cmake/glm.cmake)
##add_subdirectory(modules/spdlog)
#battery_set_cache_variable(CLIP_EXAMPLES off)
#battery_set_cache_variable(CLIP_TESTS off)
#add_subdirectory(modules/clip)
#add_subdirectory(modules/toml11)
#battery_set_cache_variable(OPTION_BUILD_TESTS off)
##add_subdirectory(modules/cpplocate)
#add_subdirectory(modules/BatteryUI)




#############################
# Static library definition #
#############################

#if(MSVC)
#    set_source_files_properties(
#            "src/serial/serial.cc" "src/serial/impl/win.cc" PROPERTIES COMPILE_FLAGS "/wd4244"
#    )
#    set_source_files_properties(
#            "modules/cpplocate/source/liblocate/source/utils.c"
#            "modules/cpplocate/source/liblocate/source/liblocate.c"
#            PROPERTIES COMPILE_FLAGS "/wd5105"
#    )
#endif()






###########
# Linking #
###########

#if (BATTERY_FEATURES_GRAPHICS)
#    if (WIN32)
#        target_link_libraries(Battery sfml-main)
#    endif()
#    target_link_libraries(Battery sfml-graphics)
#    target_link_libraries(Battery sfml-audio)
#    target_link_libraries(Battery sfml-network)
#    target_link_libraries(Battery sfml-system)
#    target_link_libraries(Battery sfml-window)
#
#    target_link_libraries(Battery imgui)
#    target_link_libraries(Battery ImGui-SFML)
#    target_link_libraries(Battery implot)
#endif()
#
#if (NOT WIN32)
#    if (BATTERY_FEATURES_GRAPHICS)
#        target_include_directories(Battery PUBLIC ${GTK3_INCLUDE_DIRS})
#        target_link_directories(Battery PRIVATE ${GTK3_LIBRARY_DIRS})
#        add_definitions(${GTK3_CFLAGS_OTHER})
#        target_link_libraries(Battery ${GTK3_LIBRARIES})
#    endif()
#    target_link_libraries(Battery ${LIBAPPINDICATOR_LIBRARIES})
#    target_include_directories(Battery PUBLIC ${LIBAPPINDICATOR_INCLUDE_DIRS})
#    target_link_directories(Battery PUBLIC ${LIBAPPINDICATOR_LIBRARY_DIRS})
#endif()
#
#target_link_libraries(Battery glm)
#target_link_libraries(Battery toml11)
#target_link_libraries(Battery spdlog)
#target_link_libraries(Battery clip)
#target_link_libraries(Battery nativefiledialog)
#target_link_libraries(Battery cpplocate)
#target_link_libraries(Battery BatteryUI)


#######################
# Precompiled Headers #
#######################

#if (BATTERY_USE_PRECOMPILED_HEADER)
#    target_precompile_headers(Battery PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_LIST_DIR}/include/Battery/pch.hpp>")
#endif()





#######
# IDE #
#######

#set_property(TARGET Battery PROPERTY FOLDER "Battery")

#set_property(TARGET cpplocate PROPERTY FOLDER "Battery/deps/cpplocate")
#set_property(TARGET component_install PROPERTY FOLDER "Battery/deps/cpplocate")
#set_property(TARGET liblocate PROPERTY FOLDER "Battery/deps/cpplocate")
#set_property(TARGET pack PROPERTY FOLDER "Battery/deps/cpplocate")
#set_property(TARGET pack-cpplocate PROPERTY FOLDER "Battery/deps/cpplocate")
#set_property(TARGET check-all PROPERTY FOLDER "Battery/deps/cpplocate")
#set_property(TARGET check-cpplocate PROPERTY FOLDER "Battery/deps/cpplocate")
#set_property(TARGET check-liblocate PROPERTY FOLDER "Battery/deps/cpplocate")
#set_property(TARGET check-template PROPERTY FOLDER "Battery/deps/cpplocate")

#set_property(TARGET spdlog PROPERTY FOLDER "Battery/deps")
#set_property(TARGET nativefiledialog PROPERTY FOLDER "Battery/deps")
#set_property(TARGET clip PROPERTY FOLDER "Battery/deps")
