cmake_minimum_required(VERSION 3.16)

project(BatteryApp VERSION 0.1.1 LANGUAGES CXX)

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

add_subdirectory(../.. "${CMAKE_CURRENT_BINARY_DIR}/../../build") # Add Battery library




#############################
# Static library definition #
#############################

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "include/*")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*")

add_executable(BatteryApp ${SRC_FILES} ${HEADER_FILES})
target_compile_features(BatteryApp PRIVATE cxx_std_17)
set_target_properties(BatteryApp PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(BatteryApp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:include>
)



if(MSVC)
    set_property(TARGET BatteryApp PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    target_compile_options(BatteryApp PRIVATE -Wno-psabi)
endif()



############################
# Preprocessor definitions #
############################

if (WIN32)
    target_compile_definitions(BatteryApp PRIVATE
       WIN32_LEAN_AND_MEAN      # Prevents Windows.h from adding unnecessary includes
       NOMINMAX                 # Prevents Windows.h from defining min/max as macros 
       _CRT_SECURE_NO_WARNINGS
    )
endif()






###########
# Linking #
###########

target_link_libraries(BatteryApp Battery::Battery)




#######################
# Precompiled Headers #
#######################

target_precompile_headers(BatteryApp PRIVATE include/pch.h)





#######
# IDE #
#######

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" PREFIX "Source Files" FILES ${SRC_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADER_FILES})



###########
# Install #
###########

# Install binaries
install(
    TARGETS BatteryApp
    EXPORT BatteryAppTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)
