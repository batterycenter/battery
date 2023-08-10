#ifndef BATTERY_CORE_STRING_HPP
#define BATTERY_CORE_STRING_HPP

//
// Copyright 2022 Florian Zachs (HerrNamenlos123)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//         http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef SPDLOG_COMPILED_LIB
#define SPDLOG_COMPILED_LIB     // Force SPDLOG to recognize the prebuilt library
#endif

#include "battery/core/exception.hpp"
#include "battery/core/byte.hpp"
#include <functional>
#include <istream>
#include <optional>
#include <ostream>
#include <regex>
#include <spdlog/fmt/fmt.h>
#include <string>
#include <variant>
#include <vector>

///
/// \brief Everything related to string manipulation and encoding
/// \addtogroup String
/// @{
///
namespace b {

    // ========================================================
    // =============== Begin b::string class ==================
    // ========================================================

    /// \brief platform native string type: std::wstring on Windows, std::string on other platforms
#ifdef B_OS_WINDOWS
    using native_string = std::wstring;
#else
    using native_string = std::string;
#endif

    /// \brief A sophisticated, high-level string class with encoding awareness
    /// \details This class is a high-level string container. It cannot be simply constructed from a char array,
    ///          because it leads to encoding ambiguity. Instead, static functions are provided to construct
    ///          strings from arbitrary encodings explicitly. The string class itself is encoding-agnostic.
    ///          It also cannot be implicitly converted to a char array, there are member functions for each encoding.
    ///
    ///          The only exceptions are std::u8string, u16string and u32string, they can be implicitly converted
    ///          because their encoding is known. A plain std::string or string literal cannot be converted as it may
    ///          have any encoding. Thus, when you want to pass a string literal to a function that expects a b::string,
    ///          either write `foo(u8"Hello World!")` or `foo(U"Hello World!")`. If you have a string in another encoding,
    ///          write e.g. `foo(b::string::decode_latin1(otherString))` or `foo(b::string::decode_utf8(stdString))`.
    ///
    ///          The string is stored internally as UTF-32, and all iterators and indices rely on the 32-bit codepoints.
    ///          Because of that, all string operations are Unicode agnostic and operate on character-basis, not byte-basis.
    ///
    ///          Furthermore, a few convenience functions are provided for string manipulation, known from other
    ///          high-level languages.
    /// \todo There is no proper handling for little-endian/big-endian encodings.
    class string {
    public:

        using iterator = std::u32string::iterator;
        using const_iterator = std::u32string::const_iterator;
        using reverse_iterator = std::u32string::reverse_iterator;
        using const_reverse_iterator = std::u32string::const_reverse_iterator;

        /// \brief b::string default constructor
        string() = default;
        string(const string& other) = default;
        string(string&& other) = default;
        string& operator=(const b::string& other) = default;
        string& operator=(b::string&& other) = default;

        /// \brief Construct a b::string from a std::u8string or from a u8 string literal
        string(const std::u8string& other);

        /// \brief Construct a b::string from a std::u8string or from a u8 string literal
        string(const char8_t* other);

        /// \brief Construct a b::string from a std::u16string or from a u16 string literal
        string(const std::u16string& other);

        /// \brief Construct a b::string from a std::u16string or from a u16 string literal
        string(const char16_t* other);

        /// \brief Construct a b::string from a std::u32string or from a U string literal
        string(const std::u32string& other);

        /// \brief Construct a b::string from a std::u32string or from a U string literal
        string(const char32_t* other);

        /// \brief Construct a b::string from a char32_t
        string(char32_t chr);

        /// \brief Assign a b::string from a std::u8string or from a u8 string literal
        string& operator=(const std::u8string& other);

        /// \brief Assign a b::string from a std::u8string or from a u8 string literal
        string& operator=(const char8_t* other);

        /// \brief Assign a b::string from a std::u16string or from a u16 string literal
        string& operator=(const std::u16string& other);

        /// \brief Assign a b::string from a std::u16string or from a u16 string literal
        string& operator=(const char16_t* other);

        /// \brief Assign a b::string from a std::u32string or from a U string literal
        string& operator=(const std::u32string& other);

        /// \brief Assign a b::string from a std::u32string or from a U string literal
        string& operator=(const char32_t* other);

        /// \brief Assign a b::string from a char32_t
        string& operator=(char32_t chr);

        /// \brief Appending operator
        string& operator+=(const string& other);

