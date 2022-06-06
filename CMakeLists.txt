cmake_minimum_required(VERSION 3.16)

project(Battery VERSION 0.1.1 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

##################################
# Options: Library configuration #
##################################





################
# Dependencies #
################

set(BUILD_SHARED_LIBS FALSE)
set(SFML_USE_STATIC_STD_LIBS TRUE)
add_subdirectory(modules/SFML)

set_property(TARGET sfml-graphics PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET sfml-window PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET sfml-main PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET sfml-system PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include(cmake/glm.cmake)
include(cmake/imgui.cmake)
include(cmake/imgui-sfml.cmake)
include(cmake/implot.cmake)
include(cmake/spdlog.cmake)
include(cmake/clip.cmake)

set_property(TARGET imgui PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET imgui_sfml PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET implot PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET spdlog PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET clip PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")




#############################
# Static library definition #
#############################

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "include/*")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*")

add_library(Battery STATIC ${SRC_FILES} ${HEADER_FILES} "cmake/BatteryConfig.cmake.in")
add_library(Battery::Battery ALIAS Battery)

target_compile_features(Battery PRIVATE cxx_std_17)
set_target_properties(Battery PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(Battery PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


if(MSVC)
    set_property(TARGET Battery PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
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
endif()






###########
# Linking #
###########

target_link_libraries(Battery SFML::Main)
target_link_libraries(Battery SFML::Graphics)
target_link_libraries(Battery SFML::Audio)
target_link_libraries(Battery SFML::Network)
target_link_libraries(Battery SFML::System)
target_link_libraries(Battery glm::glm)
target_link_libraries(Battery imgui::imgui)
target_link_libraries(Battery imgui_sfml::imgui_sfml)
target_link_libraries(Battery implot::implot)
target_link_libraries(Battery spdlog::spdlog)
target_link_libraries(Battery clip::clip)




#######################
# Precompiled Headers #
#######################

target_precompile_headers(Battery PRIVATE include/Battery/pch.h)





#######
# IDE #
#######

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" PREFIX "Source Files" FILES ${SRC_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/cmake" PREFIX "cmake" FILES "cmake/BatteryConfig.cmake.in")



###########
# Install #
###########

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    BatteryConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Install binaries
install(
    TARGETS Battery
    EXPORT BatteryTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)

# Install headers
install(
    DIRECTORY include/
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.h*"
)

install(
    EXPORT BatteryTargets 
    DESTINATION "lib/cmake/Battery"
    NAMESPACE Battery::
)

# Install Targets
configure_file(cmake/BatteryConfig.cmake.in BatteryConfig.cmake @ONLY)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/BatteryConfig.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/BatteryConfigVersion.cmake"
    DESTINATION lib/cmake/Battery
)
