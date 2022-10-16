
set(NATIVE_FILE_DIALOG_DIR $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/modules/nativefiledialog/>$<INSTALL_INTERFACE:>)
add_library(nativefiledialog STATIC)
add_library(nativefiledialog::nativefiledialog ALIAS nativefiledialog)

if (WIN32)
    target_sources(nativefiledialog PUBLIC
            ${NATIVE_FILE_DIALOG_DIR}/src/nfd_common.c
            ${NATIVE_FILE_DIALOG_DIR}/src/nfd_win.cpp)
elseif ()
    target_sources(nativefiledialog PUBLIC
            ${NATIVE_FILE_DIALOG_DIR}/src/nfd_common.c
            ${NATIVE_FILE_DIALOG_DIR}/src/nfd_zenity.c)
endif ()



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
