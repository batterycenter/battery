
set(NATIVE_FILE_DIALOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/nativefiledialog)
add_library(nativefiledialog STATIC)
add_library(nativefiledialog::nativefiledialog ALIAS nativefiledialog)

set(SRC_FILES)
if (WIN32)
list(APPEND nativefiledialog_SRC ${NATIVE_FILE_DIALOG_DIR}/src/nfd_win.cpp)
elseif ()
list(APPEND nativefiledialog_SRC ${NATIVE_FILE_DIALOG_DIR}/src/nfd_zenity.cpp)
endif ()

target_sources(nativefiledialog PRIVATE
        ${NATIVE_FILE_DIALOG_DIR}/src/nfd_common.c
        ${nativefiledialog_SRC}
)

target_include_directories(nativefiledialog PUBLIC
    $<BUILD_INTERFACE:${NATIVE_FILE_DIALOG_DIR}/src/include>
    $<INSTALL_INTERFACE:/>
)

install(
    TARGETS nativefiledialog
    EXPORT nativefiledialogTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)

install(
    EXPORT nativefiledialogTargets 
    DESTINATION "lib/cmake/nativefiledialog"
    NAMESPACE nativefiledialog::
)

# Install headers
install(
    DIRECTORY "${NATIVE_FILE_DIALOG_DIR}/"
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.h"
)

install(
    FILES "cmake/nativefiledialogConfig.cmake"
    DESTINATION "lib/cmake/nativefiledialog"
)
