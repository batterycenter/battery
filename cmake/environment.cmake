
# All of these defines are set globally, making it available for any target that is compiled

if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    add_compile_definitions(B_ARCH_64)
elseif (${CMAKE_SIZEOF_VOID_P} EQUAL 4)
    add_compile_definitions(B_ARCH_32)
else()
    message(FATAL_ERROR "Unknown architecture")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    add_compile_definitions(B_OS_WEB)
elseif (WIN32)
    add_compile_definitions(B_OS_WINDOWS)
elseif (APPLE)
    add_compile_definitions(B_OS_MACOS)
elseif (UNIX)
    add_compile_definitions(B_OS_LINUX)
else()
    message(FATAL_ERROR "Unknown OS")
endif()

if (MSVC)
    add_compile_definitions(B_COMPILER_MSVC)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_definitions(B_COMPILER_CLANG)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_definitions(B_COMPILER_GCC)
else()
    message(FATAL_ERROR "Unknown compiler")
endif()

add_compile_definitions($<IF:$<CONFIG:Debug>,B_DEBUG,B_RELEASE>)

if (B_PRODUCTION_MODE)
    add_compile_definitions(B_PRODUCTION_MODE)
endif ()