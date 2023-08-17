
# Usage of this module:
# battery_add_module(json)
# target_link_libraries(... battery::json)

set(JSON_BuildTests OFF CACHE INTERNAL "")

# Download the nlohmann::json json fileformat parsing library from Github
battery_add_package("gh:nlohmann/json#v3.11.2")
b_add_alias_library(battery::json nlohmann_json)

b_set_target_ide_folder(nlohmann_json "battery/modules")