        /// \brief Assign another string to this string
        /// \param[in] other The string to assign
        /// \return A reference to itself
        string& assign(const string& other);

        /// \brief Append a string to this string
        /// \details The other string is appended onto this string, and then a reference to itself is returned
        ///          to allow chained operations.
        /// \param[in] other The string to append
        /// \return A reference to itself
        string& append(const string& other);

        /// \brief The current length of the string in characters (Unicode agnostic)
        size_t size() const;

        /// \brief The current length of the string in characters (Unicode agnostic)
        size_t length() const;

        /// \brief If the string is empty
        /// \return true if the string is empty, false otherwise
        bool empty() const;

        /// \brief Get a reference to the last character of the string (Unicode agnostic)
        /// \throw std::out_of_range if the string is empty
        /// \return A reference to the last character of the string
        char32_t& back();

        /// \brief Get a const reference to the last character of the string (Unicode agnostic)
        /// \throw std::out_of_range if the string is empty
        /// \return A const reference to the last character of the string
        const char32_t& back() const;

        /// \brief Get a reference to the first character of the string (Unicode agnostic)
        /// \throw std::out_of_range if the string is empty
        /// \return A reference to the first character of the string
        char32_t& front();

        /// \brief Get a const reference to the first character of the string (Unicode agnostic)
        /// \throw std::out_of_range if the string is empty
        /// \return A const reference to the first character of the string
        const char32_t& front() const;

        /// \brief Get a reference to a specific character of the string (Unicode agnostic)
        /// \details This function has a const variant. If the index is out of range, std::out_of_range is thrown.
        /// \param[in] index The index of the character to get
        /// \throw std::out_of_range if the index is out of range
        /// \return A reference to the character at the specified index
        char32_t& at(size_t index);

        /// \brief Get a reference to a specific character of the string (Unicode agnostic)
        /// \details This function has a const variant. If the index is out of range, std::out_of_range is thrown.
        /// \param[in] index The index of the character to get
        /// \throw std::out_of_range if the index is out of range
        /// \return A reference to the character at the specified index
        [[nodiscard]] const char32_t& at(size_t index) const;

        /// \brief Push a character onto the string and thus append it
        void push_back(char32_t chr);

        /// \brief Pop the last character from the string and return it (Unicode agnostic)
        /// \throw std::out_of_range if the string is empty
        /// \return The last character of the string
        char32_t pop_back();

        // TODO Missing
        // insert
        // erase
        // replace
        // swap
        // find
        // rfind
        // find_first_of
        // find_last_of
        // find_first_not_of
        // find_last_not_of

        /// \brief Resize the string to a new size (By adding or removing characters)
        /// \details If the new size is smaller than the current size, the string is truncated.
        ///          Otherwise, zeros are appended to the string.
        /// \param[in] newSize The new size of the string
        void resize(size_t newSize);

        /// \brief Resize the string to a new size (By adding or removing characters)
        /// \details If the new size is smaller than the current size, the string is truncated.
        ///          Otherwise, characters of 'fill' are appended to the string.
        /// \param[in] newSize The new size of the string
        /// \param[in] fill The character to fill the string with if the new size is bigger than the current size
        void resize(size_t newSize, char32_t fill);

        /// \brief Get the capacity of the string in characters
        /// \return The capacity of the string in characters
        [[nodiscard]] size_t capacity() const;

        /// \brief Reserve x number of characters by allocating more space. No characters are added or removed.
        /// \details If the new size is larger than the current capacity, more memory is allocated to make space for
        ///          at least said number of characters.
        /// \param[in] newSize The new minimum capacity of the string
        void reserve(size_t newSize);

        /// \brief Clear the string
        /// \details This function removes all characters from the string.
        void clear();

        /// \brief Reverse the string
        /// \details This function reverses the string in-place.
        void reverse();

        /// \brief Reverse the string
        /// \param[0] str The string to reverse
        /// \return The reversed string
        [[nodiscard]] static string reverse(const string& str);

        /// \brief Get a part of the string by specifying start position and length of the substring
        /// \details This function is Unicode agnostic, the position and length is given in Characters (not bytes).
        ///          By default, the length is set to std::string::npos, which means 'until the end of the string'.
        /// \param[in] pos The start position of the substring
        /// \param[in] len The length of the substring
        /// \throw std::out_of_range if the start position is out of range
        /// \return The substring
        [[nodiscard]] string substr(size_t pos, size_t len = std::string::npos) const;

