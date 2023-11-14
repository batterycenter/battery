
# embed
set(BUILD_EXAMPLES OFF)
add_subdirectory(embed)

# reproc
set(REPROC++ ON CACHE BOOL "" FORCE)
add_subdirectory(reproc)
add_library(reproc::reproc++ ALIAS reproc++)

# SDL
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_STATIC_PIC ON CACHE BOOL "" FORCE)
set(SDL_AUDIO ON CACHE BOOL "" FORCE)
set(SDL_VIDEO ON CACHE BOOL "" FORCE)
set(SDL_EVENTS ON CACHE BOOL "" FORCE)
set(SDL_RENDER ON CACHE BOOL "" FORCE)
set(SDL_JOYSTICK ON CACHE BOOL "" FORCE)
set(SDL_LOADSO ON CACHE BOOL "" FORCE)
set(SDL_THREADS ON CACHE BOOL "" FORCE)
add_subdirectory(SDL)

# spdlog
add_subdirectory(spdlog)
target_compile_definitions(spdlog PUBLIC SPDLOG_COMPILED_LIB)

# utfcpp
add_subdirectory(utfcpp)