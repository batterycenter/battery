# Battery Concepts

Battery provides you with many concepts that make your life easier: You do not have to stay inline with them,
but if you do, you will have a much better time.

Many following concepts are explained in detail, why they cause headache and how you can use Battery to 
prevent that headache.

## Unicode (UTF-8)

Unicode support is a very broad topic and causes a lot of headache, especially on Windows. Due to the cross-platform
nature of Battery, your code is supposed to run on any computer, be it Windows, Mac or Linux.

### What is Unicode?

First, you should be familiar with ASCII. 

The problem with ASCII is that you cannot represent any character outside of the ASCII set. Greetings to all the germans out there, using programs going back to the Windows XP days, that crash as soon as you save a file containing an Umlaut like Ä, Ö, Ü :) 

We do not want this, we want a modern, robust application. Unicode solves this problem, it is known as a Multibyte language and can therefore store basically any character in the world, by using more than one bytes per character.

There are multiple Unicode encodings out there, but we want to use `UTF-8`, as enforced by http://utf8everywhere.org.

According to that, we want to use `UTF-8` encoded strings everywhere in our application. The most important thing to keep in mind is that a character is no longer a single byte. `char`s cannot be used anymore to store a character when we want to support Unicode. In this case a character is always a string with at least one byte, maybe more.

### The Problem

The problem comes with Windows, it is quite old and uses `UTF-16` for the WinAPI, as well as an ANSI version. See [WinAPI / Linux API](#WIN_LINUX_API) for more information.

What is important for us here is that Windows does not use `UTF-8`, which is the reason why you must use Battery's tools for everthing. Battery takes great care for you that everything works out of the box, especially for things related to `UTF-8`.

This is not a problem on Linux and Unix-like systems, as they are built on top of `UTF-8`, all the way through. You could think of it as unifying the application logic to always use `UTF-8` on every system, and on Windows you only convert to `UTF-16` when talking to Windows directly.

## Terminology

https://learnmoderncpp.com/2021/03/24/a-unicode-primer/

 - ASCII is the most common character encoding, understood by everyone. It uses 7 bits only and one byte is one character.
 - Extended ASCII adds more characters to the ASCII set and uses all 8 bits. It is not compatible with UTF-8.
 - Unicode is the high-level definition of the character set. It assigns every character of the *Unicode character set* to a 32-bit *Unicode codepoint*. Then, a *Unicode codepoint* can be encoded in multiple ways, e.g. *UTF-8*, *UTF-16*, ...
 - UTF-32 is the one-to-one encoding of *Unicode*, as every *codepoint* fits in 32 bits. Every character of a UTF-32 string uses 32 bits, always.
 - UTF-8 is the most common encoding for *Unicode*, every character uses 1 to 4 bytes. Common ones use 1, less common ones use more. The most common ones only use a single byte and are equivalent to *(plain) ASCII*.
 - UTF-16 follows the same rules as UTF-8, just that every character uses 1 or 2 *16-bit code units*, in other words 2 or 4 bytes, depending on the *Unicode codepoint* being encoded.
 - UTF-7 is sometimes used to refer to *plain ASCII*. It is like UTF-8, but only single-byte characters are allowed. Thus, *UTF-7* is equivalent to *plain ASCII*.

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
#ifdef BATTERY_ARCH_WINDOWS     // If the platform is Windows
#ifdef BATTERY_ARCH_LINUX       // If the platform some flavour of Linux
                                // MacOS is not supported yet, but planned

#ifdef BATTERY_ARCH_64          // Platform is 64 bit (Win or Linux)
#ifdef BATTERY_ARCH_32          // Platform is 32 bit (Win or Linux)
```

There are many other defines, see `include/Battery/Environment.h`.

```cpp
void foo(const std::string& bar) {
#if defined(BATTERY_ARCH_WINDOWS)
    // Do some Windows-specific work
    // ...
#elif defined(BATTERY_ARCH_LINUX)
    // Do some Linux-specific work
    // ...
#else               // MacOS is not supported yet, but planned
    #error Unsupported architecture!
#endif
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