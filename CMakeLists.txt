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



###################
# Helper function #
###################

function(GET_GITHUB_DEPENDENCY NAME REQUIRED_FILE REPOSITORY_URL BRANCH COMMIT)
if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${REQUIRED_FILE}")
    find_package(Git REQUIRED)
    if(GIT_FOUND)
        message(STATUS "${REQUIRED_FILE} not found, getting dependency '${NAME}'")
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/modules)
        execute_process(COMMAND ${GIT_EXECUTABLE} clone ${REPOSITORY_URL} --single-branch --branch=${BRANCH} --depth=1 && ${GIT_EXECUTABLE} checkout ${COMMIT}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/modules
                        RESULT_VARIABLE GIT_RESULT)
        if(NOT GIT_RESULT EQUAL "0")
            message(FATAL_ERROR "git clone ${REPOSITORY_URL} failed. Error: '${GIT_RESULT}'. Please check the dependencies")
        endif()
    else()
        message(FATAL_ERROR "Cannot get dependencies, git executable not found. Please install Git")
    endif()
endif()
endfunction()





################
# Dependencies #
################

get_github_dependency(clip "modules/clip/CMakeLists.txt" "https://github.com/dacap/clip.git" main 3433235c7faa86d28507558cf07ffa0d933ffd72)
get_github_dependency(glm "modules/glm/CMakeLists.txt" "https://github.com/g-truc/glm.git" master cc98465e3508535ba8c7f6208df934c156a018dc)
get_github_dependency(imgui "modules/imgui/CMakeLists.txt" "https://github.com/ocornut/imgui.git" master cc5058e5d7dcbf94f28277d6bee5face7f8b0a63)
get_github_dependency(imgui-sfml "modules/imgui-sfml/CMakeLists.txt" "https://github.com/eliasdaler/imgui-sfml.git" master 15753d56403a7aeb1cae12fd930cfb1d2eac5e74)
get_github_dependency(implot "modules/implot/CMakeLists.txt" "https://github.com/epezent/implot.git" master 626e391670a4018a365787e35ef4cf01cb68ee31)
get_github_dependency(SFML "modules/SFML/CMakeLists.txt" "https://github.com/SFML/SFML.git" master 2f11710abc5aa478503a7ff3f9e654bd2078ebab)
get_github_dependency(spdlog "modules/spdlog/CMakeLists.txt" "https://github.com/gabime/spdlog.git" v1.10.0 76fb40d95455f249bd70824ecfcae7a8f0930fa3)
get_github_dependency(cpplocate "modules/cpplocate/CMakeLists.txt" "https://github.com/cginternals/cpplocate.git" master 82e574d937a8d6d379359ede413425f7c35f59d1)
get_github_dependency(nativefiledialog "modules/nativefiledialog/CMakeLists.txt" "https://github.com/mlabbe/nativefiledialog.git" master 67345b80ebb429ecc2aeda94c478b3bcc5f7888e)

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
    target_compile_definitions(Battery PUBLIC
       UNICODE
       _UNICODE
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
