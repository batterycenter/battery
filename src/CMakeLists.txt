
# Add modules
add_subdirectory(modules)

# battery core library
battery_add_library(battery_core_no_main STATIC ALIAS battery::core_no_main)
battery_add_source_files(battery_core_no_main
        src/extern/platform_folders.cpp
        src/extern/utf8proc.c
        src/extern/utf8proc_data.c
        src/internal/folders.cpp

        src/application.cpp
        src/cache.cpp
        src/console.cpp
        src/constants.cpp
        src/fs.cpp
        src/lockfile.cpp
        src/log.cpp
        src/main.cpp
        src/messages.cpp
        src/folders.cpp
        src/process.cpp
        src/resource.cpp
        src/signal.cpp
        src/string.cpp
        src/time.cpp
        src/uuid.cpp)

battery_set_ide_folder(battery_core_no_main "battery/modules")

battery_add_module(spdlog)
battery_add_module(reproc++)
battery_add_module(utfcpp)
target_link_libraries(battery_core_no_main battery::spdlog battery::reproc++ battery::utfcpp)

if (WIN32)
    target_link_libraries(battery_core_no_main Winmm.lib)   # for b::sleep_ms timer resolution
endif ()

battery_add_library(battery_core STATIC ALIAS battery::core)
battery_add_source_files(battery_core battery_main/main.cpp)
target_link_libraries(battery_core battery::core_no_main)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()