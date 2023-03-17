
#include "battery/core/string.h"
#include "battery/core/log.h"
#include "battery/core/extern/utf8proc.h"
#include "utf8.h"

#include <cstring>
namespace battery::string {

#ifdef BATTERY_ARCH_WINDOWS
    static std::wstring utf8_to_wchar(const std::string_view& str) {
        std::u16string u16 = utf8::utf8to16(str);
        std::wstring wide(std::bit_cast<wchar_t*>(u16.data()), u16.size());
        return wide;
    }
    static std::string wchar_to_utf8(const std::wstring_view& str) {
        std::u16string u16(std::bit_cast<char16_t*>(str.data()), str.size());
        return utf8::utf16to8(u16);
    }
#endif

    osstring utf8_to_osstring(const std::string& str) {
#ifdef BATTERY_ARCH_WINDOWS
        return utf8_to_wchar(str);
#else
        return str;
#endif
    }

    std::string osstring_to_utf8(const osstring& str) {
#ifdef BATTERY_ARCH_WINDOWS
        return wchar_to_utf8(str);
#else
        return str;
#endif
    }

    std::u32string utf8_to_utf32(const std::string& str) {
        try {
            return utf8::utf8to32(str);
        }
        catch (...) {
            throw std::invalid_argument("Invalid utf-8 sequence");
        }
    }

    std::string utf32_to_utf8(const std::u32string& str) {
        try {
            return utf8::utf32to8(str);
        }
        catch (...) {
            throw std::invalid_argument("Invalid utf-8 sequence");
        }
    }

    std::string to_utf8(char32_t c) {
        return utf32_to_utf8(std::u32string(&c, 1));
    }

    std::string foreach(const std::string& str, std::function<std::variant<std::string,char32_t>(std::string)> function) {
        std::string result;
        for (char32_t c : utf8_to_utf32(str)) {
            auto v = function(to_utf8(c));
            std::visit([&result](const auto& s) {
                if constexpr(std::is_same_v<std::decay_t<decltype(s)>,std::string>) {
                    result += s;
                }
                else {
                    result += to_utf8(s);
                }
            }, v);
        }
        return result;
    }

    std::string foreach(const std::string& str, std::function<std::variant<std::string,char32_t>(char32_t)> function) {
        std::string result;
        for (char32_t c : utf8_to_utf32(str)) {
            auto v = function(c);
            std::visit([&result](const auto& s) {
                if constexpr(std::is_same_v<std::decay_t<decltype(s)>,std::string>) {
                    result += s;
                }
                else {
                    result += to_utf8(s);
                }
            }, v);
        }
        return result;
    }

    std::vector<std::string> split(const std::string& str, char delimeter) {
        std::string::size_type b = 0;
        std::vector<std::string> result;

        while ((b = str.find_first_not_of(delimeter, b)) != std::string::npos) {
            auto e = str.find_first_of(delimeter, b);
            result.push_back(str.substr(b, e - b));
            b = e;
        }

        return result;
    }

    std::string join(const std::vector<std::string>& strings, const std::string& spacer) {
        std::string str = "";

        for (size_t i = 0; i < strings.size(); i++) {
            str += strings[i];

            if (i < strings.size() - 1 && spacer != "") {
                str += spacer;
            }
        }

        return str;
    }

    std::string replace(std::string string, const std::string& from, const std::string& to) {
        if (from.empty())
            return string;
        size_t start_pos = 0;
        while ((start_pos = string.find(from, start_pos)) != std::string::npos) {
            string.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return string;
    }

    std::string replace_one(std::string string, const std::string& from, const std::string& to, [[maybe_unused]] int occurrence) {
        if (from.empty())
            return string;

        size_t start_pos = 0;
        while ((start_pos = string.find(from, start_pos)) != std::string::npos) {
            string.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return string;
    }

    std::string u8string_to_string(const std::u8string& str) {      // Simply copy the bytes as-is
        std::string target;
        target.resize(str.length());
        std::memcpy(target.data(), str.data(), str.length());
        return target;
    }

    std::u8string string_to_u8string(const std::string& str) {  // Check for validity and then copy the bytes as-is
        if (!utf8::is_valid(str)) {
            throw std::invalid_argument("Invalid utf-8 sequence");
        }
        std::u8string u8target;
        u8target.resize(str.length());
        std::memcpy(u8target.data(), str.data(), str.length());
        return u8target;
    }

    std::string to_lowercase(const std::string& str) {
        return string::foreach(str, [] (char32_t c) {
            return (char32_t)utf8proc_tolower(c);
        });
    }

    std::string to_uppercase(const std::string& str) {
        return string::foreach(str, [] (char32_t c) {
            if (to_utf8(c) == "ß") {                // Special case which is not handled well in the library (at least not how we want)
                return std::string("SS");
            }
            return to_utf8(utf8proc_toupper(c));
        });
    }

    // TODO: Support REGEX

    // TODO: Test base-64

    static const std::string base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

    static inline bool is_base64(uint8_t c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

    std::string encode_base64(const std::string& str) {
        const char* buf = str.data();
        size_t buffer_size = str.size();
        int i = 0;
        int j = 0;
        uint8_t char_array_3[3];
        uint8_t char_array_4[4];

        std::string ret;
        while (buffer_size--) {
            char_array_3[i++] = *(buf++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (i = 0; (i < 4); i++)
                    ret += base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (j = 0; (j < i + 1); j++)
                ret += base64_chars[char_array_4[j]];

            while ((i++ < 3))
                ret += '=';
        }

        return ret;
    }

    std::string decode_base64(const std::string& str) {
        const char* buf = str.data();
        size_t buffer_size = str.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        uint8_t char_array_4[4], char_array_3[3];
        std::string ret;

        while (buffer_size-- && (buf[in_] != '=') && is_base64(buf[in_])) {
            char_array_4[i++] = buf[in_]; in_++;
            if (i == 4) {
                for (i = 0; i < 4; i++)
                    char_array_4[i] = (uint8_t)base64_chars.find(char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    ret.push_back(char_array_3[i]);
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 4; j++)
                char_array_4[j] = 0;

            for (j = 0; j < 4; j++)
                char_array_4[j] = (uint8_t)base64_chars.find(char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
        }

        return ret;
    }

}
