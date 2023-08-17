
# Usage of this module:
# battery_add_module(eventbus)
# target_link_libraries(... PUBLIC battery::eventbus)

# Download the EventBus library from Github
set(ENABLE_TEST OFF)
battery_add_package("gh:gelldur/EventBus#v3.0.4")
b_set_target_ide_folder(EventBus "battery/modules")

b_add_library(battery_eventbus STATIC ALIAS battery::eventbus)
b_add_source_files(battery_eventbus   # This function also groups the files in the IDE
        src/eventbus.cpp
        )
target_include_directories(battery_eventbus PUBLIC include)

target_link_libraries(battery_eventbus PUBLIC battery::core EventBus)
b_set_target_ide_folder(battery_eventbus "battery/modules")