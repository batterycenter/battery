
#include "battery/core/OsString.h"
#include "battery/core/common.h"
#include "battery/core/platform.h"

#ifdef BATTERY_ARCH_WINDOWS
#include "windows.h"
#else
#error ARCH NOT YET SUPPORTED
#endif

// TODO: Fix error and exception messages

// TODO: Remove circular dependency and prevent possible infinite loop

namespace battery {

#ifdef BATTERY_ARCH_WINDOWS
    static std::string WcharToUtf8(const std::wstring& wString);

    static std::wstring Utf8ToWchar(const std::string& mbString) {
        std::wstring buffer;

        if (mbString.empty())
            return buffer;

        if (mbString.length() > static_cast<size_t>((std::numeric_limits<int>::max)())) {
            throw std::overflow_error("Input string too long: size_t-length doesn't fit into int.");
        }

        constexpr DWORD kFlags = MB_ERR_INVALID_CHARS;
        int bufferSize = ::MultiByteToWideChar(
            CP_UTF8,                    // Source string is in UTF-8
            kFlags,                     // Conversion flags
            (LPCCH)mbString.data(),     // Source UTF-8 string pointer
            -1,                         // Length of the source UTF-8 string, in chars
            nullptr,                    // Unused - no conversion done in this step
            0                           // Request size of destination buffer, in wchar_ts
        );

        if (bufferSize == 0) {
            log::core::critical("[WinAPI] Utf8ToWchar failed: {}", get_last_win32_error());    // TODO: Handle error gracefully
            throw std::runtime_error(
                    "Cannot get result string length when converting from UTF-8 to UTF-16 (Utf8ToWchar failed).");
        }

        buffer.resize(bufferSize);
        int result = ::MultiByteToWideChar(
            CP_UTF8,                  // Source string is in UTF-8
            kFlags,                   // Conversion flags
            (LPCCH)mbString.data(),   // Source UTF-8 string pointer
            -1,                       // Length of source UTF-8 string, in chars
            &buffer[0],               // Pointer to destination buffer
			bufferSize                // Size of destination buffer, in wchar_ts
        );

        if (result == 0) {
            log::core::critical("[WinAPI] Utf8ToWchar failed: {}", get_last_win32_error());
            throw std::runtime_error("Cannot convert from UTF-8 to UTF-16 (Utf8ToWchar failed).");
        }
		
		return { buffer.c_str() };   // This re-builds the string and therefore exits at first null-terminator
    }

    static std::string WcharToUtf8(const std::wstring& wString) {
        std::string buffer;

        if (wString.empty())
            return buffer;

        if (wString.length() > static_cast<size_t>((std::numeric_limits<int>::max)())) {
            throw std::overflow_error("Input string too long: size_t-length doesn't fit into int.");
        }

        constexpr DWORD kFlags = WC_ERR_INVALID_CHARS;
        int bufferSize = ::WideCharToMultiByte(
            CP_UTF8,                    // Source string is in UTF-8
            kFlags,                     // Conversion flags
            (LPCWCH)wString.data(),     // Source UTF-8 string pointer
            -1,                         // Length of the source UTF-8 string, in chars
            nullptr,                    // Unused - no conversion done in this step
            0,                          // Request size of destination buffer, in wchar_ts,
            nullptr,
            nullptr
        );

        if (bufferSize == 0) {
            log::core::critical("[WinAPI] WcharToUtf8 failed: {}", get_last_win32_error());
            throw std::runtime_error(
                    "Cannot get result string length when converting from UTF-16 to UTF-8 (WcharToUtf8 failed).");
        }

        buffer.resize(bufferSize);
        int result = ::WideCharToMultiByte(
            CP_UTF8,                    // Source string is in UTF-8
            kFlags,                     // Conversion flags
            (LPCWCH)wString.data(),     // Source UTF-8 string pointer
            -1,                         // Length of source UTF-8 string, in chars
            &buffer[0],                 // Pointer to destination buffer
            bufferSize,                 // Size of destination buffer, in wchar_ts
            nullptr,
            nullptr
        );

        if (result == 0) {
            log::core::critical("[WinAPI] WcharToUtf8 failed: {}", get_last_win32_error());
            throw std::runtime_error("Cannot convert from UTF-16 to UTF-8 (WcharToUtf8 failed).");
        }

        return { buffer.c_str() };   // This re-builds the string and therefore exits at first null-terminator
    }

    OsString& OsString::operator=(const char* str) {
        return operator=(std::string(str));
    }

    OsString& OsString::operator=(const std::string& str) {
		raw_string = str.c_str();           // Re-build to intentionally end at first null-terminator
        platform_string = Utf8ToWchar(str);
        return *this;
    }

    OsString& OsString::operator=(const wchar_t* str) {
        return operator=(std::wstring(str));
    }

    OsString& OsString::operator=(const std::wstring& str) {
        raw_string = WcharToUtf8(str);
        platform_string = str.c_str();      // Re-build to intentionally end at first null-terminator
        return *this;
    }

    OsString::operator char*() const {
        return (char*)raw_string.c_str();
    }

    OsString::operator const char*() const {
        return raw_string.c_str();
    }

    OsString::operator std::string() const {
        return raw_string;
    }

    //OsString::operator wchar_t*() const {
    //    return (wchar_t*)platform_string.c_str();
    //}

    //OsString::operator const wchar_t*() const {
    //    return platform_string.c_str();
    //}

    //OsString::operator std::wstring() const {
    //    return platform_string;
    //}

#endif

}
