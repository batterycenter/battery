
# battery core library
b_add_library(battery_core STATIC ALIAS battery::core)
b_add_source_files(battery_core

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
        src/application.cpp
        src/cache.cpp
        src/console.cpp
        src/constants.cpp
        src/environment.cpp
        src/error.cpp
        src/events.cpp
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
        src/renderer.cpp
        src/renderwindow.cpp
        src/resource.cpp
        src/signal.cpp
        src/string.cpp
        src/thread.cpp
        src/time.cpp
        src/uuid.cpp
        src/window.cpp)

b_set_ide_folder(battery_core "battery/modules")
b_add_module(cli11)

#set_property(SOURCE src/extern/platform_folders.cpp PROPERTY CMAKE_CXX_CLANG_TIDY "")
#set_property(SOURCE src/extern/utf8proc.c PROPERTY CMAKE_CXX_CLANG_TIDY "")
#set_property(SOURCE src/extern/utf8proc_data.c PROPERTY CMAKE_CXX_CLANG_TIDY "")

target_link_libraries(battery_core PUBLIC
        spdlog::spdlog
        reproc::reproc++
        utf8::cpp
        battery::cli11
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image-static
        imgui::imgui
        EventBus
)

if (WIN32)
    target_link_libraries(battery_core PUBLIC Winmm.lib dwmapi.lib)   # for b::sleep_ms timer resolution and immersive dark mode
endif ()

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()