        /// \brief static validation function for UTF-8 strings
        /// \param[in] str The UTF-8 encoded string to validate
        /// \return false if the string is not valid UTF-8, true otherwise
        static bool is_valid_utf8(const std::string& str);

        /// \brief Decode a byte series encoded in UTF-8 into generic b::string format.
        /// \throw b::unicode_error if the string is not encoded in valid UTF-8
        /// \return b::string
        static string decode_utf8(const std::string& str);

        /// \brief Decode a byte series encoded in UTF-8 into generic b::string format.
        /// \throw b::unicode_error if the string is not encoded in valid UTF-8
        /// \return b::string
        static string decode_utf8(const std::u8string& str);

        /// \brief Decode a byte series encoded in UTF-8 into generic b::string format.
        /// \details Equivalent to `decode_utf8()`
        /// \throw b::unicode_error if the string is not encoded in valid UTF-8
        /// \return b::string
        static string decode_u8(const std::u8string& str);

        /// \brief Decode a 16-bit character string encoded in UTF-16 into generic b::string format.
        /// \throw b::unicode_error if the string is not encoded in valid UTF-16
        /// \return b::string
        static string decode_utf16(const std::u16string& str);

        /// \brief Decode a 16-bit character string encoded in UTF-16 into generic b::string format.
        /// \throw b::unicode_error if the string is not encoded in valid UTF-16
        /// \return b::string
        static string decode_utf16(const std::wstring& str);

        /// \brief Decode a 16-bit wide-character string encoded in UTF-16 into generic b::string format.
        /// \throw b::unicode_error if the string is not encoded in valid UTF-16
        /// \return b::string
        static string decode_widestring(const std::wstring& str);

        /// \brief Decode a 32-bit character string encoded in UTF-32 into generic b::string format.
        /// \throw b::unicode_error if the string is not encoded in valid UTF-32
        /// \return b::string
        static string decode_utf32(const std::u32string& str);

        /// \brief Decode a single 32-bit character encoded in UTF-32 into generic b::string format.
        /// \throw b::unicode_error if the string is not encoded in valid UTF-32
        /// \return b::string
        static string decode_utf32(char32_t chr);

        /// \brief Decode a platform-native string from its local encoding into generic b::string format.
        /// \details This function takes a `b::native_string`, which is a typedef for `std::wstring` on Windows and
        ///          `std::string` on other platforms. On Windows it is equivalent to `decode_widestring()` and on
        ///          other platforms it is equivalent to `decode_utf8()`.
        /// \warning This function assumes that the native OS-specific encoding used for system calls is UTF-8 on
        ///          non-Windows platforms. If you are on a non-Windows platform that does not use UTF-8 for system
        ///          calls, please get in touch with the developers to get native support.
        /// \throw b::unicode_error if the string is not encoded in valid UTF-8 or UTF-16
        /// \return b::string
        static string decode_native(const b::native_string& str);

        /// \brief Decode a string encoded in ASCII into generic b::string format. (ONLY 7-bit ASCII)
        /// \details Note: The input is interpreted as UTF-8 as it can be seen as a superset of ASCII. This does not
        ///          apply for any other encoding.
        /// \warning Be careful with the distinction between ASCII, Windows-1252 and ISO-8859-1 (which is Latin-1).
        ///          This function expects plain 7-bit ASCII strings, nothing more. Encodings are sometimes mislabeled
        ///          as ASCII when they are in fact something else.
        /// \throw b::unicode_error if the string is not encoded in valid UTF-8
        /// \return b::string
        static string decode_ascii(const std::string& str);

        /// \brief Decode a single ASCII character into generic b::string format. (ONLY 7-bit ASCII)
        /// \details Note: The input is interpreted as UTF-8 as it can be seen as a superset of ASCII. This does not
        ///          apply for any other encoding.
        /// \warning Be careful with the distinction between ASCII, Windows-1252 and ISO-8859-1 (which is Latin-1).
        ///          This function expects plain 7-bit ASCII strings, nothing more. Encodings are sometimes mislabeled
        ///          as ASCII when they are in fact something else.
        /// \throw b::unicode_error if the string is not encoded in valid UTF-8
        /// \return b::string
        static string decode_ascii(char chr);

