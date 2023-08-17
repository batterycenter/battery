
# embed
set(BUILD_EXAMPLES OFF)
add_subdirectory(embed)

function(b_embed_text)
    embed_text(${ARGN})
endfunction()

function(b_embed_binary)
    embed_binary(${ARGN})
endfunction()

# fmt
set(FMT_TEST OFF CACHE BOOL "" FORCE)
set(FMT_DOC OFF CACHE BOOL "" FORCE)
set(FMT_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(fmt)

# reproc
set(REPROC++ ON CACHE BOOL "" FORCE)
add_subdirectory(reproc)
add_library(reproc::reproc++ ALIAS reproc++)

# spdlog
add_subdirectory(spdlog)
target_link_libraries(spdlog PUBLIC fmt::fmt)
target_compile_definitions(spdlog PUBLIC SPDLOG_FMT_EXTERNAL SPDLOG_COMPILED_LIB)

# utfcpp
add_subdirectory(utfcpp)