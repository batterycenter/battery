
function(GET_GITHUB_DEPENDENCY NAME REQUIRED_FILE REPOSITORY_URL BRANCH)
    if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${REQUIRED_FILE}")
        find_package(Git REQUIRED)
        if(GIT_FOUND)
            message(STATUS "${REQUIRED_FILE} not found, getting dependency '${NAME}'")
            file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/modules)
            set(GIT_COMMAND ${GIT_EXECUTABLE} clone ${REPOSITORY_URL} --single-branch --branch=${BRANCH} --depth=1)
            execute_process(COMMAND ${GIT_COMMAND}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/modules
                    RESULT_VARIABLE GIT_RESULT ERROR_VARIABLE GIT_OUTPUT OUTPUT_QUIET)
            if(NOT GIT_RESULT EQUAL "0")
                message(FATAL_ERROR "${GIT_COMMAND} failed. Error: '${GIT_RESULT}' Output: '${GIT_OUTPUT}'. Please check the dependencies")
            endif()
        else()
            message(FATAL_ERROR "Cannot get dependencies, git executable not found. Please install Git")
        endif()
    endif()
endfunction()

#function(USE_STATIC_RUNTIME TARGET)
#    if (MSVC)
#        set_property(TARGET ${TARGET_NAME} PROPERTY
#                MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#    else()
#        target_compile_options(${TARGET} PRIVATE -static -static-libgcc -static-libstdc++ -Bstatic)
#    endif()
#endfunction()

function(BATTERY_ADD_EXECUTABLE TARGET_NAME ARGS)

    # Define Target
    add_executable(${TARGET_NAME} ${ARGS})

    # Set the C++ Standard
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_23)
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)

    # Set common Preprocessor Flags
    if(MSVC)
        string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus /MP")
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -Wno-psabi)
    endif()

    if (WIN32)
        target_compile_definitions(${TARGET_NAME} PRIVATE
                WIN32_LEAN_AND_MEAN      # Prevents Windows.h from adding unnecessary includes
                NOMINMAX                 # Prevents Windows.h from defining min/max as macros
                _CRT_SECURE_NO_WARNINGS
                )
        target_compile_definitions(${TARGET_NAME} PRIVATE
                UNICODE
                _UNICODE
                )
    endif()

endfunction()

