#pragma once

#include <string>

namespace battery {

    class OsString {
    public:
        OsString() {}
        OsString(const char* str) { operator=(str); }
        OsString(const std::string& str) { operator=(str); }
        OsString(const wchar_t* str) { operator=(str); }
        OsString(const std::wstring& str) { operator=(str); }

        // Taking a UTF-8 string
        OsString& operator=(const char* str);
        OsString& operator=(const std::string& str);

        // Taking a platform string
        OsString& operator=(const wchar_t* str);
        OsString& operator=(const std::wstring& str);

        std::string str() const { return raw_string; }
        const char* c_str() const { return raw_string.c_str(); }
        std::wstring wstr() const { return platform_string; }
        const wchar_t* c_wstr() const { return platform_string.c_str(); }

        operator char*() const;
        operator const char*() const;
        operator std::string() const;
        //operator wchar_t*() const;
        //operator const wchar_t*() const;
        //operator std::wstring() const;

    private:
        std::string raw_string;
        std::wstring platform_string;
    };

}
