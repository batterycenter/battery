
#include "Battery/Utils/OsString.h"

#ifdef BATTERY_ARCH_WINDOWS
#include "windows.h"
#else
#error ARCH NOT YET SUPPORTED
#endif

namespace Battery {

#ifdef BATTERY_ARCH_WINDOWS
    std::string WcharToUtf8(const std::wstring& wString);

    static std::string GetLastWin32ErrorString() {

        DWORD errorMessageID = ::GetLastError();
        if (errorMessageID == 0)
            return "";

        LPWSTR messageBuffer = nullptr;
        size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                     NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

        std::wstring message(messageBuffer, size);
        LocalFree(messageBuffer);

        return WcharToUtf8(message);
    }

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
                kFlags,                       // Conversion flags
                (LPCCH)mbString.data(),     // Source UTF-8 string pointer
                -1,                     // Length of the source UTF-8 string, in chars
                nullptr,                  // Unused - no conversion done in this step
                0                          // Request size of destination buffer, in wchar_ts
        );

        if (bufferSize == 0) {
            std::cerr << "[WinAPI] Utf8ToWchar failed: " << GetLastWin32ErrorString() << std::endl;
            throw std::runtime_error(
                    "Cannot get result string length when converting from UTF-8 to UTF-16 (Utf8ToWchar failed).");
        }

        buffer.resize(bufferSize);
        int result = ::MultiByteToWideChar(
                CP_UTF8,       // Source string is in UTF-8
                kFlags,        // Conversion flags
                (LPCCH)mbString.data(),   // Source UTF-8 string pointer
                -1,    // Length of source UTF-8 string, in chars
                &buffer[0],     // Pointer to destination buffer
                bufferSize    // Size of destination buffer, in wchar_ts
        );

        if (result == 0) {
            std::cerr << "[WinAPI] Utf8ToWchar failed: " << GetLastWin32ErrorString() << std::endl;
            throw std::runtime_error("Cannot convert from UTF-8 to UTF-16 (Utf8ToWchar failed).");
        }

        return buffer;
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
                kFlags,                       // Conversion flags
                (LPCWCH)wString.data(),     // Source UTF-8 string pointer
                -1,                     // Length of the source UTF-8 string, in chars
                nullptr,                  // Unused - no conversion done in this step
                0,                          // Request size of destination buffer, in wchar_ts,
                nullptr,
                NULL
        );

        if (bufferSize == 0) {
            std::cerr << "[WinAPI] WcharToUtf8 failed: " << GetLastWin32ErrorString() << std::endl;
            throw std::runtime_error(
                    "Cannot get result string length when converting from UTF-16 to UTF-8 (WcharToUtf8 failed).");
        }

        buffer.resize(bufferSize);
        int result = ::WideCharToMultiByte(
                CP_UTF8,       // Source string is in UTF-8
                kFlags,        // Conversion flags
                (LPCWCH)wString.data(),   // Source UTF-8 string pointer
                -1,    // Length of source UTF-8 string, in chars
                &buffer[0],     // Pointer to destination buffer
                bufferSize,    // Size of destination buffer, in wchar_ts
                nullptr,
                NULL
        );

        if (result == 0) {
            std::cerr << "[WinAPI] WcharToUtf8 failed: " << GetLastWin32ErrorString() << std::endl;
            throw std::runtime_error("Cannot convert from UTF-16 to UTF-8 (WcharToUtf8 failed).");
        }

        return buffer;
    }

    OsString& OsString::operator=(const char* str) {
        return operator=(std::string(str));
    }

    OsString& OsString::operator=(const std::string& str) {
        raw_string = str;
        platform_string = Utf8ToWchar(str);
        return *this;
    }

    OsString& OsString::operator=(const wchar_t* str) {
        return operator=(std::wstring(str));
    }

    OsString& OsString::operator=(const std::wstring& str) {
        raw_string = WcharToUtf8(str);
        platform_string = str;
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
