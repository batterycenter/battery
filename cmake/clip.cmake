
set(CLIP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/clip)
add_library(clip STATIC)
add_library(clip::clip ALIAS clip)

set(SRC_FILES)
if (WIN32)
list(APPEND CLIP_SRC ${CLIP_DIR}/clip_win.cpp)
elseif ()
list(APPEND CLIP_SRC ${CLIP_DIR}/clip_x11.cpp)
endif ()

target_sources(clip PRIVATE
        ${CLIP_DIR}/clip.cpp
        ${CLIP_DIR}/image.cpp
        ${CLIP_SRC}
)

target_include_directories(clip PUBLIC
    $<BUILD_INTERFACE:${CLIP_DIR}/>
    $<INSTALL_INTERFACE:/>
)

install(
    TARGETS clip
    EXPORT clipTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)

install(
    EXPORT clipTargets 
    DESTINATION "lib/cmake/clip"
    NAMESPACE clip::
)

# Install headers
install(
    DIRECTORY "${CLIP_DIR}/"
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.h"
)

install(
    FILES "cmake/clipConfig.cmake"
    DESTINATION "lib/cmake/clip"
)
