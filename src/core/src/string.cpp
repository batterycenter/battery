
#include "battery/core/exception.hpp"
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
        *this = decode<b::enc::utf8>(other);
    }

    string::string(const char8_t* other) {
        *this = decode<b::enc::utf8>(std::u8string(other));
    }

    string::string(const std::u16string& other) {
        *this = decode<b::enc::utf16>(other);
    }

    string::string(const char16_t* other) {
        *this = decode<b::enc::utf16>(other);
    }

    string::string(const std::u32string& other) {
        *this = decode<b::enc::utf32>(other);
    }

    string::string(const char32_t* other) {
        *this = decode<b::enc::utf32>(other);
    }

    string::string(char32_t chr) {
        *this = decode<b::enc::utf32>(chr);
    }

    string& string::operator=(const std::u8string& other) {
        *this = decode<b::enc::utf8>(other);
        return *this;
    }

    string& string::operator=(const char8_t* other) {
        *this = decode<b::enc::utf8>(std::u8string(other));
        return *this;
    }

    string& string::operator=(const std::u16string& other) {
        *this = decode<b::enc::utf16>(other);
        return *this;
    }

    string& string::operator=(const char16_t* other) {
        *this = decode<b::enc::utf16>(other);
        return *this;
    }

    string& string::operator=(const std::u32string& other) {
        *this = decode<b::enc::utf32>(other);
        return *this;
    }

    string& string::operator=(const char32_t* other) {
        *this = decode<b::enc::utf32>(other);
        return *this;
    }

    string& string::operator=(char32_t chr) {
        *this = decode<b::enc::utf32>(chr);
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
            throw b::out_of_range_error("b::string::front: String is empty");
        }
        return m_data.back();
    }

    const char32_t& string::back() const {
        if (m_data.empty()) {
            throw b::out_of_range_error("b::string::front: String is empty");
        }
        return m_data.back();
    }

    char32_t& string::front() {
        if (m_data.empty()) {
            throw b::out_of_range_error("b::string::front: String is empty");
        }
        return m_data.front();
    }

    const char32_t& string::front() const {
        if (m_data.empty()) {
            throw b::out_of_range_error("b::string::front: String is empty");
        }
        return m_data.front();
    }

    char32_t& string::at(size_t index) {
        if (index >= m_data.size()) {
            throw b::out_of_range_error("b::string::operator[]: Index out of range");
        }
        return m_data.at(index);
    }

    const char32_t& string::at(size_t index) const {
        if (index >= m_data.size()) {
            throw b::out_of_range_error("b::string::operator[]: Index out of range");
        }
        return m_data.at(index);
    }

    void string::push_back(char32_t chr) {
        m_data.push_back(chr);
    }

    char32_t string::pop_back() {
        if (m_data.length() == 0) {
            throw b::out_of_range_error("b::string::pop_back");
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
        if (pos >= m_data.size()) {
            throw b::out_of_range_error("b::string::substr: Start position out of range");
        }
        string result;
        result.m_data = m_data.substr(pos, len);
        return result;
    }

    bool string::is_valid_utf8(const std::string& str) {
        return utf8::is_valid(str);
    }

    string string::internal::decode_utf8(const std::string& str) {
        if (!is_valid_utf8(str)) {
            throw b::unicode_error();
        }
        string result;
        result.m_data = utf8::utf8to32(str);
        return result;
    }

    string string::internal::decode_utf16(const std::u16string& str) {
        try {
            return decode_utf8(utf8::utf16to8(str));
        }
        catch (const std::exception&) {
            throw b::unicode_error();
        }
    }

    string string::internal::decode_utf32(const std::u32string& str) {
        try {
            return decode_utf8(utf8::utf32to8(str));
        }
        catch (const std::exception&) {
            throw b::unicode_error();
        }
    }

    string string::internal::decode_ascii(const std::string& str) {
        return decode_utf8(str);
    }

    string string::internal::decode_iso8859_1(const std::string& str) {
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

#ifdef B_OS_WINDOWS
    string string::internal::decode_cp1252(const std::string& str) {
        // In this function we load a string in Windows-1252 encoding. We do this by using
        // MultiByteToWideChar to turn it into common UTF-16 Windows encoding, and then
        // we can use the decode_utf16 function to turn it into a b::string.
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
        auto charsWritten = MultiByteToWideChar(
                CP_ACP,                           // ANSI code page
                MB_ERR_INVALID_CHARS,             // Stop on invalid chars
                str.c_str(),                      // Input string
                -1,                               // Length: Input string is null-terminated
                buffer.data(),                    // Output buffer
                static_cast<int>(buffer.size())   // Output buffer size
        );

        if (charsWritten == 0) {
            throw b::unicode_error();
        }
        buffer.resize(charsWritten - 1);    // Remove null terminator
        return decode_utf16(std::u16string(buffer.begin(), buffer.end()));
    }
#endif

    b::bytearray string::internal::encode_utf8(const b::string& str) {
        return b::bytearray::from_string(utf8::utf32to8(str.m_data));
    }

    b::bytearray string::internal::encode_utf16(const b::string& str) {
        auto u16 = utf8::utf8to16(utf8::utf32to8(str.m_data));
        return b::bytearray::from_string(std::string(std::bit_cast<char*>(u16.data()), u16.size() * 2));
    }

    b::bytearray string::internal::encode_utf32(const b::string& str) {
        return b::bytearray::from_string(std::string(std::bit_cast<char*>(str.m_data.data()), str.m_data.size() * 4));
    }

    string::operator std::u8string() const {
        return encode_u8<b::enc::utf8>();
    }

    string::operator std::u16string() const {
        return encode<b::enc::utf16>();
    }

    string::operator std::u32string() const {
        return encode<b::enc::utf32>();
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
            result.emplace_back(b::string::decode<b::enc::utf32>(data.substr(0, pos)));
            data.erase(0, pos + delimiter.m_data.length());
        }
        if (!data.empty()) {
            result.emplace_back(b::string::decode<b::enc::utf32>(data));
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
                str.encode<b::enc::utf16_wide>(),
                std::wregex(pattern.encode<b::enc::utf16_wide>()),
                replace.encode<b::enc::utf16_wide>(),
                flags
        );
        return b::string::decode<b::enc::utf16_wide>(result);
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
        std::wstring data = str.encode<b::enc::utf16_wide>();
        while (std::regex_search(data, match, std::wregex(pattern.encode<b::enc::utf16_wide>()), flags)) {
            matches.emplace_back(b::string::decode<b::enc::utf16_wide>(match.str(0)));
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
        return b::string::decode<b::enc::utf32>(buf);
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
            throw b::out_of_range_error("b::string::operator[]: Index out of range");
        }
        return m_data[index];
    }

    const char32_t& string::operator[](size_t index) const {
        if (index >= m_data.size()) {
            throw b::out_of_range_error("b::string::operator[]: Index out of range");
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

    [[nodiscard]] string::iterator string::begin() noexcept {
        return m_data.begin();
    }

    [[nodiscard]] string::const_iterator string::begin() const noexcept {
        return m_data.begin();
    }

    [[nodiscard]] string::iterator string::end() noexcept {
        return m_data.end();
    }

    [[nodiscard]] string::const_iterator string::end() const noexcept {
        return m_data.end();
    }

    [[nodiscard]] string::const_iterator string::cbegin() const noexcept {
        return m_data.cbegin();
    }

    [[nodiscard]] string::const_iterator string::cend() const noexcept {
        return m_data.cend();
    }

    [[nodiscard]] string::reverse_iterator string::rbegin() noexcept {
        return m_data.rbegin();
    }

    [[nodiscard]] string::const_reverse_iterator string::rbegin() const noexcept {
        return m_data.rbegin();
    }

    [[nodiscard]] string::reverse_iterator string::rend() noexcept {
        return m_data.rend();
    }

    [[nodiscard]] string::const_reverse_iterator string::rend() const noexcept {
        return m_data.rend();
    }

    [[nodiscard]] string::const_reverse_iterator string::crbegin() const noexcept {
        return m_data.crbegin();
    }

    [[nodiscard]] string::const_reverse_iterator string::crend() const noexcept {
        return m_data.crend();
    }

    // ========================================================
    // ================ End b::string class ===================
    // ========================================================

} // namespace b

b::string operator""_b(const char* str, size_t len) {
    return b::string::decode<b::enc::utf8>(std::string(str, len));
}

b::string operator""_b(const char8_t* str, size_t len) {
    return b::string::decode<b::enc::utf8>(std::u8string(str, len));
}

b::string operator""_b(const char16_t* str, size_t len) {
    return b::string::decode<b::enc::utf16>(std::u16string(str, len));
}

b::string operator""_b(const char32_t* str, size_t len) {
    return b::string::decode<b::enc::utf32>(std::u32string(str, len));
}