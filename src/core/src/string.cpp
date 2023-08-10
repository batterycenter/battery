
#include "battery/core/string.hpp"
#include "battery/core/extern/utf8proc.h"
#include "battery/core/internal/windows.hpp"
#include "battery/core/log.hpp"
#include "utf8.h"
#include <regex>

namespace b {

    // ========================================================
    // =============== Begin b::string class ==================
    // ========================================================

    string::string(const std::u8string& other) {
        *this = decode_utf8(other);
    }

    string::string(const char8_t* other) {
        *this = decode_utf8(other);
    }

    string::string(const std::u16string& other) {
        *this = decode_utf16(other);
    }

    string::string(const char16_t* other) {
        *this = decode_utf16(other);
    }

    string::string(const std::u32string& other) {
        *this = decode_utf32(other);
    }

    string::string(const char32_t* other) {
        *this = decode_utf32(other);
    }

    string::string(char32_t chr) {
        *this = decode_utf32(chr);
    }

    string& string::operator=(const std::u8string& other) {
        *this = decode_utf8(other);
        return *this;
    }

    string& string::operator=(const char8_t* other) {
        *this = decode_utf8(other);
        return *this;
    }

    string& string::operator=(const std::u16string& other) {
        *this = decode_utf16(other);
        return *this;
    }

    string& string::operator=(const char16_t* other) {
        *this = decode_utf16(other);
        return *this;
    }

    string& string::operator=(const std::u32string& other) {
        *this = decode_utf32(other);
        return *this;
    }

    string& string::operator=(const char32_t* other) {
        *this = decode_utf32(other);
        return *this;
    }

    string& string::operator=(char32_t chr) {
        *this = decode_utf32(chr);
        return *this;
    }

    string& string::operator+=(const string& other) {
        return append(other);
    }

    string& string::assign(const string& other) {
        m_data = other.m_data;
        return *this;
    }

    string& string::append(const string& other) {
        m_data += other.m_data;
        return *this;
    }

    size_t string::size() const {
        return m_data.size();
    }

    size_t string::length() const {
        return m_data.length();
    }

    bool string::empty() const {
        return m_data.empty();
    }

    char32_t& string::back() {
        if (m_data.empty()) {
            throw std::out_of_range("b::string::front: String is empty");
        }
        return m_data.back();
    }

    const char32_t& string::back() const {
        if (m_data.empty()) {
            throw std::out_of_range("b::string::front: String is empty");
        }
        return m_data.back();
    }

    char32_t& string::front() {
        if (m_data.empty()) {
            throw std::out_of_range("b::string::front: String is empty");
        }
        return m_data.front();
    }

    const char32_t& string::front() const {
        if (m_data.empty()) {
            throw std::out_of_range("b::string::front: String is empty");
        }
        return m_data.front();
    }

    char32_t& string::at(size_t index) {
        if (index >= m_data.size()) {
            throw std::out_of_range("b::string::operator[]: Index out of range");
        }
        return m_data.at(index);
    }

    const char32_t& string::at(size_t index) const {
        if (index >= m_data.size()) {
            throw std::out_of_range("b::string::operator[]: Index out of range");
        }
        return m_data.at(index);
    }

    void string::push_back(char32_t chr) {
        m_data.push_back(chr);
    }

    char32_t string::pop_back() {
        if (m_data.length() == 0) {
            throw std::out_of_range("b::string::pop_back");
        }
        auto last = m_data.back();
        m_data.pop_back();
        return last;
    }

    void string::resize(size_t newSize) {
        m_data.resize(newSize);
    }

    void string::resize(size_t newSize, char32_t fill) {
        m_data.resize(newSize, fill);
    }

    size_t string::capacity() const {
        return m_data.capacity();
    }

    void string::reserve(size_t newSize) {
        m_data.reserve(newSize);
    }

    void string::clear() {
        m_data.clear();
    }

    void string::reverse() {
        std::reverse(m_data.begin(), m_data.end());
    }

    string string::reverse(const string& str) {
        auto result = str;
        result.reverse();
        return result;
    }

    string string::substr(size_t pos, size_t len) const {
        string result;
        result.m_data = m_data.substr(pos, len);
        return result;
    }

