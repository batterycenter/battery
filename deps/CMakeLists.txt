
# embed
set(BUILD_EXAMPLES OFF)
add_subdirectory(embed)

# reproc
set(REPROC++ ON CACHE BOOL "" FORCE)
add_subdirectory(reproc)
add_library(reproc::reproc++ ALIAS reproc++)

# spdlog
add_subdirectory(spdlog)
target_compile_definitions(spdlog PUBLIC SPDLOG_COMPILED_LIB)

# utfcpp
add_subdirectory(utfcpp)