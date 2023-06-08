
#include "battery/core/string.hpp"
#include "battery/core/log.hpp"
#include "battery/core/extern/utf8proc.h"
#include "utf8.h"

#include <cstring>

namespace b {

    bool string::is_valid_utf8(const std::string& str) {
        return utf8::is_valid(str);
    }

    std::u8string string::u8() const {
        if (!b::string::is_valid_utf8(*this)) throw std::invalid_argument("Invalid utf-8 sequence");
        return std::u8string(std::bit_cast<char8_t*>(data()), size());
    }

    std::u16string string::u16() const {
        if (!b::string::is_valid_utf8(*this)) throw std::invalid_argument("Invalid utf-8 sequence");
        return utf8::utf8to16(*this);
    }

    std::u32string string::u32() const {
        if (!b::string::is_valid_utf8(*this)) throw std::invalid_argument("Invalid utf-8 sequence");
        return utf8::utf8to32(*this);
    }

    const std::wstring& string::wstr() {
        std::u16string strU16 = u16();
        internal_wide_string = std::wstring(std::bit_cast<wchar_t*>(strU16.data()), strU16.size());
        return internal_wide_string;
    }

    const platform_native_string& string::platform_native() {
#ifdef B_OS_WINDOWS
        return wstr();
#else
        return str();
#endif
    }

    b::string string::foreach(const b::string& str, const std::function<b::string(b::string)>& function) {
        if (!b::string::is_valid_utf8(str)) throw std::invalid_argument("Invalid utf-8 sequence");
        b::string result;
        for (char32_t c : str.u32()) {
            result += function(b::string(c));
        }
        return result;
    }

    b::string string::foreach(const b::string& str, const std::function<b::string(char32_t)>& function) {
        if (!b::string::is_valid_utf8(str)) throw std::invalid_argument("Invalid utf-8 sequence");
        b::string result;
        for (char32_t c : str.u32()) {
            result += function(c);
        }
        return result;
    }

    std::vector<b::string> string::split(b::string str, const b::string& delimiter) {
        std::vector<b::string> result;
        size_t pos = 0;
        while ((pos = str.find(delimiter)) != b::string::npos) {
            result.emplace_back(str.substr(0, pos));
            str.erase(0, pos + delimiter.length());
        }
        result.emplace_back(str);
        return result;
    }

    b::string string::join(const std::vector<b::string>& strings, const b::string& spacer) {
        b::string result;
        for (size_t i = 0; i < strings.size(); i++) {
            result += strings[i];
            if (i < strings.size() - 1 && !spacer.empty()) {
                result += spacer;
            }
        }
        return result;
    }

    b::string string::replace(b::string string, const b::string& from, const b::string& to) {
        if (from.empty())
            return string;
        size_t start_pos = 0;
        while ((start_pos = string.find(from, start_pos)) != b::string::npos) {
            string.str().replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return string;
    }

    b::string string::replace_one(b::string string, const b::string& from, const b::string& to, int occurrence) {
        if (from.empty())
            return string;

        size_t pos = 0;
        std::vector<size_t> occurrences;
        while ((pos = string.find(from, pos)) != b::string::npos) {
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
        return string.str().replace(occurrences[index], from.length(), to.str());
    }

    b::string string::replace_first(const b::string &string, const b::string &from, const b::string &to) {
        return replace_one(string, from, to, 0);
    }

    b::string string::replace_last(const b::string &string, const b::string &from, const b::string &to) {
        return replace_one(string, from, to, -1);
    }

    b::string string::to_lower(const b::string& str) {
        return b::string::foreach(str, [](char32_t c) {
                return (char32_t)utf8proc_tolower(c);
        });
    }

    b::string string::to_upper(const b::string& str) {
        return b::string::foreach(str, [](char32_t c) {
            if (b::string(c) == "ß") {                // This special case which is not handled well in the library (at least not how we want)
                return b::string("SS");
            }
            return b::string((char32_t)utf8proc_toupper(c));
        });
    }

    std::string string::u8_to_str(const std::u8string& str) const {
        auto result = std::string(std::bit_cast<char*>(str.data()), str.size());
        if (!b::string::is_valid_utf8(result)) throw std::invalid_argument("Invalid utf-8 sequence");
        return result;
    }

    std::string string::u16_to_str(const std::u16string& str) const {
        auto result = utf8::utf16to8(str);
        if (!b::string::is_valid_utf8(result)) throw std::invalid_argument("Invalid utf-8 sequence");
        return result;
    }

    std::string string::u32_to_str(const std::u32string& str) const {
        auto result = utf8::utf32to8(str);
        if (!b::string::is_valid_utf8(result)) throw std::invalid_argument("Invalid utf-8 sequence");
        return result;
    }

    std::string string::wstr_to_str(const std::wstring& str) const {
        auto u16 = std::u16string(std::bit_cast<char16_t*>(str.data()), str.size());
        auto result = utf8::utf16to8(u16);
        if (!b::string::is_valid_utf8(result)) throw std::invalid_argument("Invalid utf-8 sequence");
        return result;
    }








    /// =====================================================================
    /// =====================================================================
    /// ========================= BASE64 ENCODING ===========================
    /// =====================================================================
    /// =====================================================================

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
