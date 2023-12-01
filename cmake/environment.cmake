
function(b_set_environment_definitions TARGET)
    if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        target_compile_definitions(${TARGET} PUBLIC B_ARCH_64)
    elseif (${CMAKE_SIZEOF_VOID_P} EQUAL 4)
        target_compile_definitions(${TARGET} PUBLIC B_ARCH_32)
    else()
        message(FATAL_ERROR "Unknown architecture")
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
        target_compile_definitions(${TARGET} PUBLIC B_OS_WEB)
    elseif (WIN32)
        target_compile_definitions(${TARGET} PUBLIC B_OS_WINDOWS)
    elseif (APPLE)
        target_compile_definitions(${TARGET} PUBLIC B_OS_MACOS)
    elseif (UNIX)
        target_compile_definitions(${TARGET} PUBLIC B_OS_LINUX)
    else()
        message(FATAL_ERROR "Unknown OS")
    endif()

    if (MSVC)
        target_compile_definitions(${TARGET} PUBLIC B_COMPILER_MSVC)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_definitions(${TARGET} PUBLIC B_COMPILER_CLANG)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_definitions(${TARGET} PUBLIC B_COMPILER_GCC)
    else()
        message(FATAL_ERROR "Unknown compiler")
    endif()

    target_compile_definitions(${TARGET} PUBLIC $<IF:$<CONFIG:Debug>,B_DEBUG,B_RELEASE>)

    if (B_PRODUCTION_MODE)
        target_compile_definitions(${TARGET} PUBLIC B_PRODUCTION_MODE)
    else()
        message(WARNING "Development mode is enabled, allowing hot-reloaded resources. Before releasing your app, "
                "make sure to switch to production mode using -DB_PRODUCTION_MODE=ON")
    endif()
endfunction()