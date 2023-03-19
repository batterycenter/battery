
include(cmake/CPM.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

function(BATTERY_TEST_IF_COMPILES RESULT_VAR OUTPUT_VAR STD_VERSION SOURCE_CONTENT)
    message(STATUS "Checking for compiler feature ${RESULT_VAR}")
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/try_compile/main.cpp ${SOURCE_CONTENT})
    try_compile(
        ${RESULT_VAR} 
        ${CMAKE_CURRENT_BINARY_DIR}/try_compile/
        ${CMAKE_CURRENT_BINARY_DIR}/try_compile/main.cpp
        CXX_STANDARD ${STD_VERSION}
        OUTPUT_VARIABLE _OUTPUT_VAR
    )
    set(${OUTPUT_VAR} ${_OUTPUT_VAR} PARENT_SCOPE)
    file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/try_compile)
    if (${RESULT_VAR})
        message(STATUS "Checking for compiler feature ${RESULT_VAR} - Success")
    else()
        message(STATUS "Checking for compiler feature ${RESULT_VAR} - Failed")
    endif()
endfunction()

function(BATTERY_MUST_HAVE_CXX20_FEATURE FEATURE_NAME SOURCE_CONTENT)
    if (${FEATURE_NAME})
        return()
    endif()
    battery_test_if_compiles(${FEATURE_NAME} OUTPUT_VAR 20 ${SOURCE_CONTENT})
    if (NOT ${FEATURE_NAME})
        message(SEND_ERROR "Test ${FEATURE_NAME} Failed. Reason: Compiler output: ${OUTPUT_VAR}")
        set(${FEATURE_NAME} OFF CACHE BOOL "" FORCE)
    else()
        set(${FEATURE_NAME} ON CACHE BOOL "" FORCE)
    endif()
    mark_as_advanced(${FEATURE_NAME})
endfunction()

function(BATTERY_CHECK_REQUIRED_COMPILER_FEATURES)

    battery_must_have_cxx20_feature(HAVE_JTHREAD
        "#include <thread>
        #include <iostream>
        int main() {
            std::jthread thread([] {
                std::cout << \"Hello world\" << std::endl\\\;
            })\\\;
        }")
endfunction()

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
    target_compile_features(${TARGET} PRIVATE cxx_std_20)
    set_target_properties(${TARGET} PROPERTIES CXX_EXTENSIONS OFF)

    # Set common Preprocessor Flags
    if(MSVC)
        target_compile_options(${TARGET} PRIVATE /Zc:__cplusplus /MP /W2 /utf-8)
    else()
        target_compile_options(${TARGET} PRIVATE -Wno-psabi -Wall -Wextra)
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
    endif()

    # Set common folders for the IDE
    battery_set_ide_folder("${CMAKE_CURRENT_LIST_DIR}/src" "Source Files")
    battery_set_ide_folder("${CMAKE_CURRENT_LIST_DIR}/include" "Header Files")
    battery_set_ide_folder("${CMAKE_CURRENT_LIST_DIR}/cmake" "CMake")
    battery_set_ide_folder("${CMAKE_CURRENT_LIST_DIR}/resources" "Resources")

endfunction()




function(BATTERY_ADD_LIBRARY TARGET TYPE)
    set(booleanArguments)
    set(singleValueArguments ALIAS)
    set(multiValueArguments TARGETS)
    cmake_parse_arguments(ARGS "${options}" "${singleValueArguments}" "${multiValueArguments}" ${ARGN})

    # Define Target
    add_library(${TARGET} ${TYPE} ${ARGS_UNPARSED_ARGUMENTS})
    if (ARGS_ALIAS)
        add_library(${ARGS_ALIAS} ALIAS ${TARGET})
    else()
        add_library(${TARGET}::${TARGET} ALIAS ${TARGET})
    endif()

    # Apply things like C++ Standard, preprocessor defines, etc.
    __apply_common_target_options(${TARGET})

endfunction()

function(BATTERY_ADD_EXECUTABLE TARGET_NAME)

    # Define Target
    add_executable(${TARGET_NAME} ${ARGN})

    # Apply things like C++ Standard, preprocessor defines, etc.
    __apply_common_target_options(${TARGET_NAME})

endfunction()

function(BATTERY_ADD_TEST TARGET)
    if (NOT BATTERY_GTEST_INCLUDED)
        set(gtest_force_shared_crt "Use shared (DLL) run-time lib even when Google Test is built as static lib." ON FORCE)
        battery_add_package("gh:google/googletest#main")      # Live at head: main branch recommended
        include(GoogleTest)
        set(BATTERY_GTEST_INCLUDED ON PARENT_SCOPE)
    endif()
    battery_add_executable(${TARGET} ${ARGN})
    target_link_libraries(${TARGET} gtest_main)
    gtest_discover_tests(${TARGET})
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

