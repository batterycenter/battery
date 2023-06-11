
#ifndef UNICODE
#error UNICODE is not defined. Please check your build system!
#endif

#ifndef _UNICODE
#error _UNICODE is not defined. Please check your build system!
#endif

#if !defined(B_OS_WINDOWS) && !defined(B_OS_LINUX) && !defined(B_OS_MACOS) && !defined(B_OS_WEB)
#error No platform defined. Please check your build system!
#endif

#if !defined(B_COMPILER_MSVC) && !defined(B_COMPILER_GCC) && !defined(B_COMPILER_CLANG)
#error No compiler defined. Please check your build system!
#endif

#if !defined(B_ARCH_32) && !defined(B_ARCH_64)
#error No architecture defined. Please check your build system!
#endif

#if !defined(B_DEBUG) && !defined(B_RELEASE)
#error No build type defined. Please check your build system!
#endif
