
# Usage of this module:
# battery_add_module(reproc++)
# target_link_libraries(... battery::reproc++)

# Download the reproc process library from Github
set(REPROC++ ON)
battery_add_package("gh:DaanDeMeyer/reproc#v14.2.4")
battery_add_alias_library(battery::reproc++ reproc++)

battery_set_target_ide_folder(reproc "battery/modules")
battery_set_target_ide_folder(reproc++ "battery/modules")