    bool string::is_valid_utf8(const std::string& str) {
        return utf8::is_valid(str);
    }

    string string::decode_utf8(const std::string& str) {
        if (!is_valid_utf8(str)) {
            throw b::unicode_error();
        }
        string result;
        result.m_data = utf8::utf8to32(str);
        return result;
    }

    string string::decode_utf8(const std::u8string& str) {
        auto u8str = std::string(std::bit_cast<char*>(str.data()), str.size());
        if (!is_valid_utf8(u8str)) {
            throw b::unicode_error();
        }
        string result;
        result.m_data = utf8::utf8to32(u8str);
        return result;
    }

    string string::decode_u8(const std::u8string& str) {
        return decode_utf8(str);
    }

    string string::decode_utf16(const std::u16string& str) {
        try {
            return decode_utf8(utf8::utf16to8(str));
        }
        catch (const std::exception&) {
            throw b::unicode_error();
        }
    }

    string string::decode_utf16(const std::wstring& str) {
        return decode_utf16(std::u16string(std::bit_cast<char16_t *>(str.data()), str.size()));
    }

    string string::decode_widestring(const std::wstring& str) {
        return decode_utf16(std::u16string(std::bit_cast<char16_t *>(str.data()), str.size()));
    }

    string string::decode_utf32(const std::u32string& str) {
        try {
            return decode_utf8(utf8::utf32to8(str));
        }
        catch (const std::exception&) {
            throw b::unicode_error();
        }
    }

    string string::decode_utf32(char32_t chr) {
        return decode_utf32(std::u32string(1, chr));
    }

    string string::decode_native(const b::native_string& str) {
#ifdef B_OS_WINDOWS
        return decode_widestring(str);
#else
        return from_utf8(str);
#endif
    }

    string string::decode_ascii(const std::string& str) {
        if (!is_valid_utf8(str)) {
            throw b::unicode_error();
        }
        string result;
        result.m_data = utf8::utf8to32(str);
        return result;
    }

    string string::decode_ascii(char chr) {
        return decode_ascii(std::string(1, chr));
    }

    string string::decode_latin1(const std::string& str) {
        std::string result;
        for (unsigned char const chr : str) {
            if (chr < 0x80u) {
                result.push_back(static_cast<char>(chr));
            }
            else {
                result.push_back(static_cast<char>(0xc0u | (chr >> 6u)));
                result.push_back(static_cast<char>(0x80u | (chr & 0x3fu)));
            }
        }
        return utf8::utf8to32(result);
    }

    string string::decode_latin1(char chr) {
        return decode_latin1(std::string(1, chr));
    }

    string string::decode_iso8859_1(const std::string& str) {
        return decode_latin1(str);
    }

    string string::decode_iso8859_1(char chr) {
        return decode_latin1(chr);
    }

#ifdef B_OS_WINDOWS
    string string::decode_windows1252(const std::string& str) {
        // In this function we load a string in Windows-1252 encoding. We do this by using
        // MultiByteToWideChar to turn it into common UTF-16 Windows encoding, and then
        // we can use the from_utf16 function to turn it into a b::string.

        if (str.empty()) {
            return {};
        }

        auto requiredBufferSize = MultiByteToWideChar(
                CP_ACP,                 // ANSI code page
                MB_ERR_INVALID_CHARS,   // Stop on invalid chars
                str.c_str(),            // Input string
                -1,                     // Length: Input string is null-terminated
                nullptr,                // Output buffer
                0                       // Output buffer size: 0 = return required buffer size without doing work
            );

        if (requiredBufferSize == 0) {
            throw b::unicode_error();
        }

        std::wstring buffer(requiredBufferSize, 0);
        auto result = MultiByteToWideChar(
                CP_ACP,                           // ANSI code page
                MB_ERR_INVALID_CHARS,             // Stop on invalid chars
                str.c_str(),                      // Input string
                -1,                               // Length: Input string is null-terminated
                buffer.data(),                    // Output buffer
                static_cast<int>(buffer.size())   // Output buffer size
        );

        if (result == 0) {
            throw b::unicode_error();
        }

        buffer.pop_back();      // Remove null terminator from the WinAPI
        return decode_utf16(buffer);
    }

