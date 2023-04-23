
add_subdirectory(BatteryApp)
add_subdirectory(BatteryEmbed)
add_subdirectory(Lockfile)
add_subdirectory(PythonEmbed)

if (NOT BATTERY_CORE_NO_TRAY)
    add_subdirectory(TrayIconAnimated)
endif ()