
# battery core library
b_add_library(battery_core_no_main STATIC ALIAS battery::core_no_main)
b_add_source_files(battery_core_no_main

        src/tray/components/button.cpp
        src/tray/components/label.cpp
        src/tray/components/separator.cpp
        src/tray/components/submenu.cpp
        src/tray/components/syncedtoggle.cpp
        src/tray/components/toggle.cpp
        src/tray/core/linux/tray.cpp
        src/tray/core/windows/tray.cpp
        src/tray/core/entry.cpp
        src/tray/core/traybase.cpp

        src/extern/platform_folders.cpp
        src/extern/utf8proc.c
        src/extern/utf8proc_data.c

        src/internal/folders.cpp

        src/base64.cpp
        src/console_application.cpp
        src/cache.cpp
        src/console.cpp
        src/constants.cpp
        src/environment.cpp
        src/error.cpp
        src/file_watcher.cpp
        src/filedialog.cpp
        src/lockfile.cpp
        src/log.cpp
        src/main.cpp
        src/messages.cpp
        src/folders.cpp
        src/format.cpp
        src/fs.cpp
        src/process.cpp
        src/random.cpp
        src/resource.cpp
        src/signal.cpp
        src/string.cpp
        src/time.cpp
        src/uuid.cpp)

b_set_ide_folder(battery_core_no_main "battery/modules")

#set_property(SOURCE src/extern/platform_folders.cpp PROPERTY CMAKE_CXX_CLANG_TIDY "")
#set_property(SOURCE src/extern/utf8proc.c PROPERTY CMAKE_CXX_CLANG_TIDY "")
#set_property(SOURCE src/extern/utf8proc_data.c PROPERTY CMAKE_CXX_CLANG_TIDY "")

target_link_libraries(battery_core_no_main 
    spdlog::spdlog 
    reproc::reproc++
    utf8::cpp
)

if (WIN32)
    target_link_libraries(battery_core_no_main Winmm.lib dwmapi.lib)   # for b::sleep_ms timer resolution and immersive dark mode
endif ()

b_add_library(battery_core STATIC ALIAS battery::core)
b_add_source_files(battery_core battery_main/main.cpp)
target_link_libraries(battery_core battery::core_no_main)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()