    string string::decode_windows1252(char chr) {
        return decode_windows1252(std::string(1, chr));
    }
#endif

    std::string string::encode_utf8() const {
        return utf8::utf32to8(m_data);
    }

    std::u8string string::encode_u8() const {
        std::string str = encode_utf8();
        return { str.begin(), str.end() };
    }

    std::u16string string::encode_utf16() const {
        return utf8::utf8to16(utf8::utf32to8(m_data));
    }

    std::wstring string::encode_widestring() const {
        auto u16 = encode_utf16();
        return { std::bit_cast<wchar_t*>(u16.data()), u16.size() };
    }

    std::u32string string::encode_utf32() const {
        return m_data;
    }


    native_string string::encode_native() const {
#ifdef B_OS_WINDOWS
        return encode_widestring();
#else
        return to_utf8();
#endif
    }

    string::operator std::u8string() const {
        return encode_u8();
    }

    string::operator std::u16string() const {
        return encode_utf16();
    }

    string::operator std::u32string() const {
        return encode_utf32();
    }

    const std::u32string& string::str() const {
        return m_data;
    }

    std::vector<b::string> string::split(const b::string& delimiter) const {
        return split(*this, delimiter);
    }

    std::vector<b::string> string::split(const b::string& str, const b::string& delimiter) {
        auto data = str.m_data;
        size_t pos = 0;
        std::vector<b::string> result;
        while ((pos = data.find(delimiter.m_data)) != std::string::npos) {
            result.emplace_back(b::string::decode_utf32(data.substr(0, pos)));
            data.erase(0, pos + delimiter.m_data.length());
        }
        if (!data.empty()) {
            result.emplace_back(b::string::decode_utf32(data));
        }
        return result;
    }

    b::string string::join(const std::vector<b::string>& strings) {
        b::string result;
        for (const auto& token : strings) {
            result += token;
            if (!this->empty()) {
                result += *this;
            }
        }
        if (!result.empty()) {
            result.pop_back();
        }
        return result;
    }

    b::string string::join(const std::vector<b::string>& strings, const b::string& spacer) {
        b::string result;
        for (const auto& token : strings) {
            result += token;
            if (!spacer.empty()) {
                result += spacer;
            }
        }
        if (!result.empty()) {
            result.pop_back();
        }
        return result;
    }

    void string::regex_replace(const b::string& pattern,
                               const b::string& replace,
                               std::regex_constants::match_flag_type flags) {
        *this = regex_replace(*this, pattern, replace, flags);
    }

    b::string string::regex_replace(const b::string& str,
                                    const b::string& pattern,
                                    const b::string& replace,
                                    std::regex_constants::match_flag_type flags) {
        auto result = std::regex_replace(
                str.encode_widestring(),
                std::wregex(pattern.encode_widestring()),
                replace.encode_widestring(),
                flags
        );
        return b::string::decode_widestring(result);
    }

    std::vector<b::string> string::regex_match(const b::string& pattern,
                                               std::regex_constants::match_flag_type flags) {
        return regex_match(*this, pattern, flags);
    }

    std::vector<b::string> string::regex_match(const b::string& str,
                                               const b::string& pattern,
                                               std::regex_constants::match_flag_type flags) {
        std::vector<b::string> matches;
        std::wsmatch match;
        std::wstring data = str.encode_widestring();
        while (std::regex_search(data, match, std::wregex(pattern.encode_widestring()), flags)) {
            matches.emplace_back(b::string::decode_widestring(match.str(0)));
            data = match.suffix().str();
        }
        return matches;
    }

    void string::replace(const b::string& pattern, std::function<b::string(int)> replacer) {
        *this = replace(*this, pattern, replacer);
    }

    b::string string::replace(const b::string& str, const b::string& pattern,
                              std::function<b::string(int)> replacer) {
        std::u32string data = str.m_data;
        std::u32string buf;
        std::size_t pos = 0;
        std::size_t prevPos = 0;

        // Reserves rough estimate of final size of string.
        buf.reserve(str.size());

        int occurrence = 0;
        while (true) {
            prevPos = pos;
            pos = data.find(pattern, pos);
            if (pos == std::string::npos) {
                break;
            }
            buf.append(data, prevPos, pos - prevPos);
            buf += replacer(occurrence);
            pos += pattern.size();
            occurrence++;
        }

        buf.append(data, prevPos, data.size() - prevPos);
        return b::string::decode_utf32(buf);
    }