        /// \brief Decode a string encoded in Latin-1 into generic b::string format. Equivalent to decode_iso8859_1().
        /// \details Latin-1 (which is also called ISO-8859-1) is a superset of ASCII, but only a subset of
        ///          Windows-1252. Thus, this function can also take plain 7-bit ASCII, but not Windows-1252 (which is
        ///          very common on Windows and is known for being mistreated as ASCII).
        /// \note This function would throw when the string after conversion to UTF-8 is not convertible to UTF-32,
        ///       but that is practically impossible as every possible byte in Latin-1 is mapped to a character.
        ///       Thus, this function practically does not throw.
        /// \return b::string
        static string decode_latin1(const std::string& str);

        /// \brief Decode a single Latin-1 character into generic b::string format. Equivalent to decode_iso8859_1().
        /// \details Latin-1 (which is also called ISO-8859-1) is a superset of ASCII, but only a subset of
        ///          Windows-1252. Thus, this function can also take plain 7-bit ASCII, but not Windows-1252 (which is
        ///          very common on Windows and is known for being mistreated as ASCII).
        /// \note This function would throw when the string after conversion to UTF-8 is not convertible to UTF-32,
        ///       but that is practically impossible as every possible byte in Latin-1 is mapped to a character.
        ///       Thus, this function practically does not throw.
        /// \return b::string
        static string decode_latin1(char chr);

        /// \brief Decode a string encoded in ISO-8859-1 into generic b::string format. Equivalent to decode_latin1().
        /// \details ISO-8859-1 (which is also called Latin-1) is a superset of ASCII, but only a subset of
        ///          Windows-1252. Thus, this function can also take plain 7-bit ASCII, but not Windows-1252 (which is
        ///          very common on Windows and is known for being mistreated as ASCII).
        /// \note This function would throw when the string after conversion to UTF-8 is not convertible to UTF-32,
        ///       but that is practically impossible as every possible byte in ISO-8859-1 is mapped to a character.
        ///       Thus, this function practically does not throw.
        /// \return b::string
        static string decode_iso8859_1(const std::string& str);

        /// \brief Decode a single ISO-8859-1 character into generic b::string format. Equivalent to decode_latin1().
        /// \details ISO-8859-1 (which is also called Latin-1) is a superset of ASCII, but only a subset of
        ///          Windows-1252. Thus, this function can also take plain 7-bit ASCII, but not Windows-1252 (which is
        ///          very common on Windows and is known for being mistreated as ASCII).
        /// \note This function would throw when the string after conversion to UTF-8 is not convertible to UTF-32,
        ///       but that is practically impossible as every possible byte in ISO-8859-1 is mapped to a character.
        ///       Thus, this function practically does not throw.
        /// \return b::string
        static string decode_iso8859_1(char chr);

#ifdef B_OS_WINDOWS
        /// \brief Decode a string encoded in Windows-1252 (incorrectly known as ANSI) into generic b::string format.
        /// \details Windows-1252 is a superset of ASCII and Latin-1 (ISO-8859-1). Thus, this function may also take
        ///          Latin-1 (ISO-8859-1) or ASCII input.
        /// \warning This function is only available on Windows
        /// \note This function would throw when the string after conversion to UTF-8 is not convertible to UTF-32,
        ///       but that is practically impossible as every possible byte in Windows-1252 is mapped to a character.
        ///       Thus, this function practically does not throw.
        /// \return b::string
        static string decode_windows1252(const std::string& str);

        /// \brief Decode a single Windows-1252 (incorrectly known as ANSI) character into generic b::string format.
        /// \details Windows-1252 is a superset of ASCII and Latin-1 (ISO-8859-1). Thus, this function may also take
        ///          Latin-1 (ISO-8859-1) or ASCII input.
        /// \warning This function is only available on Windows
        /// \note This function would throw when the string after conversion to UTF-8 is not convertible to UTF-32,
        ///       but that is practically impossible as every possible byte in Windows-1252 is mapped to a character.
        ///       Thus, this function practically does not throw.
        /// \return b::string
        static string decode_windows1252(char chr);
#endif

        /// \brief Encode the high-level generic b::string object into a string containing UTF-8 encoded bytes.
        /// \return The string containing UTF-8 encoded data
        [[nodiscard]] std::string encode_utf8() const;

        /// \brief Encode the high-level generic b::string object into a string containing UTF-8 encoded bytes.
        /// \return The string containing UTF-8 encoded data
        [[nodiscard]] std::u8string encode_u8() const;

