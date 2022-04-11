cmake_minimum_required(VERSION 3.16)

set(LIBRARY_NAME Battery)
project(${LIBRARY_NAME} VERSION 0.1.1 LANGUAGES CXX)


##################################
# Options: Library configuration #
##################################

option(DISABLE_LOGGING "Strip all logging commands" OFF)





################
# Source files #
################

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



if (WIN32)
    set_source_files_properties(
        "src/clip/clip_win.cpp" "src/clip/clip.cpp" "src/clip/image.cpp"
        PROPERTIES COMPILE_FLAGS "/wd4018 /wd4267"
    )
else()
    #set_source_files_properties(
    #    "src/clip/clip_win.cpp" "src/clip/clip.cpp" "src/clip/image.cpp"
    #    PROPERTIES COMPILE_FLAGS "..."
    #)
endif()



#############################
# Static library definition #
#############################

add_library(${LIBRARY_NAME} STATIC ${SRC_FILES} ${HEADER_FILES})
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)
set_target_properties(${LIBRARY_NAME} PROPERTIES CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
    target_include_directories(${LIBRARY_NAME} PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/Battery/clip>
        $<INSTALL_INTERFACE:include/Battery/clip>)




######################
# Conan dependencies #
######################

include(${CMAKE_BINARY_DIR}/../conanbuildinfo.cmake)
conan_basic_setup()



############################
# Preprocessor definitions #
############################

if (DISABLE_LOGGING)
    target_compile_definitions(${LIBRARY_NAME} PRIVATE DISABLE_LOGGING=1)
endif()

if (WIN32)
    target_compile_definitions(${LIBRARY_NAME} PRIVATE
       WIN32_LEAN_AND_MEAN      # Prevents Windows.h from adding unnecessary includes
       NOMINMAX                 # Prevents Windows.h from defining min/max as macros 
       _CRT_SECURE_NO_WARNINGS
    )
endif()




#######################
# Precompiled Headers #
#######################

target_precompile_headers(${LIBRARY_NAME} PUBLIC include/Battery/pch.h)



###########
# Linking #
###########

target_link_libraries(${LIBRARY_NAME} PUBLIC ${CONAN_LIBS})




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
    BatteryConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Install binaries
install(
    TARGETS ${LIBRARY_NAME}
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