    void string::replace(const b::string& pattern, const b::string& value) {
        *this = replace(*this, pattern, value);
    }

    b::string string::replace(const b::string& str, const b::string& pattern, const b::string& value) {
        return replace(str, pattern, [&value](int) { return value; });
    }

    void string::replace_first(const b::string& pattern, const b::string& value) {
        *this = replace_first(*this, pattern, value);
    }

    b::string string::replace_first(const b::string& str, const b::string& pattern, const b::string& value) {
        return replace(str, pattern, [&value,&pattern](int occurrence) {
            return occurrence == 0 ? value : pattern;
        });
    }

    void string::replace_last(const b::string& pattern, const b::string& value) {
        *this = replace_last(*this, pattern, value);
    }

    b::string string::replace_last(const b::string& str, const b::string& pattern, const b::string& value) {
        auto tmp = str;
        tmp.reverse();
        tmp.replace_first(b::string::reverse(pattern), b::string::reverse(value));
        tmp.reverse();
        return tmp;
    }

    b::string& string::to_lower() {
        *this = to_lower(*this);
        return *this;
    }

    b::string string::to_lower(const b::string& str) {
        b::string result;
        for (const auto& chr : str.m_data) {
            result += static_cast<char32_t>(utf8proc_tolower(static_cast<int32_t>(chr)));
        }
        return result;
    }

    b::string& string::to_upper() {
        *this = to_upper(*this);
        return *this;
    }

    b::string string::to_upper(const b::string& str) {
        b::string result;
        for (const auto& chr : str.m_data) {
            if (chr == U'ß') {  // This special case is not handled well in the library (at least not how we want)
                result += U"SS";
                continue;
            }
            result += static_cast<char32_t>(utf8proc_toupper(static_cast<int32_t>(chr)));
        }
        return result;
    }

    // Operators

    char32_t& string::operator[](size_t index) {
        if (index >= m_data.size()) {
            throw std::out_of_range("b::string::operator[]: Index out of range");
        }
        return m_data[index];
    }

    const char32_t& string::operator[](size_t index) const {
        if (index >= m_data.size()) {
            throw std::out_of_range("b::string::operator[]: Index out of range");
        }
        return m_data[index];
    }

    string& string::operator+(const string& other) {
        m_data += other.m_data;
        return *this;
    }

    bool string::operator==(const string& other) const {
        return m_data == other.m_data;
    }

    bool string::operator!=(const string& other) const {
        return m_data != other.m_data;
    }

    [[nodiscard]] string::iterator string::begin() {
        return m_data.begin();
    }

    [[nodiscard]] string::const_iterator string::begin() const {
        return m_data.begin();
    }

    [[nodiscard]] string::iterator string::end() {
        return m_data.end();
    }

    [[nodiscard]] string::const_iterator string::end() const {
        return m_data.end();
    }

    [[nodiscard]] string::const_iterator string::cbegin() const {
        return m_data.cbegin();
    }

    [[nodiscard]] string::const_iterator string::cend() const {
        return m_data.cend();
    }

    [[nodiscard]] string::reverse_iterator string::rbegin() {
        return m_data.rbegin();
    }

    [[nodiscard]] string::const_reverse_iterator string::rbegin() const {
        return m_data.rbegin();
    }

    [[nodiscard]] string::reverse_iterator string::rend() {
        return m_data.rend();
    }

    [[nodiscard]] string::const_reverse_iterator string::rend() const {
        return m_data.rend();
    }

    [[nodiscard]] string::const_reverse_iterator string::crbegin() const {
        return m_data.crbegin();
    }

    [[nodiscard]] string::const_reverse_iterator string::crend() const {
        return m_data.crend();
    }

    // ========================================================
    // ================ End b::string class ===================
    // ========================================================





