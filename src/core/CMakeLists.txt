
# battery core library
battery_add_library(battery_core_no_main STATIC ALIAS battery::core_no_main)
battery_add_source_files(battery_core_no_main
        src/extern/platform_folders.cpp
        src/extern/utf8proc.c
        src/extern/utf8proc_data.c
        src/internal/folders.cpp

        src/console_application.cpp
        src/cache.cpp
        src/console.cpp
        src/constants.cpp
        src/environment.cpp
        src/error.cpp
        src/file_watcher.cpp
        src/lockfile.cpp
        src/log.cpp
        src/main.cpp
        src/messages.cpp
        src/folders.cpp
        src/process.cpp
        src/random.cpp
        src/resource.cpp
        src/signal.cpp
        src/string.cpp
        src/time.cpp
        src/uuid.cpp)

battery_set_ide_folder(battery_core_no_main "battery/modules")

#set_property(SOURCE src/extern/platform_folders.cpp PROPERTY CMAKE_CXX_CLANG_TIDY "")
#set_property(SOURCE src/extern/utf8proc.c PROPERTY CMAKE_CXX_CLANG_TIDY "")
#set_property(SOURCE src/extern/utf8proc_data.c PROPERTY CMAKE_CXX_CLANG_TIDY "")

target_link_libraries(battery_core_no_main 
    spdlog::spdlog 
    reproc::reproc++
    utf8::cpp
)

battery_add_module(check)

if (WIN32)
    target_link_libraries(battery_core_no_main Winmm.lib dwmapi.lib)   # for b::sleep_ms timer resolution and immersive dark mode
endif ()

battery_add_library(battery_core STATIC ALIAS battery::core)
battery_add_source_files(battery_core battery_main/main.cpp)
target_link_libraries(battery_core battery::core_no_main)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()