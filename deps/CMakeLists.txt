
set(OLD_BUILD_EXAMPLES ${BUILD_EXAMPLES})
set(BUILD_EXAMPLES OFF CACHE BOOL  "" FORCE)
set(OLD_ENABLE_TEST ${ENABLE_TEST})
set(ENABLE_TEST OFF CACHE BOOL  "" FORCE)

# embed
add_subdirectory(embed)

# EventBus
add_subdirectory(EventBus)

# imgui
b_add_library(imgui STATIC ALIAS imgui::imgui
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_sdl2.cpp
        imgui/backends/imgui_impl_sdlrenderer2.cpp
)
target_include_directories(imgui PUBLIC "imgui")
target_include_directories(imgui PRIVATE "SDL/include")

# Lua
add_subdirectory(Lua)

# LuaBridge3
add_subdirectory(LuaBridge3)

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

# SDL_image
set(SDL2IMAGE_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(SDL_image)

# spdlog
add_subdirectory(spdlog)
target_compile_definitions(spdlog PUBLIC SPDLOG_COMPILED_LIB)

# utfcpp
add_subdirectory(utfcpp)

set(BUILD_EXAMPLES ${OLD_BUILD_EXAMPLES} CACHE BOOL "" FORCE)
set(ENABLE_TEST ${OLD_ENABLE_TEST} CACHE BOOL  "" FORCE)
