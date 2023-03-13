
battery_add_library(battery_core_main STATIC ALIAS battery::core::main src/main.cpp)

if (BATTERY_CORE_NO_MAIN)
    target_compile_definitions(battery_core_main PUBLIC BATTERY_CORE_NO_MAIN)
endif()