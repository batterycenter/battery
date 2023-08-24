
@page guidelines Battery Guidelines

# UTF-8 Everywhere

Battery strictly follows http://utf8everywhere.org/. This manifesto describes many concepts in detail, that greatly influenced decisions and guidelines documented on this page. Read it yourself for an in-depth primer on Unicode and String encodings.

## Unicode Terminology

https://learnmoderncpp.com/2021/03/24/a-unicode-primer/

 - ASCII is the most common character encoding. It uses 7 bits only and one byte is always one character. It is sometimes also called UTF-7.
 - Extended ASCII is not a real term and criticized as misleading, but is usually used to refer to Latin-1 or ISO-8859-1.
 - Latin-1 or ISO-8859-1 is a superset of ASCII and uses all 8 bits to provide more characters.
 - The word ANSI has no official meaning, but is usually used to refer to Windows Western 1252 enconding.
 - ANSI/Windows-1252 is a Microsoft-specific superset of Latin-1 and provides even more characters in 8 bits.
 - None of these are able to encode even the slightest percentage of all available characters world-wide.
 - Unicode is not an encoding, but a standard, that defines a code point for virtually any character in any language in the world, and is thus superior to all encodings listed so far. *UTF-8*, *UTF-16* and *UTF-32* are encodings that encode the *Unicode codepoints* in 1 to 4 bytes.
 - In UTF-32, every 4 bytes represent one *Unicode codepoint*. It is available as Big-Endian and Little-Endian.
 - In UTF-8, every *Unicode codepoint* is encoded in 1 to 4 bytes or *code units*. Multiple *code units* make up a *Codepoint*.
 - In UTF-16, every *Unicode codepoint* is encoded in 1 or 2 *16-bit code units*, which is 2 or 4 bytes correspondingly. Unicode characters from the *BMP (Basic Multilingual Plane)* use two bytes and are very common. Less common characters outside the BMP are called *Surrogates* and use 4 bytes.

# Programming Environment

 - Every source file must be edited using UTF-8 to ensure Unicode String literals work correctly. Most modern IDEs use it as their default anyways, except Visual Studio.

<details>
<summary>How to use UTF-8 source files in Visual Studio?</summary>
<p>Visual Studio will work fine with UTF-8, it just doesn't it is as a default when creating files. If you create a file within Visual Studio, open it in VS Code as Windows Western 1252 and save it as UTF-8 (without BOM). After that just continue editing like usual.</p>
</details>

