
# Usage of this module:
# battery_add_module(utf8cpp)
# target_link_libraries(... battery::utfcpp)

# Download the utfcpp unicode library from Github
battery_add_package("gh:nemtrif/utfcpp#v3.2.3")
battery_add_alias_library(battery::utfcpp utf8cpp)

battery_set_target_ide_folder(utf8cpp "battery/modules")