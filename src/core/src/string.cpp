
#include "battery/core/string.h"
#include "battery/core/log.h"
#include "battery/core/extern/utf8proc.h"
#include "utf8.h"

#include <cstring>

namespace b {

    osstring to_osstring(const std::string& str) {
#ifdef BATTERY_ARCH_WINDOWS
        auto u16 = utf8::utf8to16(str);
        return std::wstring(std::bit_cast<wchar_t*>(u16.data()), u16.size());   // construct new from other
#else
        return str;
#endif
    }

    std::string from_osstring(const osstring& str) {
#ifdef BATTERY_ARCH_WINDOWS
        auto u16 = std::u16string(std::bit_cast<char16_t*>(str.data()), str.size());
        return utf8::utf16to8(u16);
#else
        return str;
#endif
    }

    std::u32string to_u32(const std::string& str) {
        try {
            return utf8::utf8to32(str);
        }
        catch (...) {
            throw std::invalid_argument("Invalid utf-8 sequence");
        }
    }

    std::string to_u8(const std::u32string& str) {
        try {
            return utf8::utf32to8(str);
        }
        catch (...) {
            throw std::invalid_argument("Invalid utf-8 sequence");
        }
    }

    std::string to_u8(char32_t c) {
        return to_u8(std::u32string(&c, 1));
    }

    std::string u8_foreach(const std::string& str, const std::function<std::variant<std::string,char32_t>(std::string)>& function) {
        std::string result;
        for (char32_t c : to_u32(str)) {
            auto v = function(to_u8(c));
            std::visit([&result]<typename T>(const T& s) {
                if constexpr(std::is_same_v<std::decay_t<T>,std::string>) {
                    result += s;
                }
                else {
                    result += to_u8(s);
                }
            }, v);
        }
        return result;
    }

    std::string u8_foreach(const std::string& str, const std::function<std::variant<std::string,char32_t>(char32_t)>& function) {
        std::string result;
        for (char32_t c : to_u32(str)) {
            auto v = function(c);
            std::visit([&result]<typename T>(const T& s) {
                if constexpr(std::is_same_v<std::decay_t<T>,std::string>) {
                    result += s;
                }
                else {
                    result += to_u8(s);
                }
            }, v);
        }
        return result;
    }

    std::vector<std::string> split(std::string str, const std::string& delimiter) {
        std::vector<std::string> result;
        size_t pos = 0;
        while ((pos = str.find(delimiter)) != std::string::npos) {
            result.emplace_back(str.substr(0, pos));
            str.erase(0, pos + delimiter.length());
        }
        result.emplace_back(str);
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

        size_t pos = 0;
        std::vector<size_t> occurrences;
        while ((pos = string.find(from, pos)) != std::string::npos) {
            occurrences.push_back(pos);
            pos += to.length();
        }

        if (occurrence >= 0 && occurrence >= static_cast<int>(occurrences.size())) {   // index too high
            return string;
        }
        if (occurrence < 0 && abs(occurrence) > occurrences.size()) {   // index too low
            return string;
        }
        size_t index = occurrence >= 0 ? occurrence : occurrences.size() - abs(occurrence);
        string.replace(occurrences[index], from.length(), to);
        return string;
    }

    std::string replace_first(const std::string& string, const std::string& from, const std::string& to) {
        return replace_one(string, from, to, 0);
    }

    std::string replace_last(const std::string& string, const std::string& from, const std::string& to) {
        return replace_one(string, from, to, -1);
    }

    std::string from_u8string(const std::u8string& str) {      // Simply copy the bytes as-is
        std::string target;
        target.resize(str.length());
        std::memcpy(target.data(), str.data(), str.length());
        return target;
    }

    std::u8string to_u8string(const std::string& str) {  // Check for validity and then copy the bytes as-is
        if (!utf8::is_valid(str)) {
            throw std::invalid_argument("Invalid utf-8 sequence");
        }
        std::u8string u8target;
        u8target.resize(str.length());
        std::memcpy(u8target.data(), str.data(), str.length());
        return u8target;
    }

    std::string to_lower(const std::string& str) {
        return u8_foreach(str, [] (char32_t c) {
            return (char32_t)utf8proc_tolower(c);
        });
    }

    std::string to_upper(const std::string& str) {
        return u8_foreach(str, [] (char32_t c) {
            if (to_u8(c) == "ß") {                // Special case which is not handled well in the library (at least not how we want)
                return std::string("SS");
            }
            return to_u8(utf8proc_toupper(c));
        });
    }

    static const std::string base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

    static inline uint8_t toint(std::byte b) {
        return std::to_integer<uint8_t>(b);
    }

    static inline bool is_base64(std::byte c) {
        return (isalnum(toint(c)) || (toint(c) == '+') || (toint(c) == '/'));
    }

    std::byte operator&(std::byte left, int right) {
        return static_cast<std::byte>(std::to_integer<int>(left) & right);
    }

    std::byte operator+(std::byte left, std::byte right) {
        return static_cast<std::byte>(toint(left) + toint(right));
    }

    std::string encode_base64(const std::string& str) {
        auto buf = std::bit_cast<const std::byte*>(str.data());
        size_t buffer_size = str.size();
        int i = 0;
        int j = 0;
        std::array<std::byte,3> char_array_3 {};
        std::array<std::byte,4> char_array_4 {};

        std::string result;
        while (buffer_size--) {
            char_array_3[i] = *(buf++);
            i++;
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;
                for (i = 0; i < 4; i++) result += base64_chars[toint(char_array_4[i])];
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 3; j++)
                char_array_3[j] = static_cast<std::byte>('\0');

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (j = 0; (j < i + 1); j++)
                result += base64_chars[toint(char_array_4[j])];

            while (i++ < 3)
                result += '=';
        }

        return result;
    }

    std::string decode_base64(const std::string& str) {
        auto buf = std::bit_cast<const std::byte*>(str.data());
        size_t buffer_size = str.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        std::array<std::byte,4> char_array_4 {};
        std::array<std::byte,3> char_array_3 {};
        std::string result;

        while (buffer_size-- && (static_cast<char>(buf[in_]) != '=') && is_base64(buf[in_])) {
            char_array_4[i] = buf[in_];
            i++;
            in_++;
            if (i == 4) {
                for (i = 0; i < 4; i++)
                    char_array_4[i] = static_cast<std::byte>(base64_chars.find(static_cast<char>(char_array_4[i])));

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    result.push_back(static_cast<char>(char_array_3[i]));
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 4; j++)
                char_array_4[j] = std::byte(0);

            for (j = 0; j < 4; j++)
                char_array_4[j] = static_cast<std::byte>(base64_chars.find(static_cast<char>(char_array_4[j])));

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++) result.push_back(static_cast<char>(char_array_3[j]));
        }
        return result;
    }

}