    // ========================================================
    // =============== Begin base-64 encoding =================
    // ========================================================

    static const char CHAR_TO_BASE64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static const char BASE64_TO_CHAR[] = "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                                         "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                                         "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x3E\xFF\xFF\xFF\x3F"
                                         "\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\xFF\xFF\xFF\xFF\xFF\xFF"
                                         "\xFF\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E"
                                         "\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\xFF\xFF\xFF\xFF\xFF"
                                         "\xFF\x1A\x1B\x1C\x1D\x1E\x1F\x20\x21\x22\x23\x24\x25\x26\x27\x28"
                                         "\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\xFF\xFF\xFF\xFF\xFF"
                                         "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                                         "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                                         "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x3E\xFF\xFF\xFF\x3F"
                                         "\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\xFF\xFF\xFF\xFF\xFF\xFF"
                                         "\xFF\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E"
                                         "\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\xFF\xFF\xFF\xFF\xFF"
                                         "\xFF\x1A\x1B\x1C\x1D\x1E\x1F\x20\x21\x22\x23\x24\x25\x26\x27\x28"
                                         "\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\xFF\xFF\xFF\xFF\xFF";

    b::string encode_base64(const b::string& str) {
        return encode_base64(str.encode_utf8());
    }

    b::string encode_base64(const b::bytearray& data) {
        b::string result;
        for (size_t index = 0; index < data.size(); index += 3) {
            size_t remaining = std::min<size_t>(data.size() - index, 3u); // 1, 2 or 3 bytes remaining each pass

            // Load 3 bytes (24 bits) of input data into the temporary buffer
            uint32_t buffer = data[index] << 16u;
            buffer |= remaining >= 2u ? data[index + 1] << 8u : 0u;
            buffer |= remaining >= 3u ? data[index + 2] << 0u : 0u;

            // Separate the 24 bits into 4 6-bit groups and append to the result (Add Padding character if needed)
            result += CHAR_TO_BASE64[(buffer & (63u << 18u)) >> 18u];
            result += CHAR_TO_BASE64[(buffer & (63u << 12u)) >> 12u];
            result += remaining >= 2u ? CHAR_TO_BASE64[(buffer & (63u << 6u)) >> 6u] : '=';
            result += remaining >= 3u ? CHAR_TO_BASE64[(buffer & 63u)] : '=';
        }
        return result;
    }

    b::string decode_base64_text(const b::string& str) {
        return b::string::decode_utf8(decode_base64_binary(str));
    }

    b::bytearray decode_base64_binary(const b::string& str) {
        std::vector<uint8_t> result;
        for (size_t index = 0; index < str.size(); index += 4) {
            size_t remaining = std::min<size_t>(str.size() - index, 4u); // 1, 2, 3 or 4 characters remaining each pass

            // Load 4 6-bit characters (24 bits) of input data into the temporary buffer
            uint32_t buf = 0;
            uint32_t shift = 18u;
            for (int i = 0; i < 4; i++) {
                if (i < remaining) {
                    auto chr = str[index + i];
                    uint8_t val = BASE64_TO_CHAR[chr];
                    if (val == 0xFF && chr != '=') return {};
                    buf |= chr != '=' ? (val << shift) : 0u;
                    shift -= 6;
                }
            }
            result.push_back((buf & (0xFFu << 16u)) >> 16u);
            if (remaining >= 3) if (str[index + 2] != '=') result.push_back((buf & (0xFFu << 8u)) >> 8u);
            if (remaining >= 4) if (str[index + 3] != '=') result.push_back(buf & 0xFFu);
        }
        return result;
    }

    // ========================================================
    // ================ End base-64 encoding ==================
    // ========================================================

} // namespace b

b::string operator""_b(const char* str, size_t len) {
    return b::string::decode_utf8(std::string(str, len));
}

b::string operator""_b(const char8_t* str, size_t len) {
    return b::string::decode_u8(std::u8string(str, len));
}

b::string operator""_b(const char16_t* str, size_t len) {
    return b::string::decode_utf16(std::u16string(str, len));
}

b::string operator""_b(const char32_t* str, size_t len) {
    return b::string::decode_utf32(std::u32string(str, len));
}