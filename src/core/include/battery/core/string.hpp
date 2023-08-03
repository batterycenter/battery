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

#pragma once

#include "battery/core/exception.hpp"
#include "battery/core/byte.hpp"
#include <functional>
#include <istream>
#include <optional>
#include <ostream>
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

    /// \brief platform native string type: std::wstring on Windows, std::string on other platforms
#ifdef B_OS_WINDOWS
    using native_string = std::wstring;
#else
    using native_string = std::string;
#endif

    /// \brief A string iterator class for b::string
    template<typename T, bool Reverse>
    class string_iterator {
    public:
        string_iterator(T* ptr) : m_ptr(ptr) {}

        bool operator!=(const string_iterator& other) {
            return m_ptr != other.m_ptr;
        }

        string_iterator operator++() {
            if constexpr (Reverse) {
                m_ptr--;
            }
            else {
                m_ptr++;
            }
            return *this;
        }

        string_iterator operator++(int) {
            auto iter = *this;
            if constexpr (Reverse) {
                --(*this);
            }
            else {
                ++(*this);
            }
            return iter;
        }

        string_iterator operator--() {
            if constexpr (Reverse) {
                m_ptr++;
            }
            else {
                m_ptr--;
            }
            return *this;
        }

        string_iterator operator--(int) {
            auto iter = *this;
            if constexpr (Reverse) {
                ++(*this);
            }
            else {
                --(*this);
            }
            return iter;
        }

        T& operator[](size_t index) {
            return *(get() + index);
        }

        T* operator->() {
            return get();
        }

        T& operator*() {
            return *get();
        }

    private:
        T* get() {
            if constexpr (Reverse) {
                return m_ptr - 1;
            }
            else {
                return m_ptr;
            }
        }

        T* m_ptr;
    };

    ///
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
    ///          write e.g. `foo(b::string::from_latin1(otherString))` or `foo(b::string::from_utf8(stdString))`.
    ///
    ///          The string is stored internally as UTF-32, and all iterators and indices rely on the 32-bit codepoints.
    ///          Because of that, all string operations are Unicode agnostic and operate on character-basis, not byte-basis.
    ///
    ///          Furthermore, a few convenience functions are provided for string manipulation, known from other
    ///          high-level languages.
    ///
    class string {
    public:

        using iterator = string_iterator<char32_t,false>;
        using const_iterator = string_iterator<const char32_t,false>;
        using reverse_iterator = string_iterator<char32_t,true>;
        using const_reverse_iterator = string_iterator<const char32_t,true>;

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
        const char32_t& at(size_t index) const;

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
        // high-level find functions

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

        /// \brief Reverse the string (Unicode agnostic)
        /// \details This function reverses the string in-place.
        void reverse();

        /// \brief Get a part of the string by specifying start position and length of the substring (Unicode agnostic)
        /// \details This function is Unicode agnostic, the position and length is given in Characters (not bytes).
        ///          By default, the length is set to std::string::npos, which means 'until the end of the string'.
        /// \param[in] pos The start position of the substring
        /// \param[in] len The length of the substring
        /// \return The substring
        string substr(size_t pos, size_t len = std::string::npos) const;

        /// \brief static validation function for UTF-8 strings
        /// \param[in] str The UTF-8 encoded string to validate
        /// \return false if the string is not valid UTF-8, true otherwise
        static bool is_valid_utf8(const std::string& str);

        /// \brief Build a b::string from an std::string containing UTF-8 encoded bytes
        /// \throw b::unicode_error if the string does not contain valid UTF-8
        /// \return b::string
        static string from_utf8(const std::string& str);

        /// \brief Build a b::string from an std::u8string containing UTF-8 encoded bytes
        /// \throw b::unicode_error if the string does not contain valid UTF-8
        /// \return b::string
        static string from_utf8(const std::u8string& str);

        /// \brief Build a b::string from an std::u16string containing UTF-16 encoded bytes
        /// \throw b::unicode_error if the string does not contain valid UTF-8
        /// \return b::string
        static string from_utf16(const std::u16string& str);

        /// \brief Build a b::string from an std::wstring containing UTF-16 encoded bytes
        /// \throw b::unicode_error if the string does not contain valid UTF-8
        /// \return b::string
        static string from_utf16(const std::wstring& str);

        /// \brief Build a b::string from an std::wstring containing UTF-16 encoded bytes
        /// \throw b::unicode_error if the string does not contain valid UTF-8
        /// \return b::string
        static string from_widestring(const std::wstring& str);

        /// \brief Build a b::string from an std::u32string containing UTF-32 encoded bytes
        /// \throw b::unicode_error if the string does not contain valid UTF-8
        /// \return b::string
        static string from_utf32(const std::u32string& str);

        /// \brief Build a b::string from a single char32_t containing a single UTF-32 codepoint
        /// \throw b::unicode_error if the string does not contain valid UTF-32
        /// \return b::string
        static string from_utf32(char32_t chr);

        /// \brief Build a b::string from a native_string, which is a UTF-16 wide string on Windows and a UTF-8 string others
        /// \throw b::unicode_error if the string does not contain valid Unicode
        /// \return b::string
        static string from_native(const b::native_string& str);

        /// \brief Build a b::string from an std::string containing plain ASCII bytes (7-bit characters)
        /// \details No validation is performed. All bytes are copied as-is and interpreted as UTF-8.
        /// \warning Be very careful with the distinction between ASCII and Windows-1252 and ISO-8859-1.
        ///          This function expects plain 7-bit ASCII strings, nothing more. Windows-1252 and ISO-8859-1 are
        ///          not compatible. Encodings are often mislabeled as ASCII when they are in fact Windows-1252.
        /// \throw b::unicode_error if the string does not contain valid UTF-8
        /// \return b::string
        static string from_ascii(const std::string& str);

        /// \brief Build a b::string from a single ASCII character (7-bit character)
        /// \details No validation is performed. All bytes are copied as-is and interpreted as UTF-8.
        /// \warning Be very careful with the distinction between ASCII and Windows-1252 and ISO-8859-1.
        ///          This function expects plain 7-bit ASCII strings, nothing more. Windows-1252 and ISO-8859-1 are
        ///          not compatible. Encodings are often mislabeled as ASCII when they are in fact Windows-1252.
        /// \throw b::unicode_error if the string does not contain valid UTF-8
        /// \return b::string
        static string from_ascii(char chr);

        /// \brief Build a b::string from an std::string containing Latin-1 or ISO-8859-1 encoding
        /// \details Latin-1 is a subset of Windows-1252, this function cannot take Windows-1252 input.
        ///          This function fails if the result after the conversion is invalid UTF-8.
        /// \warning Be very careful with the distinction between Windows-1252 and ISO-8859-1. Encodings are often
        ///          mislabeled as ISO-8859-1 when they are in fact Windows-1252.
        /// \return b::string
        static string from_latin1(const std::string& str);

        /// \brief Build a b::string from a single ISO-8859-1 character
        /// \details Latin-1 is a subset of Windows-1252, this function cannot take Windows-1252 input.
        ///          This function fails if the result after the conversion is invalid UTF-8.
        /// \warning Be very careful with the distinction between Windows-1252 and ISO-8859-1. Encodings are often
        ///          mislabeled as ISO-8859-1 when they are in fact Windows-1252.
        /// \return b::string
        static string from_latin1(char chr);

#ifdef B_OS_WINDOWS
        /// \brief Build a b::string from an std::string containing Windows-1252 encoding (sometimes called ANSI)
        /// \details Windows-1252 is a superset of Latin-1 or ISO-8859-1. Thus, this function can also take Latin-1 input.
        /// \warning This function is only available on Windows
        /// \warning Be very careful with the distinction between Windows-1252 and ISO-8859-1. It will happen that
        ///          you at some point receive Windows-1252 data mislabeled as ISO-8859-1.
        /// \throw b::unicode_error if the string is invalid
        /// \return b::string
        static string from_windows1252(const std::string& str);

        /// \brief Build a b::string from a single character with Windows-1252 encoding (sometimes called ANSI)
        /// \details Windows-1252 is a superset of Latin-1 or ISO-8859-1. Thus, this function can also take Latin-1 input.
        /// \warning This function is only available on Windows
        /// \warning Be very careful with the distinction between Windows-1252 and ISO-8859-1. It will happen that
        ///          you at some point receive Windows-1252 data mislabeled as ISO-8859-1.
        /// \throw b::unicode_error if the string is invalid
        /// \return b::string
        static string from_windows1252(char chr);
#endif

        /// \brief conversion function to std::string encoded in UTF-8
        /// \details This function cannot fail.
        /// \return std::string containing the UTF-8 encoded data
        [[nodiscard]] std::string to_utf8() const;

        /// \brief conversion function to std::u8string encoded in UTF-8
        /// \details This function cannot fail.
        /// \return std::u8string containing the UTF-8 encoded data
        [[nodiscard]] std::u8string to_u8() const;

        /// \brief conversion function to std::string encoded in UTF-16
        /// \details This function cannot fail.
        /// \return std::u16string containing the UTF-16 encoded data
        [[nodiscard]] std::u16string to_utf16() const;

        /// \brief conversion function to std::wstring encoded in UTF-16. Use this for WinAPI functions.
        /// \details This function cannot fail. Also see `to_osstring()`.
        /// \warning If you use this function for WinAPI calls, be aware of the lifetime of the strings.
        ///          Calling `string.to_widestring().c_str()` means the lifetime of the pointer is just this one
        ///          function call. Some WinAPI functions need the string to live longer until it is processed internally.
        ///          In this case store the widestring in a `std::wstring` (using auto) explicitly and then
        ///          call `c_str()` on that, it will keep the data alive as long as the variable exists.
        /// \return std::wstring containing the UTF-16 encoded data
        /// \see to_osstring()
        [[nodiscard]] std::wstring to_widestring() const;

        /// \brief conversion function to std::u32string encoded in UTF-32
        /// \details This function cannot fail.
        /// \return std::u32string containing the UTF-32 encoded data
        [[nodiscard]] std::u32string to_utf32() const;

        /// \brief convert to a platform native string: std::wstring on Windows, std::string on other platforms
        /// \details This function cannot fail. Also see `to_widestring()`. `string.to_native().c_str()` will
        ///          return a UTF-16 encoded LPWSTR on Windows and a UTF-8 encoded const char* on other platforms,
        ///          suitable for System API calls.
        /// \warning If you use this function for system calls, be aware of the lifetime of the strings.
        ///          Calling `string.to_native().c_str()` means the lifetime of the pointer is just this one
        ///          function call. Some OS functions need the string to live longer until it is processed internally.
        ///          In this case store the result in a `b::native_string` (using auto) explicitly and then
        ///          call `c_str()` on that, it will keep the data alive as long as the variable exists.
        /// \return b::native_string, which is a std::wstring on Windows and std::string on other platforms
        /// \see to_widestring()
        [[nodiscard]] native_string to_native() const;

        /// \brief Conversion operator to std::u8string encoded in UTF-8
        /// \return std::u8string containing the UTF-8 encoded data
        [[nodiscard]] operator std::u8string() const;

        /// \brief Conversion operator to std::u16string encoded in UTF-16
        /// \return std::u8string containing the UTF-16 encoded data
        [[nodiscard]] operator std::u16string() const;

        /// \brief Conversion operator to std::u32string encoded in UTF-32
        /// \return std::u8string containing the UTF-32 encoded data
        [[nodiscard]] operator std::u32string() const;

        /// \brief Get the underlying std::u32string
        /// \return The underlying std::u32string
        [[nodiscard]] std::u32string& str();

        /// \brief Get the underlying std::u32string
        /// \return The underlying std::u32string
        [[nodiscard]] const std::u32string& str() const;

        ///
        /// \brief Split this string object into an array of string pieces by a delimiter.
        /// \details When no delimiter is found, only the input string is returned. Several delimiters in a row are treated
        ///          as a single one. The delimeter can be one or more characters.
        /// \param[in] delimiter The characters to split the string at
        /// \return An array of string pieces
        /// \see b::string::join()
        ///
        [[nodiscard]] std::vector<b::string> split(const b::string& delimiter) const;

        ///
        /// \brief Split a string into an array of string pieces by a delimiter.
        /// \details When no delimiter is found, only the input string is returned. Several delimiters in a row are treated
        ///          as a single one. The delimeter can be one or more characters.
        /// \param[in] str The string to be split
        /// \param[in] delimiter The characters to split the string at
        /// \return An array of string pieces
        /// \see b::string::join()
        ///
        static std::vector<b::string> split(const b::string& str, const b::string& delimiter);

        ///
        /// \brief Join an array of strings into a single one with this string as the spacer.
        /// \details The spacer string (this object) is inserted between any two string tokens being joined.
        /// \param[in] strings Array of string tokens to be joined
        /// \return The combined string
        /// \see b::string::split()
        ///
        b::string join(const std::vector<b::string>& strings);

        ///
        /// \brief Join an array of strings into a single one.
        /// \details The spacer string is inserted between any two string tokens being joined.
        /// \param[in] strings Array of string tokens to be joined
        /// \param[in] spacer Spacer string
        /// \return The combined string
        /// \see b::string::split()
        ///
        static b::string join(const std::vector<b::string>& strings, const b::string& spacer = u8"");

        ///
        /// \brief Replace all matches of a Regular Expression (RegEx) (Unicode aware)
        /// \param[in] pattern A regex pattern
        /// \param[in] replace What to replace it with
        /// \see b::string::regex_replace_one()
        ///
        void regex_replace(const b::string& pattern, const b::string& replace);

        ///
        /// \brief Replace all matches of a Regular Expression (RegEx) (Unicode aware)
        /// \param[in] str The string to be searched
        /// \param[in] pattern A regex pattern
        /// \param[in] replace What to replace it with
        /// \return The string with all matches replaced
        /// \see b::string::regex_replace_one()
        ///
        static b::string regex_replace(const b::string& str, const b::string& pattern, const b::string& replace);

        ///
        /// \brief Replace the first match of a Regular Expression (RegEx) (Unicode aware)
        /// \param[in] pattern A regex pattern
        /// \param[in] replace What to replace it with
        /// \see b::string::regex_replace_all()
        ///
        void regex_replace_one(const b::string& pattern, const b::string& replace);

        ///
        /// \brief Replace the first match of a Regular Expression (RegEx) (Unicode aware)
        /// \param[in] str The string to be searched
        /// \param[in] pattern A regex pattern
        /// \param[in] replace What to replace it with
        /// \return The string with the first match replaced
        /// \see b::string::regex_replace_all()
        ///
        static b::string regex_replace_one(const b::string& str, const b::string& pattern, const b::string& replace);

        ///
        /// \brief Replace all occurrences of a string with another string using a custom replacer function.
        /// \details The replacer function is called for every occurrence of the pattern.
        /// \param[in] pattern A token to be replaced
        /// \param[in] replacer A small function taking the matched string and the match index,
        ///            and returning the replacement string
        /// \see b::string::regex_replace()
        ///
        void replace(const b::string& pattern, std::function<b::string(const b::string&, int)> replacer);

        ///
        /// \brief Replace all occurrences of a string with another string using a custom replacer function.
        /// \details The replacer function is called for every occurrence of the pattern.
        /// \param[in] str The string to be modified
        /// \param[in] pattern A token to be replaced
        /// \param[in] replacer A small function taking the matched string and the match index,
        ///            and returning the replacement string
        /// \return The modified string
        /// \see b::string::regex_replace()
        ///
        static b::string replace(const b::string& str, const b::string& pattern, std::function<b::string(const b::string&, int)> replacer);

        ///
        /// \brief Replace all occurrences of a string with another string
        /// \param[in] pattern A token to be replaced
        /// \param[in] value What to replace the token with
        /// \see b::string::regex_replace()
        ///
        void replace(const b::string& pattern, const b::string& value);

        ///
        /// \brief Replace all occurrences of a string with another string
        /// \param[in] str The string to be modified
        /// \param[in] pattern A token to be replaced
        /// \param[in] value What to replace the token with
        /// \return The modified string
        /// \see b::string::regex_replace()
        ///
        static b::string replace(const b::string& str, const b::string& pattern, const b::string& value);

        ///
        /// \brief Replace the first occurrence of a string with another string
        /// \param[in] pattern A token to be replaced
        /// \param[in] value What to replace the token with
        /// \see b::string::regex_replace()
        ///
        void replace_first(const b::string& pattern, const b::string& value);

        ///
        /// \brief Replace the first occurrence of a string with another string
        /// \param[in] str The string to be modified
        /// \param[in] pattern A token to be replaced
        /// \param[in] value What to replace the token with
        /// \return The modified string
        /// \see b::string::regex_replace()
        ///
        static b::string replace_first(const b::string& str, const b::string& pattern, const b::string& value);

        ///
        /// \brief Replace the first occurrence of a string with another string
        /// \param[in] pattern A token to be replaced
        /// \param[in] value What to replace the token with
        /// \see b::string::regex_replace()
        ///
        void replace_last(const b::string& pattern, const b::string& value);

        ///
        /// \brief Replace the first occurrence of a string with another string
        /// \param[in] str The string to be modified
        /// \param[in] pattern A token to be replaced
        /// \param[in] value What to replace the token with
        /// \return The modified string
        /// \see b::string::regex_replace()
        ///
        static b::string replace_last(const b::string& str, const b::string& pattern, const b::string& value);

        ///
        /// \brief Make all characters lowercase (Unicode agnostic)
        /// \details Unicode characters are transformed like expected.
        /// \return Reference to self
        /// \see b::string::to_upper()
        ///
        string& to_lower();

        ///
        /// \brief Make all characters uppercase (Unicode agnostic)
        /// \details Unicode characters are transformed like expected. Special case: "ß" is transformed to "SS".
        /// \return Reference to self
        /// \see b::string::to_lower()
        ///
        string& to_upper();

//      Operators

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

//      Iterators

        [[nodiscard]] iterator begin() {    // TODO: Apparently we can use m_data.data()
            return &m_data[0];
        }

        [[nodiscard]] iterator end() {
            return &m_data[0] + m_data.size();
        }

        [[nodiscard]] const_iterator begin() const {
            return &m_data[0];
        }

        [[nodiscard]] const_iterator end() const {
            return &m_data[0] + m_data.size();
        }

        [[nodiscard]] const_iterator cbegin() const {
            return &m_data[0];
        }

        [[nodiscard]] const_iterator cend() const {
            return &m_data[0] + m_data.size();
        }

        [[nodiscard]] reverse_iterator rbegin() {
            return &m_data[0] + m_data.size();
        }

        [[nodiscard]] reverse_iterator rend() {
            return &m_data[0];
        }

        [[nodiscard]] const_reverse_iterator rbegin() const {
            return &m_data[0] + m_data.size();
        }

        [[nodiscard]] const_reverse_iterator rend() const {
            return &m_data[0];
        }

        [[nodiscard]] const_reverse_iterator crbegin() const {
            return &m_data[0] + m_data.size();
        }

        [[nodiscard]] const_reverse_iterator crend() const {
            return &m_data[0];
        }

    private:
        std::u32string m_data;
    };

    ///
    /// \brief Encode a binary string or a binary resource as Base-64.
    /// \details This string can either be a text or a binary-like byte-series (loaded from a file).
    ///          Especially useful for embedding small resource files in a source file
    ///          or exchanging binary resources over a network. The binary resource is stored in the std::string
    ///          as a series of bytes, `std::string` can also hold non-printable characters including zero bytes.
    /// \param[in] str The string or byte-series to encode
    /// \return Encoded Base-64 string
    /// \see b::decode_base64()
    ///
    std::string encode_base64(const std::string& str);

    ///
    /// \brief Decode a Base-64 encoded resource back to its original form.
    /// \details See b::encode_base64() for more information
    /// \param[in] str A Base-64 encoded string representing a resource
    /// \return Decoded resource as a string containing a series of bytes (not necessarily printable characters).
    /// \see b::encode_base64()
    ///
    std::string decode_base64(const std::string& str);

} // namespace b

// Allow b::fs::path to be used with std::ostream
std::ostream& operator<<(std::ostream& stream, const b::string& str);

// Allow b::fs::path to be used with std::istream
std::istream& operator>>(std::istream& stream, b::string& str);

// Hash function that lets b::fs::path be used as a key in std::map and std::unordered_map
namespace std {
    template <> struct hash<b::string> {
        size_t operator()(const b::string& str) const {
            return std::hash<std::string>()(str.to_utf8());
        }
    };
} // namespace std

// Allow b::fs::path to be printed with fmt::format and in logging messages
template <> struct fmt::formatter<b::string> {
    constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator {
        return ctx.begin();
    }
    auto format(const b::string& str, format_context& ctx) const -> format_context::iterator {
        return fmt::format_to(ctx.out(), "{}", str.to_utf8());
    }
};

// Custom string literal to be able to write "Hello"_b to make it a b::string straight away.
[[nodiscard]] b::string operator""_b(const char* str, size_t len);   // TODO: Make these constexpr
[[nodiscard]] b::string operator""_b(const char8_t* str, size_t len);
[[nodiscard]] b::string operator""_b(const char16_t* str, size_t len);
[[nodiscard]] b::string operator""_b(const char32_t* str, size_t len);