# Working with Strings

 - Every `std::string` and `char*` is always considered to be UTF-8 encoded
 - Never, ever, do anything with ANSI encoding (also called Windows Western or cp1252). Stay far away from it. See below for why.
 - A *character* can always consist of multiple bytes. Be careful when storing single characters and be especially careful with the `char` datatype. Prefer `std::string`, even for single characters.
 - User-perceived characters on screen cannot be mapped to individual code points and do not need to be. Usually, the user-perceived characters are completely irrelevant for logic.
 - The string length is always noted in terms of bytes or code units used in memory. `str.length() ` != `number of characters`
 - There is no such thing as an ASCII-only string. Everything is considered to be UTF-8. If necessary, use Battery's utilities to strip non-ASCII characters. Only after that are you allowed to think of one byte as one *character*.
 - Never use wide strings or UTF-16 anywhere except directly adjacent to system APIs or Library APIs. See [Working with the Windows API](#working-with-the-windows-api)

<details>
<summary>More about Windows and ANSI</summary>
<p>Just as http://utf8everywhere.org/ states, this is not a religious war against Windows or Microsoft. In fact, Battery was for a long time Windows-only and is still primarily developed for Windows.</p>

<p>Microsoft is not to be blamed for inventing the ANSI encoding, CRLF or choosing UTF-16, because they were one of the first ones to see the challenge back then. We just think it is time to move on and that nowadays it is time to ban ANSI encoding from modern software development. See [utf8everywhere](http://utf8everywhere.org/) for explanations why this encoding is straight-up broken</p>
</details>

# File I/O

 - Every text file is always and everywhere read and written as UTF-8. Do not ever write files with other encodings.
 - Every text file is always read and written in binary mode, to preserve line endings.
 - Every text file is always written with LF line endings, even on Windows. See below for more.
 - Do not use BOMs, since anything is considered to be UTF-8 anyways and it makes things such as file concatenation a nightmare.

<details>
<summary>Why not CRLF on Windows?</summary>
<p>CRLF line endings have been around for a very long time and Microsoft chose them when typewriters were popular. Later, Unix, POSIX and all derivatives chose LF, because it does the job and the extra byte is simply unnecessary. The rest of the world settled on LF, only Windows is still using CRLF.</p>
<p>Actually, Windows deals with LF just fine these days. The only programs that have issues with CRLF are the built-in Windows programs themselves, like the original Notepad. But even that is changing. As of Windows 11, even the built-in Notepad already has proper LF and UTF-8 support. Thus, just write every file as LF and help preventing bugs by extincting CRLF.</p>
</details>

# Working with the Windows API

 - Try not to do it in the first place. Using it makes your code platform dependent. Use the functions provided by Battery instead or create a GitHub issue to request a new feature. If you still insist on using it, do the following:
 - Never use ANSI-Win32 functions (e.g. `CreateFileA`).
 - Never use the Win32 function macros (e.g. `CreateFile`)
 - Always use the Unicode variant explicitly (e.g. `CreateFileW`)
 - Use `std::wstring` or `L""` string literals to represent UTF-16, needed by the Windows API
 - Never use `std::wstring` or `L""` string literals anywhere else except directly adjacent to system APIs.
 - Always widen UTF-8 strings directly adjacent to system calls (e.g. `MessageBoxW(...,b::widen(str).c_str(),...)`), and when receiving strings, do the opposite (e.g. `return b::narrow(wideStringBufferFromAPI);`). This only applies when the string is immediately processed. See below for what to do when the string must live longer.

<details>
<summary>My string must live longer than one function call</summary>
<p>For some WinAPI functions, the string must live longer than one line of code. This might be the case when a string is psrt of a struct, that is then passed to a function for processing (e.g. `RegisterClassExW` and the corresponding `WNDCLASSW` struct). 

If you write `wndClass.lpClassName = b::widen(str).c_str();`, the string will immediately be destroyed after this line, leaving the pointer pointing to invalid memory. Therefore, save the string as a separate variable (e.g. `auto wideStr = b::widen(str);`. This variable must live at least until it has been processed by the system. Now you can write `wndClass.lpClassName = wideStr.c_str();` and it will work correctly.</p>
</details>

















## Working with Unicode

Following online tools can be used for working with Unicode. Be very cautious of the terminology used.

In the following websites, *unicode* is equivalent to *utf-8*.  
Be aware that you might have to set the base. If there is no dropdown menu for hexadecimal, set the base to 16 if your input is hexadecimal bytes.  

Convert a *Unicode string* to their corresponding *Unicode codepoints*: https://onlineunicodetools.com/convert-unicode-to-code-points  

Convert a *Unicode string* to their *utf-8 encoded bytes*: https://onlineunicodetools.com/convert-unicode-to-utf8  

Convert *utf-8 encoded bytes* to a *Unicode string* (set base to 16 for hexadecimal bytes): https://onlineutf8tools.com/convert-bytes-to-utf8

Many other converters are available on this site. Be careful of what exactly is encoded in which format. It's often not obvious.

## Source files

To make `UTF-8` encoded string literals work, you must also save your source files as `UTF-8`. This is the default in most modern IDE's like Visual Studio Code or Jetbrains CLion.

You can usually select the file encoding in the bottom right corner, it should always be set to `UTF-8` for every file you work with. This is necessary so that your compiler interprets the source file correctly, otherwise the string literals means something different to the compiler.
CMake additionally adds a flag to parse every source file as `UTF-8`.

Be aware that *u8 string literals* do not like byte-encoding.
Never write `"\x23\x54\xfc"`-style strings, always use Unicode codepoints, like this: `"\uFC34"`.

## WinAPI / Linux API {#WIN_LINUX_API}

You are not encouraged to use the Windows API or the Linux API in any way. They are huge, unportable between OSes and you probably write a few thousand bugs when using these directly. Almost anything you want to do is probably implemented in the C++ Standard library or is available as a third-party cross-platform library. In such a case, use it.

In the very rare circumstances that you still have to use the WinAPI or the Linux API directly, because there is no other way, there are a few things to keep in mind.

### WinAPI: Do not use ANSI functions!

In the Win32 API, almost every function and data structure has two variants: e.g. `CreateWindowsA()` and `CreateWindowsW()`. The `...A()`-function is the ANSI function, forget that and never use it! There is no way it can support Unicode.

The `...W()`-functions on the other hand, are the WideString variants, or `UTF-16`. These are the ones you want to use. You have to convert your `UTF-8` string to `UTF-16` for the function call, and if you get something back, you convert it from `UTF-16` back to `UTF-8` immediately. See section [OsString](#OsString). 

Just make sure to never use `std::wstring` or `wchar_t` manually and that all your logic stays in the `UTF-8` world. No ANSI, no ASCII, no `UTF-16`.

### Use OsString whenever applicable {#OsString}

For converting between `UTF-8` and `UTF-16`, there is a very handy class, called `Battery::OsString`. It is inspired by the `OsString` struct, supported natively in the [Rust Programming Language](https://doc.rust-lang.org/std/ffi/struct.OsString.html).

It is basically a string class, but it converts your string encoding automatically. Every `std::string` and `char*` is interpreted as a `UTF-8` string, while a `UTF-16` string is stored in an `std::wstring` or `wchar_t*`. A `wchar_t` is 16 bits wide on Windows. On Linux, an `std::wstring` and a `wchar_t*` are equivalent to a conventional string.

Here is an example how you could use the `OsString` class, while using the Win32 API:

```cpp
#include <windows.h>

std::string title = "Important message";
std::string message = "This is a UTF-8 message! Σ Φ μ π 乪 乫 乬";
MessageBoxW(nullptr,                                        // Notice how we use the ...W() variation
            Battery::OsString(message).wstr().c_str(),
            Battery::OsString(title).wstr().c_str(),
			MB_ICONERROR);
```

What happens here is that we create an `OsString` instance with our string and it gets converted automatically. The call to `.wstr()` returns the string as a `std::wstring`, encoded as `UTF-16`. Finally, `.c_str()` gives us the raw C-pointer in the form of a `wchar_t*`, which is what the Windows API wants.

You do not have to write this of course, Battery offers you a cross-platform MessageBox function.

Be aware that the data below `c_str()` is only valid as long as the `std::wstring` object is alive.

And here is an example on how to do it the other way around:
```cpp
LPWSTR messageBuffer = nullptr;                             // C-sytle wide-character string (UTF-16)
size_t size = FormatMessageW(/* ... */ (LPWSTR)&messageBuffer /* ... */);

std::wstring message(messageBuffer, size);
LocalFree(messageBuffer);

std::string str = OsString(message);
```

In this case, `FormatMessageW` takes a pointer to our pointer, allocates a buffer and writes the pointer to our pointer. Then we construct a `std::wstring` from the buffer, free the buffer and then convert our `wstring` to a `std::string`. Now it is `UTF-8` and we can happily use it whereever we want.

Just make sure that `wstring`, `wchar_t`, `LPWSTR` and so on are only used directly on the WinAPI and nowhere else.

### Using high-level containers

The Linux API doesn't have the `UTF-8` problems, it only gives you headache because it is so complicated and so easy to write bugs and memory leaks. Besides from the fact that it makes your code unportable, of course.

If you write code using any of these APIs, you should make sure to always take the time and read the documentation on the [Linux man pages](https://linux.die.net/man/) or the [Microsoft Docs](https://learn.microsoft.com/en-us/windows/win32/api/). The best way is to search the internet, find out what functions to use, and then look out for the documentation of the needed functions on these two documentation pages.

The most important things to look out for in the function documentation are return values, error codes and exceptions, but most importantly: constraints on input parameters and if they allocate or free the memory you pass to it.

Once you know exactly how to deal with this function, you can apply high-level containers to make it safe. Most importantly `std::string` and `std::vector`. Many API calls need arrays of some sort and every kind of array can be implemented with an `std::vector`, instead of using `new` and `delete`. This will result in very safe code and the memory will be freed automatically. One example of using a high-level container directly with the API can be seen in [OsString](#OsString).

### Keeping WinAPI / Linux API cross-platform

The primary goal of Battery is to unify your code to compile on any platform. If you use the WinAPI or Linux API you prevent this of course, and it will only compile on one platform. Here is a guide on how you can make it cross-platform again, yourself.

Battery has a few `define`s which help you with this:

```cpp
#ifdef B_OS_WINDOWS     // If the platform is Windows
#ifdef B_OS_LINUX       // If the platform some flavour of Linux
//#ifdef B_OS_MACOS     // MacOS is not officially supported yet, but planned

#ifdef B_ARCH_64          // Platform is 64 bit
#ifdef B_ARCH_32          // Platform is 32 bit

#ifdef B_COMPILER_MSVC    // Compiler is Microsoft Visual C++ (Visual Studio)
#ifdef B_COMPILER_GCC     // Compiler is GNU Compiler Collection (or MinGW)
#ifdef B_COMPILER_CLANG   // Compiler is Clang

#ifdef B_DEBUG            // Debug build
#ifdef B_RELEASE          // Release build

#ifdef B_PRODUCTION_MODE  // Production mode
```

All of these blocks are guaranteed to be mutually exclusive.

```cpp
void foo(const std::string& bar) {
#if defined(B_OS_WINDOWS)
    // Do some Windows-specific work
    // ...
#elif defined(B_OS_LINUX)
    // Do some Linux-specific work
    // ...
//#elif defined(B_OS_MACOS)             // MacOS is not supported yet, but planned
    // Do some Apple-specific work
    // ...
#endif              // No else needed, because the defines are mutually exclusive and guaranteed to be one of them
    return bar;
}
```

This is how you write functions that use OS-specific features without sacrificing cross-platform availability.




# Lockfiles

Having multiple instances of your process is a great thing, but sometimes you need to make sure that only one instance can ever be running. This is what Lockfiles are good for. You might use them if you want to make sure that exactly one process can run and not a second, or if you have several processes that do critical work and one process doing critical work must block all others until it is finished.

You know this behaviour when you are on Linux, and call `apt` in two different terminals: The first terminal will run, and the second will give you an error like: "Cannot lock frontend, lockfile failed" or something similar. This is because the first process locked a file and the second cannot lock it again. It has to wait until the first one unlocks it again and then the second process can lock it and do critical work.

## Different types

Especially in more modern and more high-level programming languages, there are Fake-Lockfiles. This can also be found for C++ sometimes. Such Fake-Lockfiles simply create a file and the resource is 'locked' when the file exists. The resource is unlocked again when deleting the file. The problem with that is that the file remains when the process crashes or is stopped unexpectedly (PC shutdown). As a result the file remains and there is an infinite lock. You know this behaviour from tools where you must sometimes go into a folder, delete a file called `data.lock` for example, and then it works again.

Battery implements real Lockfiles: The difference is that the lock is not retrieved by creating a file, it is retrieved by opening the file in exclusive write mode. This means that in this case the file always remains and the first process opens it in exclusive write mode, which lock out anyone else. Nothing is written to the file, it is just kept open.

The advantage with this is that when the process stops or crashes, the lock is dropped immediately by the operating system. It is much closer to the process needing a resource: If a process does not exist anymore, it does not need the resource anymore.

The only disadvantage with this is that this operation might not be supported on networked locations. If you want a local Hard-Lock that is tied to a very specific process, use the hard lock option. If you want a Soft-Lock that can also work across network drives and is not tied to a specific process, use the soft lock option. Just be aware of the infinite lockup problem, when your process stops unexpectedly and then restarts.








# OsString -> Always terminates at '\0'


    auto path = std::filesystem::current_path(); 	// get path
    std::filesystem::current_path(path); 			// set path

add_custom_command() PRE_BUILD is Visual Studio only !!!!!!

WinAPI docs: 
WinAPI docs: Do not consider examples, they are outdated!

`std::source_location::current()` instead of __FUNC__ macros