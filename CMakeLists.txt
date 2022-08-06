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

if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/clip/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/glm/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/imgui/imgui.cpp"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/imgui-sfml/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/implot/implot.cpp"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/SFML/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/spdlog/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/cpplocate/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/nativefiledialog/README.md")
set(GET_SUB true)
endif ()

if (GET_SUB)
find_package(Git)
if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_LIST_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update in ${CMAKE_CURRENT_LIST_DIR}")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()
endif()

if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/clip/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/glm/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/imgui/imgui.cpp"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/imgui-sfml/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/implot/implot.cpp"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/SFML/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/spdlog/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/cpplocate/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/nativefiledialog/README.md")
message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif ()

set(BUILD_SHARED_LIBS FALSE)
set(SFML_USE_STATIC_STD_LIBS TRUE)
add_subdirectory(modules/SFML)

target_compile_options(sfml-graphics PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(sfml-main PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(sfml-window PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(sfml-system PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(sfml-audio PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(sfml-network PRIVATE "/MT$<$<CONFIG:Debug>:d>")

include(cmake/glm.cmake)
include(cmake/imgui.cmake)
include(cmake/imgui-sfml.cmake)
include(cmake/implot.cmake)
include(cmake/spdlog.cmake)
include(cmake/clip.cmake)
include(cmake/nativefiledialog.cmake)
include(cmake/cpplocate.cmake)

target_compile_options(imgui PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(imgui_sfml PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(implot PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(spdlog PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(clip PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(nativefiledialog PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(cpplocate PRIVATE "/MT$<$<CONFIG:Debug>:d>")




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
    set_source_files_properties(
        "src/serial/serial.cc" "src/serial/impl/win.cc" PROPERTIES COMPILE_FLAGS "/wd4244"
    )
    target_compile_options(Battery PRIVATE "/MT$<$<CONFIG:Debug>:d>")
    string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus /MP")
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
target_link_libraries(Battery nativefiledialog::nativefiledialog)
target_link_libraries(Battery cpplocate::cpplocate)




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
