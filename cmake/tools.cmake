
include(cmake/CPM.cmake)

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

function(__APPLY_COMMON_TARGET_OPTIONS TARGET)  # For all libraries and executables

    # Set the C++ Standard
    target_compile_features(${TARGET} PRIVATE cxx_std_23)
    set_target_properties(${TARGET} PROPERTIES CXX_EXTENSIONS OFF)

    # Set common Preprocessor Flags
    if(MSVC)
        string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus /MP")      # TODO: Do this properly per-target
    else()
        target_compile_options(${TARGET} PRIVATE -Wno-psabi)
    endif()

    if (WIN32)
        target_compile_definitions(${TARGET} PRIVATE
                WIN32_LEAN_AND_MEAN      # Prevents Windows.h from adding unnecessary includes
                NOMINMAX                 # Prevents Windows.h from defining min/max as macros
                _CRT_SECURE_NO_WARNINGS
                )
        target_compile_definitions(${TARGET} PRIVATE
                UNICODE
                _UNICODE
                )
    else()
        target_compile_options(${TARGET} PRIVATE -Wno-psabi)
    endif()

    # Set common folders for the IDE
    battery_set_ide_folder("${CMAKE_CURRENT_LIST_DIR}/src" "Source Files")
    battery_set_ide_folder("${CMAKE_CURRENT_LIST_DIR}/include" "Header Files")
    battery_set_ide_folder("${CMAKE_CURRENT_LIST_DIR}/cmake" "CMake")
    battery_set_ide_folder("${CMAKE_CURRENT_LIST_DIR}/resources" "Resources")

endfunction()




macro(BATTERY_INIT_HUNTER)
    if (NOT DEFINED BATTERY_HUNTER_INIT_DONE)
        if (DEFINED PROJECT_NAME)
            message(FATAL_ERROR "Please include the Battery's .cmake file before any call to project(). This is required by Hunter, the embedded dependency manager for Battery. Detected project(${PROJECT_NAME})")
        endif()

        cmake_policy(PUSH)
        cmake_policy(SET CMP0135 OLD)
        include("${BATTERY_ROOT_DIR}/cmake/HunterGate.cmake")     # Initialize Hunter, our package manager
        HunterGate(
                URL "https://github.com/cpp-pm/hunter/archive/v0.24.13.tar.gz"
                SHA1 "2bc7384b2bf27db5b3847739a6a5361fa04075e7"
        )
        cmake_policy(POP)
        set(BATTERY_HUNTER_INIT_DONE ON)
    endif()
endmacro()

function(BATTERY_ADD_LIBRARY TARGET_NAME TYPE)

    # Define Target
    add_library(${TARGET_NAME} ${TYPE} ${ARGN})
    add_library(${TARGET_NAME}::${TARGET_NAME} ALIAS ${TARGET_NAME})

    # Apply things like C++ Standard, preprocessor defines, etc.
    __apply_common_target_options(${TARGET_NAME})

endfunction()

function(BATTERY_ADD_EXECUTABLE TARGET_NAME)

    # Define Target
    add_executable(${TARGET_NAME} ${ARGN})

    # Apply things like C++ Standard, preprocessor defines, etc.
    __apply_common_target_options(${TARGET_NAME})

endfunction()





macro(BATTERY_ADD_PACKAGE)
    CPMAddPackage(${ARGN})
endmacro()

function(BATTERY_SET_CACHE_VARIABLE VARIABLE VALUE)
    set(${VARIABLE} ${VALUE} CACHE BOOL "" FORCE)
endfunction()

function(BATTERY_SET_OUTPUT_NAME TARGET NAME)
    set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${NAME})
endfunction()

function(BATTERY_SET_IDE_FOLDER DIRECTORY LABEL)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    file(GLOB_RECURSE FILES "${DIRECTORY}/*")
    source_group(TREE "${DIRECTORY}" PREFIX ${LABEL} FILES ${FILES})
endfunction()

