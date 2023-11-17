
function(b_clear_available_modules)
    set(B_AVAILABLE_MODULES "" CACHE STRING "List of available battery modules" FORCE)
    set(B_AVAILABLE_MODULE_PATHS "" CACHE STRING "List of available battery module paths" FORCE)
    set(B_AVAILABLE_MODULE_BINARY_DIRS "" CACHE STRING "List of available battery module binary dirs" FORCE)
endfunction()

function(b_make_module_available module_name module_path)

    # First add it to the list of available modules
    set(AVAILABLE_MODULES_TEMP ${B_AVAILABLE_MODULES})
    list(APPEND AVAILABLE_MODULES_TEMP ${module_name})
    set(B_AVAILABLE_MODULES ${AVAILABLE_MODULES_TEMP} CACHE STRING "List of available battery modules" FORCE)

    # And to the list of module paths (the two lists are parallel)
    set(AVAILABLE_MODULE_PATHS_TEMP ${B_AVAILABLE_MODULE_PATHS})
    list(APPEND AVAILABLE_MODULE_PATHS_TEMP ${module_path})
    set(B_AVAILABLE_MODULE_PATHS ${AVAILABLE_MODULE_PATHS_TEMP} CACHE STRING "List of available battery module paths" FORCE)

    # And to the list of module binary dirs
    set(AVAILABLE_MODULE_BINARY_DIRS_TEMP ${B_AVAILABLE_MODULE_BINARY_DIRS})
    list(APPEND AVAILABLE_MODULE_BINARY_DIRS_TEMP ${CMAKE_CURRENT_BINARY_DIR}/battery_modules/${module_name})
    set(B_AVAILABLE_MODULE_BINARY_DIRS ${AVAILABLE_MODULE_BINARY_DIRS_TEMP} CACHE STRING "List of available battery module binary dirs" FORCE)

    # And finally set the module as not loaded
    set(B_MODULE_LOADED_${module_name} OFF CACHE INTERNAL "" FORCE)

endfunction()

function(b_add_module module_name)

    # First we need to find the module path
    set(module_found FALSE)
    set(module_path "")
    set(module_binary_dir "")
    foreach(name IN LISTS B_AVAILABLE_MODULES)
        list(FIND B_AVAILABLE_MODULES ${name} index)
        if(index EQUAL -1)
            continue()
        endif()

        # If the module name is found, save its path from the second list
        if(name STREQUAL module_name)
            list(GET B_AVAILABLE_MODULE_PATHS ${index} module_path)
            list(GET B_AVAILABLE_MODULE_BINARY_DIRS ${index} module_binary_dir)
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
    if (NOT ${B_MODULE_LOADED_${module_name}})
        add_subdirectory(${module_path} ${module_binary_dir})

        # Mark the module as loaded globally, for all compiled targets
        set(B_MODULE_LOADED_${module_name} ON CACHE INTERNAL "" FORCE)
        target_compile_definitions(battery_core PUBLIC B_MODULE_LOADED_${module_name})
    endif()

endfunction()
