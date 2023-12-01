
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

get_filename_component(B_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
set(B_ROOT_DIR ${B_ROOT_DIR} CACHE INTERNAL "Root directory of the battery project" FORCE)
set(B_GTEST_INCLUDED OFF CACHE INTERNAL "Whether GoogleTest is included in the project" FORCE)

#if (ENABLE_CLANG_TIDY)
#    if (NOT DEFINED CLANG_TIDY_CONFIG_FILE)
#        set(CLANG_TIDY_CONFIG_FILE ${B_ROOT_DIR}/.clang-tidy CACHE INTERNAL "Path to the clang-tidy config file" FORCE)
#    endif()
#    execute_process(COMMAND clang-tidy --version OUTPUT_QUIET RESULT_VARIABLE CLANG_TIDY_RESULT)
#    if (NOT CLANG_TIDY_RESULT EQUAL 0)
#        if (WIN32)
#            if (MSVC)
#                message(FATAL_ERROR "clang-tidy.exe not found. Please install the LLVM toolset from 'https://github.com/llvm/llvm-project/releases/latest' (LLVM-...-win64.exe) and make sure clang-tidy.exe is available in the PATH. Otherwise set ENABLE_CLANG_TIDY to OFF to disable static code analysis (not recommended).")
#            else()
#                message(FATAL_ERROR "clang-tidy.exe not found. Please install clang-tidy manually or set ENABLE_CLANG_TIDY to OFF.")
#            endif()
#        else()
#            message(FATAL_ERROR "clang-tidy not found. Please install clang-tidy manually or set ENABLE_CLANG_TIDY to OFF.")
#        endif()
#    endif()
#endif()

#function(battery_test_if_compiles RESULT_VAR OUTPUT_VAR STD_VERSION SOURCE_CONTENT)
#    message(STATUS "Checking for compiler feature ${RESULT_VAR}")
#    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/try_compile/main.cpp ${SOURCE_CONTENT})
#    if (NOT MSVC)
#        set(pthread pthread)
#    endif()
#    try_compile(
#        ${RESULT_VAR}
#        ${CMAKE_CURRENT_BINARY_DIR}/try_compile/
#        ${CMAKE_CURRENT_BINARY_DIR}/try_compile/main.cpp
#        CXX_STANDARD ${STD_VERSION}
#        CXX_STANDARD_REQUIRED YES
#        CXX_EXTENSIONS NO
#        LINK_LIBRARIES ${pthread}
#        OUTPUT_VARIABLE _OUTPUT_VAR
#    )
#    set(${OUTPUT_VAR} ${_OUTPUT_VAR} PARENT_SCOPE)
#    file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/try_compile)
#    if (${RESULT_VAR})
#        message(STATUS "Checking for compiler feature ${RESULT_VAR} - Success")
#    else()
#        message(STATUS "Checking for compiler feature ${RESULT_VAR} - Failed")
#    endif()
#endfunction()

#function(battery_must_have_cxx20_feature FEATURE_NAME SOURCE_CONTENT)
#    if (${FEATURE_NAME})
#        return()
#    endif()
#    battery_test_if_compiles(${FEATURE_NAME} OUTPUT_VAR 20 ${SOURCE_CONTENT})
#    if (NOT ${FEATURE_NAME})
#        message(SEND_ERROR "Test ${FEATURE_NAME} Failed. Reason: Compiler output: ${OUTPUT_VAR}")
#        set(${FEATURE_NAME} OFF CACHE BOOL "" FORCE)
#    else()
#        set(${FEATURE_NAME} ON CACHE BOOL "" FORCE)
#    endif()
#    mark_as_advanced(${FEATURE_NAME})
#endfunction()

#function(battery_check_required_compiler_features)
#    battery_must_have_cxx20_feature(HAVE_JTHREAD
#        "#include <thread>
#        #include <iostream>
#        int main() {
#            std::jthread thread([] {
#                std::cout << \"Hello world\" << std::endl\\\;
#            })\\\;
#        }")
#endfunction()

function(b_apply_common_target_options TARGET)  # For all libraries and executables

    # Set the C++ Standard
    target_compile_features(${TARGET} PRIVATE cxx_std_23)
    set_target_properties(${TARGET} PROPERTIES CXX_EXTENSIONS OFF)

    if (ENABLE_CLANG_TIDY)
        if (MSVC)
            set(EXTRA_FLAGS ";--extra-arg=/EHsc")
        endif()
        set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY "clang-tidy;-config-file=${CLANG_TIDY_CONFIG_FILE}${EXTRA_FLAGS}")
    endif()

    # Set common Preprocessor Flags
    if(MSVC)        # TODO: Document all of these MSVC shitties
        target_compile_options(${TARGET} PRIVATE /Zc:__cplusplus)   # Use the standards-compliant __cplusplus macro
        target_compile_options(${TARGET} PRIVATE /Zc:preprocessor)  # Use the standards-compliant preprocessor instead of MSVC legacy mode
        target_compile_options(${TARGET} PRIVATE /MP)               # Use multiple cores for compilation
        target_compile_options(${TARGET} PRIVATE /W2)               # Warning level
        target_compile_options(${TARGET} PRIVATE /utf-8)            # Use UTF-8 source and execution character sets
    else()
        target_compile_options(${TARGET} PRIVATE -Wno-psabi -Wall -Wextra)
    endif()

    if (WIN32)
        target_compile_definitions(${TARGET} PRIVATE
                WIN32_LEAN_AND_MEAN         # Prevents Windows.h from adding unnecessary includes
                NOMINMAX                    # Prevents Windows.h from defining min/max as macros
                _CRT_SECURE_NO_WARNINGS     # Prevent MSVC from complaining about memset, strcpy, etc.
        )
    endif()
    target_include_directories(${TARGET} PUBLIC include)
    target_compile_definitions(${TARGET} PUBLIC UNICODE _UNICODE UTF8PROC_STATIC)
    b_set_environment_definitions(${TARGET})