        /// \brief Encode the high-level generic b::string object into a string containing UTF-16 encoded characters.
        /// \return The string containing UTF-16 encoded data
        [[nodiscard]] std::u16string encode_utf16() const;

        /// \brief Encode the high-level generic b::string object into a string with UTF-16 encoded wide-characters.
        /// \warning If you use this function for OS-level system calls, be aware of the lifetime of the strings.
        ///          Calling `str.encode_widestring().c_str()` means the lifetime of the pointer is just this one
        ///          line of code. Some system functions need the string to live longer until the processing is
        ///          triggered by another function call, in this case store the return value of this function in a
        ///          separate variable and then call `c_str()` on this variable instead. This will keep the pointer
        ///          alive as long as the variable exists.
        /// \return The wide-character string containing UTF-16 encoded data
        /// \see b::string::encode_native()
        [[nodiscard]] std::wstring encode_widestring() const;

        /// \brief Encode the high-level generic b::string object into a string containing UTF-32 encoded characters.
        /// \return The string containing UTF-32 encoded data
        [[nodiscard]] std::u32string encode_utf32() const;

        /// \brief Encode the high-level generic b::string object into a string with platform-native encoding.
        /// \details This function returns a `b::native_string`, which is a std::wstring on Windows and std::string on
        ///          other platforms. On Windows it is equivalent to encode_widestring(), on other platforms it is
        ///          equivalent to encode_utf8(). This function is meant to be used with OS-level system function calls:
        ///          `str.encode_native().c_str()` will return a UTF-16 encoded `wchar_t*` on Windows and a UTF-8
        ///          encoded `const char*` on other platforms, to be used with the WinAPI or Linux API.
        /// \warning If you use this function for OS-level system calls, be aware of the lifetime of the strings.
        ///          Calling `str.encode_widestring().c_str()` means the lifetime of the pointer is just this one
        ///          line of code. Some system functions need the string to live longer until the processing is
        ///          triggered by another function call, in this case store the return value of this function in a
        ///          separate variable and then call `c_str()` on this variable instead. This will keep the pointer
        ///          alive as long as the variable exists.
        /// \warning This function assumes that the native OS-specific encoding used for system calls is UTF-8 on
        ///          non-Windows platforms. If you are on a non-Windows platform that does not use UTF-8 for system
        ///          calls, please get in touch with the developers to get native support.
        /// \return b::native_string, which is a `std::wstring` on Windows and `std::string` on other platforms
        /// \see b::string::encode_widestring()
        [[nodiscard]] native_string encode_native() const;

        /// \brief Conversion operator to std::u8string. Effectively constructs a string from `encode_u8()`.
        /// \return std::u8string containing UTF-8 encoded data
        [[nodiscard]] operator std::u8string() const;

        /// \brief Conversion operator to std::u16string. Effectively constructs a string from `encode_utf16()`.
        /// \return std::u8string containing UTF-16 encoded data
        [[nodiscard]] operator std::u16string() const;

        /// \brief Conversion operator to std::u32string. Effectively constructs a string from `encode_utf32()`.
        /// \return std::u8string containing UTF-32 encoded data
        [[nodiscard]] operator std::u32string() const;

        /// \brief Get the underlying std::u32string as a read-only reference.
        /// \return The underlying std::u32string
        [[nodiscard]] const std::u32string& str() const;

        /// \brief Split this string object into an array of string pieces by a delimiter.
        /// \details When no delimiter is found, only the input string is returned. Several delimiters in a row
        ///          are treated as a single one. The delimiter can be one or more characters.
        /// \param[in] delimiter The characters to split the string at
        /// \return An array of string tokens
        /// \see b::string::join()
        [[nodiscard]] std::vector<b::string> split(const b::string& delimiter) const;

        /// \brief Split a string into an array of string pieces by a delimiter.
        /// \details When no delimiter is found, only the input string is returned. Several delimiters in a row
        ///          are treated as a single one. The delimiter can be one or more characters.
        /// \param[in] str The string to be split
        /// \param[in] delimiter The characters to split the string at
        /// \return An array of string tokens
        /// \see b::string::join()
        static std::vector<b::string> split(const b::string& str, const b::string& delimiter);

