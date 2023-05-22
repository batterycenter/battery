execute_process(COMMAND
        ${TOOL_EXE}
        ${RESOURCE_FILE}
        ${TARGET_FILE}
        --symbol_name
        ${SYMBOL_NAME}
        ${TYPE}
        ${BINARY}
        OUTPUT_QUIET
        RESULT_VARIABLE RESULT)

if (NOT RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to execute battery_embed: Process returned with error code ${RESULT}")
endif()