
add_subdirectory(BatteryApp)
add_subdirectory(BatteryEmbed)
add_subdirectory(GraphicsExample)
add_subdirectory(LockfileBlocking)
add_subdirectory(LockfileNonBlocking)
add_subdirectory(PythonEmbed)

if (NOT BATTERY_CORE_NO_TRAY)
    add_subdirectory(TrayIconAnimated)
endif ()