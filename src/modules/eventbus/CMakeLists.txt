
# Usage of this module:
# battery_add_module(eventbus)
# target_link_libraries(... battery::eventbus)

# Download the EventBus library from Github
set(ENABLE_TEST OFF)
battery_add_package("gh:gelldur/EventBus#v3.0.3")
battery_add_alias_library(battery::eventbus EventBus)

battery_set_target_ide_folder(EventBus "battery/modules")