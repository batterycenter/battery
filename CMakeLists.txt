cmake_minimum_required(VERSION 3.13)

set(LIBRARY_NAME Battery)
project(${LIBRARY_NAME} VERSION 0.1.1 LANGUAGES CXX)


##################################
# Options: Library configuration #
##################################

option(DISABLE_LOGGING "Strip all logging commands" OFF)



#############################
# Static library definition #
#############################

add_library(${LIBRARY_NAME} "")
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)
set_target_properties(${LIBRARY_NAME} PROPERTIES CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()



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
    )
endif()



#######################
# Precompiled Headers #
#######################

target_precompile_headers(${LIBRARY_NAME} PUBLIC include/Battery/pch.h)



#######################
# Include directories #
#######################

target_include_directories(${LIBRARY_NAME} PUBLIC "include")
target_include_directories(${LIBRARY_NAME} PUBLIC "include/clip")



################
# Source files #
################

file(GLOB_RECURSE COMMON_SRC "src/Battery/*")
list(APPEND COMMON_SRC "src/clip/clip_none.cpp" "src/clip/clip.cpp" "src/clip/image.cpp")

file(GLOB_RECURSE REMOVE_WIN "src/Battery/Platform/win/*")
file(GLOB_RECURSE REMOVE_X11 "src/Battery/Platform/x11/*")

if (WIN32)
    list(APPEND PLATFORM_SRC "src/clip/clip_win.cpp")
    list(REMOVE_ITEM COMMON_SRC ${REMOVE_X11})
else()
    list(APPEND PLATFORM_SRC "src/clip/clip_x11.cpp")
    list(REMOVE_ITEM COMMON_SRC ${REMOVE_WIN})
endif()

target_sources(${LIBRARY_NAME} PUBLIC ${COMMON_SRC} ${PLATFORM_SRC})



###########
# Linking #
###########

target_link_libraries(${LIBRARY_NAME} PUBLIC ${CONAN_LIBS})



###########
# Install #
###########

# Install binaries
install(
  TARGETS ${LIBRARY_NAME}
  EXPORT "${TARGETS_EXPORT_NAME}"
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
