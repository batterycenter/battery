
function(battery_clear_available_modules)
    set(BATTERY_AVAILABLE_MODULES "" CACHE STRING "List of available battery modules" FORCE)
    set(BATTERY_AVAILABLE_MODULE_PATHS "" CACHE STRING "List of available battery module paths" FORCE)
endfunction()

function(battery_make_module_available module_name module_path)

    # First add it to the list of available modules
    set(AVAILABLE_MODULES_TEMP ${BATTERY_AVAILABLE_MODULES})
    list(APPEND AVAILABLE_MODULES_TEMP ${module_name})
    set(BATTERY_AVAILABLE_MODULES ${AVAILABLE_MODULES_TEMP} CACHE STRING "List of available battery modules" FORCE)

    # And to the list of module paths (the two lists are parallel)
    set(AVAILABLE_MODULE_PATHS_TEMP ${BATTERY_AVAILABLE_MODULE_PATHS})
    list(APPEND AVAILABLE_MODULE_PATHS_TEMP ${module_path})
    set(BATTERY_AVAILABLE_MODULE_PATHS ${AVAILABLE_MODULE_PATHS_TEMP} CACHE STRING "List of available battery module paths" FORCE)

endfunction()

function(battery_add_module module_name)

    # First we need to find the module path
    set(module_found FALSE)
    set(module_path "")
    foreach(name IN LISTS BATTERY_AVAILABLE_MODULES)
        list(FIND BATTERY_AVAILABLE_MODULES ${name} index)
        if(index EQUAL -1)
            continue()
        endif()

        # If the module name is found, save its path from the second list
        if(name STREQUAL module_name)
            list(GET BATTERY_AVAILABLE_MODULE_PATHS ${index} module_path)
            set(module_found TRUE)
            break()
        endif()
    endforeach()

    # Error if not found
    if(NOT module_found)
        message(FATAL_ERROR
                "Battery: Module not found: ${module_name}\n"
                "Make sure it is not misspelled and if it is a custom module, make sure that battery_make_module_available() is called before battery_add_module().")
    endif()

    # And now process the actual module
    add_subdirectory(${module_path} ${CMAKE_CURRENT_BINARY_DIR}/battery_modules/${module_name})

endfunction()