        /// \brief Join an array of strings into a single one with itself being the spacer.
        /// \details The spacer string (this object) is inserted between any two string tokens being joined.
        /// \param[in] strings Array of string tokens to be joined
        /// \return The combined string
        /// \see b::string::split()
        b::string join(const std::vector<b::string>& strings);

        /// \brief Join an array of strings into a single one.
        /// \details The spacer string is inserted between any two string tokens being joined.
        /// \param[in] strings Array of string tokens to be joined
        /// \param[in] spacer Spacer string
        /// \return The combined string
        /// \see b::string::split()
        static b::string join(const std::vector<b::string>& strings, const b::string& spacer);

        /// \brief Replace all matches of a Regular Expression (RegEx)
        /// \details The last parameter is used to further define the processing of the regex.
        ///          Use it to only replace the first match, specify non-global regex, multiline, etc.
        ///          If the regex in generic form is `/[a-z]/g`, pass `"[a-z]"_b` as the pattern, and 'g'
        ///          is already a default flag. If there were any other characters after the last '/',
        ///          pass the corresponding flags as the last parameter. It is advised to use raw string
        ///          literals for the pattern, to avoid having to escape backslashes.
        ///          e.g. `/a-z\(\)0-9/g` can be written as `R"(a-z\(\)0-9)"_b` instead of `"a-z\\(\\)0-9"_b`
        /// \param[in] pattern A regex pattern
        /// \param[in] replace What to replace it with
        /// \param[in] flags Regex flags of type std::regex_constants::...
        void regex_replace(const b::string& pattern, const b::string& replace,
                           std::regex_constants::match_flag_type flags =
                                   std::regex_constants::match_default);

        /// \brief Replace all matches of a Regular Expression (RegEx)
        /// \details See the non-static version of this function for more details.
        /// \param[in] str The string to be manipulated
        /// \param[in] pattern A regex pattern
        /// \param[in] replace What to replace it with
        /// \param[in] flags Regex flags of type std::regex_constants::...
        /// \return The string with all matches replaced
        static b::string regex_replace(const b::string& str, const b::string& pattern, const b::string& replace,
                                       std::regex_constants::match_flag_type flags =
                                               std::regex_constants::match_default);

        /// \brief Find all matches of a Regular Expression (RegEx)
        /// \details The last parameter is used to further define the processing of the regex.
        ///          Use it to only replace the first match, specify non-global regex, multiline, etc.
        ///          If the regex in generic form is `/[a-z]/g`, pass `"[a-z]"_b` as the pattern, and 'g'
        ///          is already a default flag. If there were any other characters after the last '/',
        ///          pass the corresponding flags as the last parameter. It is advised to use raw string
        ///          literals for the pattern, to avoid having to escape backslashes.
        ///          e.g. `/a-z\(\)0-9/g` can be written as `R"(a-z\(\)0-9)"_b` instead of `"a-z\\(\\)0-9"_b`
        /// \param[in] pattern A regex pattern
        /// \param[in] flags Regex flags of type std::regex_constants::...
        /// \return An array with all matches
        std::vector<b::string> regex_match(const b::string& pattern,
                                           std::regex_constants::match_flag_type flags =
                                                   std::regex_constants::match_default);

        /// \brief Find all matches of a Regular Expression (RegEx)
        /// \details See the non-static version of this function for more details.
        /// \param[in] str The string to be manipulated
        /// \param[in] pattern A regex pattern
        /// \param[in] flags Regex flags of type std::regex_constants::...
        /// \return An array with all matches
        static std::vector<b::string> regex_match(const b::string& str,
                                                  const b::string& pattern,
                                                  std::regex_constants::match_flag_type flags =
                                                        std::regex_constants::match_default);

        /// \brief Replace all occurrences of a string with another string using a custom replacer function.
        /// \details The replacer function is called for every occurrence of the pattern.
        /// \param[in] pattern A token to be replaced
        /// \param[in] replacer A small function taking the matched string and the match index,
        ///            and returning the replacement string
        /// \see b::string::regex_replace()
        void replace(const b::string& pattern, std::function<b::string(int)> replacer);

        /// \brief Replace all occurrences of a string with another string using a custom replacer function.
        /// \details The replacer function is called for every occurrence of the pattern.
        /// \param[in] str The string to be modified
        /// \param[in] pattern A token to be replaced
        /// \param[in] replacer A small function taking the matched string and the match index,
        ///            and returning the replacement string
        /// \return The modified string
        /// \see b::string::regex_replace()
        static b::string replace(const b::string& str, const b::string& pattern, std::function<b::string(int)> replacer);

