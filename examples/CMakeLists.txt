
function(battery_add_example_no_main name)
    b_add_executable(${name} ${ARGN} EXCLUDE_FROM_ALL)
    b_set_ide_folder(${name} "battery/examples")
    target_link_libraries(${name} PUBLIC battery::core_no_main)
endfunction()

function(battery_add_example name)
    battery_add_example_no_main(${name} ${ARGN})
    target_link_libraries(${name} PUBLIC battery::core)
endfunction()

add_subdirectory(core)
#add_subdirectory(graphics)
#add_subdirectory(gui)
#add_subdirectory(python)