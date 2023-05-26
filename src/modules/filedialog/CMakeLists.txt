
# Usage of this module:
# battery_add_module(filedialog)
# target_link_libraries(... battery::filedialog)

battery_add_library(battery_filedialog STATIC ALIAS battery::filedialog)
battery_add_source_files(battery_filedialog
        src/filedialog.cpp
        )

battery_set_ide_folder(battery_filedialog "battery/modules")

target_link_libraries(battery_filedialog
        battery::core
    )
