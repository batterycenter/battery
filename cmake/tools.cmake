
include(cmake/CPM.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

function(battery_test_if_compiles RESULT_VAR OUTPUT_VAR STD_VERSION SOURCE_CONTENT)
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

function(battery_must_have_cxx20_feature FEATURE_NAME SOURCE_CONTENT)
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

function(battery_check_required_compiler_features)

    battery_must_have_cxx20_feature(HAVE_JTHREAD
        "#include <thread>
        #include <iostream>
        int main() {
            std::jthread thread([] {
                std::cout << \"Hello world\" << std::endl\\\;
            })\\\;
        }")
endfunction()

function(__apply_common_target_options TARGET)  # For all libraries and executables

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
    endif()
    target_compile_definitions(${TARGET} PUBLIC UNICODE _UNICODE)

endfunction()




function(battery_add_library TARGET TYPE)
    set(booleanArguments)
    set(singleValueArguments ALIAS)
    set(multiValueArguments TARGETS)
    cmake_parse_arguments(ARGS "${options}" "${singleValueArguments}" "${multiValueArguments}" ${ARGN})

    add_library(${TARGET} ${TYPE} ${ARGS_UNPARSED_ARGUMENTS})
    if (ARGS_ALIAS)
        add_library(${ARGS_ALIAS} ALIAS ${TARGET})
    else()
        add_library(${TARGET}::${TARGET} ALIAS ${TARGET})
    endif()
    __apply_common_target_options(${TARGET})       # Apply things like C++ Standard, preprocessor defines, etc.
endfunction()

function(battery_add_executable TARGET)
    set(booleanArguments)
    set(singleValueArguments ALIAS)
    set(multiValueArguments TARGETS)
    cmake_parse_arguments(ARGS "${options}" "${singleValueArguments}" "${multiValueArguments}" ${ARGN})

    add_executable(${TARGET} ${ARGS_UNPARSED_ARGUMENTS})
    if (ARGS_ALIAS)
        add_executable(${ARGS_ALIAS} ALIAS ${TARGET})
    else()
        add_executable(${TARGET}::${TARGET} ALIAS ${TARGET})
    endif()
    __apply_common_target_options(${TARGET})   # Apply things like C++ Standard, preprocessor defines, etc.
endfunction()

function(battery_add_test TARGET)
    if (NOT BATTERY_GTEST_INCLUDED)
        set(gtest_force_shared_crt "Use shared (DLL) run-time lib even when Google Test is built as static lib." ON FORCE)
        battery_add_package("gh:google/googletest#main")      # Live at head: main branch recommended
        include(GoogleTest)
        set(BATTERY_GTEST_INCLUDED ON PARENT_SCOPE)
    endif()
    battery_add_executable(${TARGET} ${ARGN})
    target_link_libraries(${TARGET} gtest)
    gtest_discover_tests(${TARGET})
endfunction()





macro(battery_add_package)
    CPMAddPackage(${ARGN})
endmacro()

function(battery_set_output_name TARGET NAME)
    set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${NAME})
endfunction()

function(battery_set_exe_output_folder TARGET FOLDER)
    set_target_properties(${TARGET} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${FOLDER}"
    )
endfunction()

function(battery_set_ide_folder DIRECTORY LABEL)
    file(GLOB_RECURSE FILES "${DIRECTORY}/**")
    source_group(TREE "${DIRECTORY}" PREFIX ${LABEL} FILES ${FILES})
endfunction()

function(battery_precompile_headers TARGET FILE)
    target_precompile_headers(${TARGET} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${FILE}>")
endfunction()

function(battery_require_find_package PACKAGE_NAME PACKAGE_NAME_FOUND ADDITIONAL_MESSAGE)

    # Simply test the package and print understandable error message if it's not found
    find_package(${PACKAGE_NAME} QUIET)
    if (NOT ${PACKAGE_NAME_FOUND})
        message(FATAL_ERROR "${PACKAGE_NAME} development libraries not found! ${ADDITIONAL_MESSAGE}")
    endif()

endfunction()




function(battery_embed TARGET RESOURCE_FILE TYPE)

    if (NOT BATTERY_ROOT_DIR)
        message(FATAL_ERROR "battery_embed(): Battery root directory has not been set. Did you include the Battery library?")
    endif()

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
    set(TARGET_HEADER ${CMAKE_CURRENT_BINARY_DIR}/resources/${RESOURCE_FILENAME}.h)
    set(TARGET_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/resources/${RESOURCE_FILENAME}.cpp)

    add_custom_command(
            COMMAND ${TOOL_EXE} ${RESOURCE_FILE} ${TARGET_HEADER} --header ${BINARY}
            DEPENDS ${RESOURCE_FILE} battery::embed
            OUTPUT ${TARGET_HEADER})

    add_custom_command(
            COMMAND ${TOOL_EXE} ${RESOURCE_FILE} ${TARGET_SOURCE} --cpp ${BINARY}
            DEPENDS ${RESOURCE_FILE} battery::embed
            OUTPUT ${TARGET_SOURCE})

    # Imitate files so cmake can configure successfully
    target_sources(${TARGET} PUBLIC ${RESOURCE_FILE} ${TARGET_HEADER} ${TARGET_SOURCE})
    source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/" PREFIX "Resource Files" FILES ${TARGET_HEADER} ${TARGET_SOURCE})
    source_group(TREE "${RESOURCE_DIRECTORY}/" PREFIX "Resource Files" FILES ${RESOURCE_FILE})

    add_custom_target(${TARGET}-embed-${RESOURCE_FILENAME} DEPENDS battery::embed ${TARGET_HEADER} ${TARGET_SOURCE})
    add_dependencies(${TARGET} ${TARGET}-embed-${RESOURCE_FILENAME})
    set_target_properties(${TARGET}-embed-${RESOURCE_FILENAME} PROPERTIES FOLDER "${TARGET}-resources")

    # Where all headers and sources will be placed
    target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

endfunction()

