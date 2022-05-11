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

find_package(allegro5 REQUIRED)
find_package(glm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(imgui REQUIRED)
find_package(implot REQUIRED)
find_package(spdlog REQUIRED)
find_package(asio REQUIRED)





#############################
# Static library definition #
#############################

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "include/*")
file(GLOB_RECURSE COMMON_SRC CONFIGURE_DEPENDS "src/Battery/*")
list(APPEND COMMON_SRC "src/clip/clip.cpp" "src/clip/image.cpp")

file(GLOB_RECURSE REMOVE_WIN CONFIGURE_DEPENDS "src/Battery/Platform/win/*")
file(GLOB_RECURSE REMOVE_X11 CONFIGURE_DEPENDS "src/Battery/Platform/x11/*")

if (WIN32)
    list(APPEND PLATFORM_SRC "src/clip/clip_win.cpp")
    list(REMOVE_ITEM COMMON_SRC ${REMOVE_X11})
else()
    list(APPEND PLATFORM_SRC "src/clip/clip_x11.cpp")
    list(REMOVE_ITEM COMMON_SRC ${REMOVE_WIN})
endif()

list(APPEND SRC_FILES ${COMMON_SRC} ${PLATFORM_SRC})

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HEADER_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/Battery/clip>
    $<INSTALL_INTERFACE:include/Battery/clip>
)



if(MSVC)
    set_source_files_properties(
        "src/clip/clip_win.cpp" "src/clip/clip.cpp" "src/clip/image.cpp"
        PROPERTIES COMPILE_FLAGS "/wd4018 /wd4267"
    )
    set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-psabi)
endif()



############################
# Preprocessor definitions #
############################

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
       WIN32_LEAN_AND_MEAN      # Prevents Windows.h from adding unnecessary includes
       NOMINMAX                 # Prevents Windows.h from defining min/max as macros 
       _CRT_SECURE_NO_WARNINGS
    )
endif()






###########
# Linking #
###########

target_link_libraries(${PROJECT_NAME} allegro5::allegro5)
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} imgui::imgui)
target_link_libraries(${PROJECT_NAME} implot::implot)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} asio::asio)





#######
# IDE #
#######

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SRC_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_FILES})



###########
# Install #
###########

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Install binaries
install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)

# Install symbols
install(
    DIRECTORY build/
    DESTINATION "lib"
    FILES_MATCHING PATTERN "*.pdb"
)

# Install headers
install(
    DIRECTORY include/
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.h*"
)

install(
    EXPORT ${PROJECT_NAME}Targets 
    DESTINATION "lib/cmake/${PROJECT_NAME}"
    NAMESPACE ${PROJECT_NAME}::
)

# Install Targets
configure_file(cmake/${PROJECT_NAME}Config.cmake.in ${PROJECT_NAME}Config.cmake @ONLY)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# Install any conan dependencies
file(GLOB CONAN_DEPS "${CMAKE_CURRENT_BINARY_DIR}/*.cmake")
install(
    FILES ${CONAN_DEPS}
    DESTINATION lib/cmake/${PROJECT_NAME}
)
