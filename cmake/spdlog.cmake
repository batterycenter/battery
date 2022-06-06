
set(SPDLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/spdlog)
add_library(spdlog STATIC)

target_sources(spdlog PRIVATE
        ${SPDLOG_DIR}/src/async.cpp
        ${SPDLOG_DIR}/src/cfg.cpp
        ${SPDLOG_DIR}/src/color_sinks.cpp
        ${SPDLOG_DIR}/src/file_sinks.cpp
        ${SPDLOG_DIR}/src/fmt.cpp
        ${SPDLOG_DIR}/src/spdlog.cpp
        ${SPDLOG_DIR}/src/stdout_sinks.cpp
)

target_include_directories(spdlog PUBLIC
    $<BUILD_INTERFACE:${SPDLOG_DIR}/include>
    $<INSTALL_INTERFACE:/>
)

target_compile_definitions(spdlog PUBLIC SPDLOG_COMPILED_LIB)

install(
    TARGETS spdlog
    EXPORT spdlogTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)

install(
    EXPORT spdlogTargets 
    DESTINATION "lib/cmake/spdlog"
    NAMESPACE spdlog::
)

# Install headers
install(
    DIRECTORY "${SPDLOG_DIR}/include/"
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.h"
)

install(
    FILES "cmake/spdlogConfig.cmake"
    DESTINATION "lib/cmake/spdlog"
)
