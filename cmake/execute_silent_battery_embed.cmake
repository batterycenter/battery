execute_process(COMMAND
        ${TOOL_EXE}
        ${RESOURCE_FILE}
        ${TARGET_FILE}
        --symbol_name
        ${SYMBOL_NAME}
        ${TYPE}
        ${BINARY}
        OUTPUT_QUIET)