
function(battery_embed TARGET RESOURCE_FILE SYMBOL_NAME TYPE)

    # Define the command to be run at build time
    get_filename_component(RESOURCE_FILENAME ${RESOURCE_FILE} NAME)
    if (${TYPE} STREQUAL "TEXT")
        set(BINARY "")
    elseif(${TYPE} STREQUAL "BINARY")
        set(BINARY "--binary")
    else()
        message(FATAL_ERROR "Invalid file type given to battery_embed(). Can either be TEXT or BINARY")
    endif()

    # How to call the tool
    if (WIN32)
        set(TOOL_EXE ${CMAKE_BINARY_DIR}/battery_embed_bin/battery_embed.exe)
    else()
        set(TOOL_EXE ${CMAKE_BINARY_DIR}/battery_embed_bin/battery_embed)
    endif()
    get_filename_component(RESOURCE_FILENAME ${RESOURCE_FILE} NAME)
    get_filename_component(RESOURCE_DIRECTORY ${RESOURCE_FILE} DIRECTORY)
    set(TARGET_HEADER ${CMAKE_CURRENT_BINARY_DIR}/resources_include/resources/${SYMBOL_NAME}.hpp)
    set(TARGET_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/resources_include/resources/${SYMBOL_NAME}.cpp)

    add_custom_command(
            COMMAND ${TOOL_EXE} ${RESOURCE_FILE} ${TARGET_HEADER} --symbol_name ${SYMBOL_NAME} --header ${BINARY}
            DEPENDS ${RESOURCE_FILE} battery::embed
            OUTPUT ${TARGET_HEADER})

    add_custom_command(
            COMMAND ${TOOL_EXE} ${RESOURCE_FILE} ${TARGET_SOURCE} --symbol_name ${SYMBOL_NAME} --cpp ${BINARY}
            DEPENDS ${RESOURCE_FILE} battery::embed
            OUTPUT ${TARGET_SOURCE})

    target_sources(${TARGET} PRIVATE ${TARGET_HEADER} ${TARGET_SOURCE})
    source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/resources_include" PREFIX "" FILES ${TARGET_HEADER} ${TARGET_SOURCE})
    source_group(TREE "${RESOURCE_DIRECTORY}/" PREFIX "Resource Files" FILES ${RESOURCE_FILE})
    if (MSVC)
        #target_sources(${TARGET} PUBLIC ${RESOURCE_FILE})
        #set_property(SOURCE ${RESOURCE_FILE} PROPERTY VS_SETTINGS "ExcludedFromBuild=true")     # TODO: This is CMake 3.18+ only
    endif()

    add_custom_target(${TARGET}-embed-${RESOURCE_FILENAME} DEPENDS battery::embed ${TARGET_HEADER} ${TARGET_SOURCE})
    add_dependencies(${TARGET} ${TARGET}-embed-${RESOURCE_FILENAME})
    set_target_properties(${TARGET}-embed-${RESOURCE_FILENAME} PROPERTIES FOLDER "${TARGET}-resources")

    # Where all headers and sources will be placed
    target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/resources_include)

endfunction()