endfunction()

function(b_add_alias_library target_name original_target)
    add_library(${target_name} ALIAS ${original_target})
endfunction()

function(b_add_alias_executable target_name original_target)
    add_executable(${target_name} ALIAS ${original_target})
endfunction()

function(b_add_msvc_header_files target_name)
    if (MSVC)
        file(GLOB_RECURSE headers ${CMAKE_CURRENT_LIST_DIR}/include/**)
        target_sources(${target_name} PRIVATE ${headers})
        source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "include" FILES ${headers})
    endif()
endfunction()

function(b_add_library TARGET TYPE)
    set(booleanArguments)
    set(singleValueArguments ALIAS)
    set(multiValueArguments TARGETS)
    cmake_parse_arguments(ARGS "${options}" "${singleValueArguments}" "${multiValueArguments}" ${ARGN})

    if (NOT TYPE STREQUAL "STATIC" AND NOT TYPE STREQUAL "SHARED")
        message(FATAL_ERROR "b_add_library expects a library type of either STATIC or SHARED, not '${TYPE}'")
    endif ()

    add_library(${TARGET} ${TYPE} ${ARGS_UNPARSED_ARGUMENTS})
    if (ARGS_ALIAS)
        add_library(${ARGS_ALIAS} ALIAS ${TARGET})
    else()
        add_library(${TARGET}::${TARGET} ALIAS ${TARGET})
    endif()
    b_apply_common_target_options(${TARGET})       # Apply things like C++ Standard, preprocessor defines, etc.
    b_add_msvc_header_files(${TARGET})
endfunction()

function(b_add_executable TARGET)
    set(booleanArguments EXCLUDE_FROM_ALL)
    set(singleValueArguments ALIAS)
    set(multiValueArguments TARGETS)
    cmake_parse_arguments(ARGS "${booleanArguments}" "${singleValueArguments}" "${multiValueArguments}" ${ARGN})

    if (ARGS_EXCLUDE_FROM_ALL)
        add_executable(${TARGET} EXCLUDE_FROM_ALL ${ARGS_UNPARSED_ARGUMENTS})
    else()
        add_executable(${TARGET} ${ARGS_UNPARSED_ARGUMENTS})
    endif()

    if (ARGS_ALIAS)
        add_executable(${ARGS_ALIAS} ALIAS ${TARGET})
    else()
        add_executable(${TARGET}::${TARGET} ALIAS ${TARGET})
    endif()

    file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/**")
    target_sources(${TARGET} PRIVATE ${HEADERS})
    source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADERS})

    b_set_exe_output_folder(${TARGET} ${CMAKE_BINARY_DIR}/bin/$<IF:$<CONFIG:Debug>,debug,release>/${TARGET})
    b_apply_common_target_options(${TARGET})   # Apply things like C++ Standard, preprocessor defines, etc.
    b_add_msvc_header_files(${TARGET})

    if (B_PRODUCTION_MODE AND WIN32)
        set_target_properties(${TARGET} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
    endif()
endfunction()

function(b_add_test TARGET)
    if (NOT B_GTEST_INCLUDED)
        set(gtest_force_shared_crt "Use shared (DLL) run-time lib even when Google Test is built as static lib." ON FORCE)
        set(CMAKE_DISABLE_FIND_PACKAGE_Python TRUE)

        include(FetchContent)
        FetchContent_Declare(
                googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG        main    # Live at head: main branch recommended
        )
        FetchContent_MakeAvailable(googletest)

        include(GoogleTest)
        set(B_GTEST_INCLUDED ON CACHE BOOL "Whether or not GoogleTest has been included" FORCE)
        b_set_target_ide_folder(gtest "battery/tests")
    endif()
    b_add_executable(${TARGET} ${ARGN})
    target_link_libraries(${TARGET} PUBLIC gtest)
    gtest_discover_tests(${TARGET})
    b_add_msvc_header_files(${TARGET})
endfunction()





# macro(battery_add_package)
#     CPMAddPackage(${ARGN})
# endmacro()

function(b_set_output_name TARGET NAME)
    set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${NAME})
endfunction()

function(b_set_target_ide_folder TARGET FOLDER)
    set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})
endfunction()

function(b_set_exe_output_folder TARGET FOLDER)
    set_target_properties(${TARGET} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${FOLDER}"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${FOLDER}"
            RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FOLDER}"
            RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${FOLDER}"
    )
endfunction()

function(b_set_ide_folder DIRECTORY LABEL)
    file(GLOB_RECURSE FILES "${DIRECTORY}/**")
    source_group(TREE "${DIRECTORY}" PREFIX ${LABEL} FILES ${FILES})
endfunction()

function(b_add_source_files TARGET)
    target_sources(${TARGET} PRIVATE ${ARGN})
    source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" PREFIX "src" FILES ${SOURCES})
endfunction()

function(b_precompile_headers TARGET FILE)
    get_filename_component(FILE ${FILE} ABSOLUTE)
    if (NOT EXISTS ${FILE})
        message(FATAL_ERROR "Precompiled header file ${FILE} does not exist!")
    endif()
    target_precompile_headers(${TARGET} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${FILE}>")
endfunction()

function(b_require_find_package PACKAGE_NAME PACKAGE_NAME_FOUND ADDITIONAL_MESSAGE)

    # Simply test the package and print understandable error message if it's not found
    find_package(${PACKAGE_NAME} QUIET)
    if (NOT ${PACKAGE_NAME_FOUND})
        message(FATAL_ERROR "${PACKAGE_NAME} development libraries not found! ${ADDITIONAL_MESSAGE}")
    endif()

endfunction()

function(b_icon TARGET ICON_FILE)
    # And we also automatically embed it
    b_embed(${TARGET} ${ICON_FILE})

    if (WIN32)
        get_filename_component(ICON_FILE ${ICON_FILE} ABSOLUTE)
        if (NOT EXISTS ${ICON_FILE})
            message(FATAL_ERROR "b_icon(): Icon file '${ICON_FILE}' does not exist!")
        endif()

        get_filename_component(ICON_NAME ${ICON_FILE} NAME_WLE)
        set(TARGET_FILE ${CMAKE_CURRENT_BINARY_DIR}/icons/${ICON_NAME}.ico)
        if (NOT EXISTS ${TARGET_FILE})
            file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/icons)
            execute_process(COMMAND
                    powershell -command
                    "& { . ${B_ROOT_DIR}/cmake/convert_ico_win32.ps1; ConvertTo-Icon -Path ${ICON_FILE} -Destination ${TARGET_FILE} }"
                    RESULT_VARIABLE RESULT
                    OUTPUT_VARIABLE OUTPUT
                )
            if (NOT RESULT EQUAL 0)
                message(FATAL_ERROR "b_icon(): Failed to convert icon file '${ICON_FILE}' to Windows icon file '${TARGET_FILE}'. Error: ${OUTPUT}")
            endif()
        endif()

        file(WRITE ${TARGET_FILE}.rc "IDI_ICON1 ICON DISCARDABLE \"${TARGET_FILE}\"")
        target_sources(${TARGET} PRIVATE ${TARGET_FILE}.rc)
    endif()
endfunction()

include(${CMAKE_CURRENT_LIST_DIR}/modules.cmake)        # This defines the module helper functions
include(${CMAKE_CURRENT_LIST_DIR}/environment.cmake)    # battery_add_environment_definitions()