function(BATTERY_PRECOMPILE_HEADERS TARGET FILE)
    target_precompile_headers(${TARGET} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${FILE}>")
endfunction()

function(BATTERY_REQUIRE_FIND_PACKAGE PACKAGE_NAME PACKAGE_NAME_FOUND ADDITIONAL_MESSAGE)

    # Simply test the package and print understandable error message if it's not found
    find_package(${PACKAGE_NAME} QUIET)
    if (NOT ${PACKAGE_NAME_FOUND})
        message(FATAL_ERROR "${PACKAGE_NAME} development libraries not found! ${ADDITIONAL_MESSAGE}")
    endif()

endfunction()




function(BATTERY_EMBED TARGET FILE_NAME TYPE)

    if (NOT BATTERY_ROOT_DIR)
        message(FATAL_ERROR "battery_embed(): Battery root directory has not been set. Did you include the Battery library?")
    endif()

    if (NOT BATTERY_EMBED_BUILT)
        message(STATUS "[Battery] Building and verifying Battery-Embed tool")
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/battery_embed)

        execute_process(                # Configure the battery_embed project
                COMMAND ${CMAKE_COMMAND} ${BATTERY_ROOT_DIR}/battery_embed -DCMAKE_BUILD_TYPE=Release
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/battery_embed
                RESULT_VARIABLE RESULT
                OUTPUT_VARIABLE OUTPUT
                )
        if (NOT ${RESULT} EQUAL 0)
            message(FATAL_ERROR "[Battery] The Battery-Embed tool failed to configure. Output: \n${OUTPUT}")
        endif()

        include(ProcessorCount)
        ProcessorCount(N)
        execute_process(                # Build the battery_embed project
                COMMAND ${CMAKE_COMMAND} --build .  -j${N} --config=Release
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/battery_embed
                RESULT_VARIABLE RESULT
                OUTPUT_VARIABLE OUTPUT
                )
        if (NOT ${RESULT} EQUAL 0)
            message(FATAL_ERROR "[Battery] The Battery-Embed tool failed to build. Output: \n${OUTPUT}")
        endif()
        message(STATUS "[Battery] Battery-Embed built")
        set(BATTERY_EMBED_BUILT ON PARENT_SCOPE)
    endif()


    # Now verify the tool and retrieve the resulting filename
    execute_process(
            COMMAND ${CMAKE_CURRENT_BINARY_DIR}/battery_embed/Release/battery_embed.exe
            ${FILE_NAME} ${CMAKE_CURRENT_BINARY_DIR}/resources --print-src-name-only
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/
            OUTPUT_VARIABLE SOURCE_FILE
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE RESULT
    )
    if (NOT ${RESULT} EQUAL 0)
        message(FATAL_ERROR "[Battery] Battery-Embed could not be executed (${RESULT}): ${SOURCE_FILE}")
    endif()

    # Define the command to be run at build time
    if (${TYPE} STREQUAL "TEXT")
        set(COMMAND_PRINT "Generating TEXT file ${FILE_NAME}")
        set(BINARY "")
    elseif(${TYPE} STREQUAL "BINARY")
        set(COMMAND_PRINT "Generating BINARY file ${FILE_NAME}")
        set(BINARY "--binary")
    else()
        message(FATAL_ERROR "Invalid file type given to battery_embed(). Can either be TEXT or BINARY")
    endif()

    add_custom_command(OUTPUT ${SOURCE_FILE}_nonexistent
        COMMAND ${CMAKE_BINARY_DIR}/battery_embed/Release/battery_embed.exe     # TODO: This will not work on GCC
            ${FILE_NAME} ${CMAKE_CURRENT_BINARY_DIR}/resources ${BINARY}
        COMMENT ${COMMAND_PRINT}
        )

    # Link the source file to the target
    get_filename_component(OUT_DIR_PARENT ${CMAKE_CURRENT_BINARY_DIR}/resources DIRECTORY)
    target_include_directories(${TARGET} PUBLIC ${OUT_DIR_PARENT})
    file(WRITE ${SOURCE_FILE} "[temporary file by cmake]")          # Create an empty file, so that CMake can successfully link.
    target_sources(${TARGET} PUBLIC ${SOURCE_FILE} ${SOURCE_FILE}_nonexistent)   # File will be overwritten at build time by Battery-Embed

endfunction()