        /// \brief Replace all occurrences of a string with another string (simple find and replace)
        /// \param[in] pattern A token to be replaced
        /// \param[in] value What to replace the token with
        /// \see b::string::regex_replace()
        void replace(const b::string& pattern, const b::string& value);

        /// \brief Replace all occurrences of a string with another string (simple find and replace)
        /// \param[in] str The string to be modified
        /// \param[in] pattern A token to be replaced
        /// \param[in] value What to replace the token with
        /// \return The modified string
        /// \see b::string::regex_replace()
        static b::string replace(const b::string& str, const b::string& pattern, const b::string& value);

        /// \brief Replace the first occurrence of a string with another string (simple find and replace)
        /// \param[in] pattern A token to be replaced
        /// \param[in] value What to replace the token with
        /// \see b::string::regex_replace()
        void replace_first(const b::string& pattern, const b::string& value);

        /// \brief Replace the first occurrence of a string with another string (simple find and replace)
        /// \param[in] str The string to be modified
        /// \param[in] pattern A token to be replaced
        /// \param[in] value What to replace the token with
        /// \return The modified string
        /// \see b::string::regex_replace()
        static b::string replace_first(const b::string& str, const b::string& pattern, const b::string& value);

        /// \brief Replace the last occurrence of a string with another string (simple find and replace)
        /// \param[in] pattern A token to be replaced
        /// \param[in] value What to replace the token with
        /// \see b::string::regex_replace()
        void replace_last(const b::string& pattern, const b::string& value);

        /// \brief Replace the last occurrence of a string with another string (simple find and replace)
        /// \param[in] str The string to be modified
        /// \param[in] pattern A token to be replaced
        /// \param[in] value What to replace the token with
        /// \return The modified string
        /// \see b::string::regex_replace()
        static b::string replace_last(const b::string& str, const b::string& pattern, const b::string& value);

        /// \brief Make all characters lowercase
        /// \details Unicode characters are transformed like expected.
        /// \return Reference to self
        /// \see b::string::to_upper()
        string& to_lower();

        /// \brief Make all characters lowercase
        /// \details Unicode characters are transformed like expected.
        /// \return Reference to self
        /// \see b::string::to_upper()
        static string to_lower(const b::string& str);

        /// \brief Make all characters uppercase
        /// \details Unicode characters are transformed like expected. Special case: "ß" is transformed to "SS".
        /// \return Reference to self
        /// \see b::string::to_lower()
        string& to_upper();

        /// \brief Make all characters uppercase
        /// \details Unicode characters are transformed like expected. Special case: "ß" is transformed to "SS".
        /// \return Reference to self
        /// \see b::string::to_lower()
        static string to_upper(const b::string& str);

        /// \brief Get a character at a specific index.
        /// \param[in] index The index of the character
        /// \throw std::out_of_range if the index is out of bounds
        /// \return A reference to the character at the index
        char32_t& operator[](size_t index);

        /// \brief Get a character at a specific index.
        /// \param[in] index The index of the character
        /// \throw std::out_of_range if the index is out of bounds
        /// \return A reference to the character at the index
        const char32_t& operator[](size_t index) const;

        /// \brief String concatenation operator
        /// \details appends two strings
        /// \param[in] other The string to append
        /// \return The concatenated string
        string& operator+(const string& other);

        /// \brief String comparison operator
        /// \return True if the strings are equal
        bool operator==(const string& other) const;

        /// \brief String comparison operator
        /// \return True if the strings are not equal
        bool operator!=(const string& other) const;

        /// \brief Simple, standard iterator forwarding
        [[nodiscard]] iterator begin() noexcept;

        /// \brief Simple, standard iterator forwarding
        [[nodiscard]] const_iterator begin() const noexcept;

        /// \brief Simple, standard iterator forwarding
        [[nodiscard]] iterator end() noexcept;

        /// \brief Simple, standard iterator forwarding
        [[nodiscard]] const_iterator end() const noexcept;

        /// \brief Simple, standard iterator forwarding
        [[nodiscard]] const_iterator cbegin() const noexcept;

        /// \brief Simple, standard iterator forwarding
        [[nodiscard]] const_iterator cend() const noexcept;

        /// \brief Simple, standard iterator forwarding
        [[nodiscard]] reverse_iterator rbegin() noexcept;

        /// \brief Simple, standard iterator forwarding
        [[nodiscard]] const_reverse_iterator rbegin() const noexcept;

        /// \brief Simple, standard iterator forwarding
        [[nodiscard]] reverse_iterator rend() noexcept;

        /// \brief Simple, standard iterator forwarding
        [[nodiscard]] const_reverse_iterator rend() const noexcept;

        /// \brief Simple, standard iterator forwarding
        [[nodiscard]] const_reverse_iterator crbegin() const noexcept;

        /// \brief Simple, standard iterator forwarding
        [[nodiscard]] const_reverse_iterator crend() const noexcept;

    private:
        std::u32string m_data;
    };

    // ========================================================
    // ================ End b::string class ===================
    // ========================================================





    // ========================================================
    // =============== Begin base-64 encoding =================
    // ========================================================

    ///
    /// \brief Encode a string or text message as Base-64, encoded in UTF-8
    /// \details This function is especially useful for exchanging binary resources over a network.
    ///          See `b::encode_base64(const b::bytearray&)` for more information about binary resources.
    /// \param[in] str The string to encode
    /// \return Encoded Base-64 string
    /// \see b::decode_base64()
    ///
    b::string encode_base64(const b::string& str);

    ///
    /// \brief Encode a binary resource as Base-64
    /// \details This is especially useful for embedding small resource files in the source code, or exchanging
    ///          binary resources over a network. This overload is for encoding binary resources based on
    ///          `std::vector<uint8_t>`. Both overloads do the same thing with another input type.
    /// \param[in] str The byte-series to encode
    /// \return Encoded Base-64 string
    /// \see b::decode_base64_text()
    /// \see b::decode_base64_binary()
    ///
    b::string encode_base64(const b::bytearray& data);

    ///
    /// \brief Decode a Base-64 encoded text resource back to its original form. The encoding is assumed to be UTF-8.
    /// \details The encoding of the string before it was encoded is assumed to be UTF-8. This function is to be used
    ///          with text resources.
    /// \param[in] str A Base-64 encoded string representing a resource
    /// \throw b::base64_invalid_error if the input string is not a valid Base-64 string or the input is empty
    /// \todo Implement a way to specify the encoding of the string before it was encoded.
    /// \return Decoded resource in its original form when it was encoded.
    /// \see b::encode_base64()
    ///
    b::string decode_base64_text(const b::string& str);

    ///
    /// \brief Decode a Base-64 encoded binary resource back to its original form.
    /// \details This function is to be used with binary resources such as images, icons, etc.
    /// \param[in] str A Base-64 encoded string representing a resource
    /// \throw b::base64_invalid_error if the input string is not a valid Base-64 string or the input is empty
    /// \return Decoded binary resource as a byte array
    /// \see b::encode_base64()
    ///
    b::bytearray decode_base64_binary(const b::string& str);

    // ========================================================
    // ================ End base-64 encoding ==================
    // ========================================================

} // namespace b

/// \brief Hash function overload that lets b::string be used as a key in
///        `std::map` and `std::unordered_map`
namespace std {
    template <> struct hash<b::string> {
        size_t operator()(const b::string& str) const {
            return std::hash<std::u32string>()(str.str());
        }
    };
} // namespace std

/// \brief fmt formatter to allow formatting b::string with fmt::format
template <> struct fmt::formatter<b::string> {
    constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator {
        return ctx.begin();
    }
    auto format(const b::string& str, format_context& ctx) const -> format_context::iterator {
        return fmt::format_to(ctx.out(), "{}", str.encode_utf8());
    }
};

/// brief Custom string literal operator to be able to write "Hello"_b to make it a b::string immediately.
[[nodiscard]] b::string operator""_b(const char* str, size_t len);   // TODO: Make these constexpr

/// brief Custom string literal operator to be able to write u8"Hello"_b to make it a b::string immediately.
[[nodiscard]] b::string operator""_b(const char8_t* str, size_t len);

/// brief Custom string literal operator to be able to write u"Hello"_b to make it a b::string immediately.
[[nodiscard]] b::string operator""_b(const char16_t* str, size_t len);

/// brief Custom string literal operator to be able to write U"Hello"_b to make it a b::string immediately.
[[nodiscard]] b::string operator""_b(const char32_t* str, size_t len);

#endif // BATTERY_CORE_STRING_HPP