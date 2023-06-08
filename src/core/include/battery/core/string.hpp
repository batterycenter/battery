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

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <functional>

///
/// \brief Everything related to string manipulation and encoding
/// \addtogroup String
/// @{
///
namespace b {

    /// \brief platform native string type: std::wstring on Windows, std::string on other platforms
#ifdef B_OS_WINDOWS
    using platform_native_string = std::wstring;
#else
    using platform_native_string = std::string;
#endif

    ///
    /// \brief Yet another string class, that combines all other. Synonymous to `std::string`, but with more features.
    /// \details This class is derived from `std::string`, thus all `std::string` features are directly available.
    ///          You are encouraged to use this class wherever possible. What separates it from `std::string` is that
    ///          it takes in all other forms of strings (std::u8string, wide strings on windows, sf::String, etc.) and
    ///          automatically performs Unicode conversions when needed. It is basically a container for everything
    ///          related to Unicode, using it everywhere makes sure that your program is automatically
    ///          Unicode aware out of the box.
    ///
    ///          Every function that takes a string expects it to be in either UTF-8, UTF-16 or UTF-32.
    ///          No other encoding is supported as you are discouraged to use anything but Unicode.
    ///
    ///          It also provides a few convenience functions for string manipulation and formatting.
    ///
    class string : public std::string {
    public:

        /// \brief static validation function for UTF-8 strings
        /// \param[in] str The UTF-8 encoded string to validate
        /// \return false if the string is not valid UTF-8, true otherwise
        static bool is_valid_utf8(const std::string& str);

        /// \brief b::string default constructor
        string() = default;

        /// \brief b::string forwarding constructor to forward any other non-implemented parameters to std::string
        template<typename... TArgs>
        string(const TArgs&... args) : std::string(args...) {}

        /// \brief b::string constructor taking char
        /// \param[in] str The character to convert
        string(char c) : std::string(&c, 1) {}

        /// \brief b::string constructor taking char8_t
        /// \param[in] str The character to convert
        string(char8_t c) : std::string(u8_to_str(std::u8string(&c, 1))) {}

        /// \brief b::string constructor taking char16_t
        /// \param[in] str The character to convert
        string(char16_t c) : std::string(u16_to_str(std::u16string(&c, 1))) {}

        /// \brief b::string constructor taking char32_t
        /// \param[in] str The character to convert
        string(char32_t c) : std::string(u32_to_str(std::u32string(&c, 1))) {}

        /// \brief b::string constructor taking const char*
        /// \param[in] str The string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-8
        string(const char* str) : std::string(str) {}

        /// \brief b::string constructor taking const char8_t*
        /// \param[in] str The string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-8
        string(const char8_t* str) : std::string(u8_to_str(str)) {}

        /// \brief b::string constructor taking const char16_t*
        /// \param[in] str The string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-8
        string(const char16_t* str) : std::string(u16_to_str(str)) {}

        /// \brief b::string constructor taking const char32_t*
        /// \param[in] str The string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-8
        string(const char32_t* str) : std::string(u32_to_str(str)) {}

        /// \brief b::string constructor taking std::string
        /// \param[in] str The string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-8
        string(const std::string& str) : std::string(str) {}

        /// \brief b::string constructor taking std::u8string
        /// \param[in] str The u8string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-8
        string(const std::u8string& str) : std::string(u8_to_str(str)) {}

        /// \brief b::string constructor taking std::u16string
        /// \param[in] str The u16string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-16
        string(const std::u16string& str) : std::string(u16_to_str(str)) {}

        /// \brief b::string constructor taking std::u32string
        /// \param[in] str The u32string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-32
        string(const std::u32string& str) : std::string(u32_to_str(str)) {}

        /// \brief b::string constructor taking std::wstring
        /// \param[in] str The platform native wide string to convert
        /// \throw std::invalid_argument if the conversion does not result in valid UTF-8
        string(const std::wstring& str) : std::string(wstr_to_str(str)) {}

        /// \brief assignment operator taking char
        /// \param[in] other The string to convert
        /// \return reference to itself
        string& operator=(char c) { *static_cast<std::string*>(this) = std::string(c, 1); return *this; }

        /// \brief assignment operator taking char8_t
        /// \param[in] other The string to convert
        /// \return reference to itself
        string& operator=(char8_t c) { *static_cast<std::string*>(this) = std::string(c, 1); return *this; }

        /// \brief assignment operator taking char16_t
        /// \param[in] other The string to convert
        /// \return reference to itself
        string& operator=(char16_t c) { *static_cast<std::string*>(this) = std::string(c, 1); return *this; }

        /// \brief assignment operator taking char32_t
        /// \param[in] other The string to convert
        /// \return reference to itself
        string& operator=(char32_t c) { *static_cast<std::string*>(this) = std::string(c, 1); return *this; }

        /// \brief assignment operator taking const char*
        /// \param[in] other The string to convert
        /// \return reference to itself
        string& operator=(const char* other) { *static_cast<std::string*>(this) = other; return *this; }

        /// \brief assignment operator taking const char8_t*
        /// \param[in] other The string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-8
        /// \return reference to itself
        string& operator=(const char8_t* other) { *static_cast<std::string*>(this) = u8_to_str(other); return *this; }

        /// \brief assignment operator taking const char16_t*
        /// \param[in] other The string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-8
        /// \return reference to itself
        string& operator=(const char16_t* other) { *static_cast<std::string*>(this) = u16_to_str(other); return *this; }

        /// \brief assignment operator taking const char32_t*
        /// \param[in] other The string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-8
        /// \return reference to itself
        string& operator=(const char32_t* other) { *static_cast<std::string*>(this) = u32_to_str(other); return *this; }

        /// \brief assignment operator taking std::string
        /// \param[in] other The string to convert
        /// \return reference to itself
        string& operator=(const std::string& other) { *static_cast<std::string*>(this) = other; return *this; }

        /// \brief assignment operator taking std::u8string
        /// \param[in] other The u8string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-8
        /// \return reference to itself
        string& operator=(const std::u8string& other) { *static_cast<std::string*>(this) = u8_to_str(other); return *this; }

        /// \brief assignment operator taking std::u16string
        /// \param[in] other The u16string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-16
        /// \return reference to itself
        string& operator=(const std::u16string& other) { *static_cast<std::string*>(this) = u16_to_str(other); return *this; }

        /// \brief assignment operator taking std::u32string
        /// \param[in] other The u8string to convert
        /// \throw std::invalid_argument if the string is not valid UTF-32
        /// \return reference to itself
        string& operator=(const std::u32string& other) { *static_cast<std::string*>(this) = u32_to_str(other); return *this; }

        /// \brief assignment operator taking std::wstring
        /// \param[in] other The platform native wide string to convert
        /// \throw std::invalid_argument if the conversion does not result in valid UTF-8
        /// \return reference to itself
        string& operator=(const std::wstring& other) { *static_cast<std::string*>(this) = wstr_to_str(other); return *this; }

        /// \brief conversion operator to std::u8string
        /// \throw std::invalid_argument if the string does not contain valid UTF-8
        /// \return std::u8string containing the UTF-8 representation of the string
        explicit operator std::u8string() const { return u8(); }

        /// \brief conversion operator to std::u16string
        /// \throw std::invalid_argument if the string does not contain valid UTF-8
        /// \return std::u16string containing the UTF-16 representation of the string
        explicit operator std::u16string() const { return u16(); }

        /// \brief conversion operator to std::u32string
        /// \throw std::invalid_argument if the string does not contain valid UTF-8
        /// \return std::u32string containing the UTF-32 representation of the string
        explicit operator std::u32string() const { return u32(); }

        /// \brief conversion operator to std::wstring
        /// \warning You are not encouraged to use `std::wstring` anywhere, except for platform specific APIs.
        /// \throw std::invalid_argument if the string does not contain valid UTF-8
        /// \return std::wstring containing the UTF-16 wide-string representation of the string
        explicit operator std::wstring() { return wstr(); }

        /// \brief conversion function to std::string reference
        /// \details This function provides a non-const reference to the std::string base class. Thus, this function could
        ///          theoretically be used to modify the base string directly. This does not mean you should.
        /// \return The raw std::string itself
        std::string& str() { return *this; }

        /// \brief conversion function to const std::string reference
        /// \return The raw std::string itself
        [[nodiscard]] const std::string& str() const { return *this; }

        /// \brief conversion function to std::u8string
        /// \throw std::invalid_argument if the string does not contain valid UTF-8
        /// \return std::u8string containing the UTF-8 representation of the string
        [[nodiscard]] std::u8string u8() const;

        /// \brief conversion function to std::u16string
        /// \throw std::invalid_argument if the string does not contain valid UTF-8
        /// \return std::u16string containing the UTF-8 representation of the string
        [[nodiscard]] std::u16string u16() const;

        /// \brief conversion function to std::u32string
        /// \throw std::invalid_argument if the string does not contain valid UTF-8
        /// \return std::u32string containing the UTF-8 representation of the string
        [[nodiscard]] std::u32string u32() const;

        /// \brief conversion function to std::wstring
        /// \warning You are not encouraged to use `std::wstring` anywhere, except for platform specific APIs.
        /// \details This function stores the wide string representation inside the string and returns a reference to it.
        ///          This makes the lifetime of the return value the lifetime of this b::string. Returning a copy would not.
        /// \throw std::invalid_argument if the string does not contain valid UTF-8
        /// \return std::wstring containing the UTF-16 wide-string representation of the string
        const std::wstring& wstr();

        /// \brief convert to a platform native string: std::wstring on Windows, std::string on other platforms
        /// \details Calling `string.platform_native().c_str()` returns a string pointer in the platform-native encoding
        ///          (std::wstring on Windows, std::string on other platforms), which is valid until the function is
        ///          called again or the object is destroyed.
        /// \return std::wstring or std::string depending on the platform
        const platform_native_string& platform_native();

        ///
        /// \brief Call a function for every character in a UTF-8 encoded string, not for every byte.
        /// \details This can be used to do operations on a string
        ///          whereas the function is called for every character (not for every byte). The return values are appended
        ///          to one another and returned as another string. Thus, directly returning the character in the callback
        ///          would make this function return the string unchanged. The callback can return either a char32_t or a UTF-8
        ///          string, whereas the string can also have more or fewer characters. Returning `""` essentially removes it.
        ///          Example usage: `auto new = b::string::foreach(str, [] (std::string c) { return c == "ß" ? "SS" : c; })`
        /// \param[in] str The string over which to iterate
        /// \param[in] function A callback which is called for every character or codepoint in the string
        /// \throw std::invalid_argument on an invalid UTF-8 sequence
        /// \return All return values from the callbacks appended into a string
        static b::string foreach(const b::string& str, const std::function<b::string(b::string)>& function);

        ///
        /// \brief Call a function for every character in a UTF-8 encoded string. Check the overload for more info.
        /// \details This is an overload that supplies the callback with `char32_t` instead of `std::string`.
        ///          Please see `b::string::foreach()` for any other details.
        static b::string foreach(const b::string& str, const std::function<b::string(char32_t)>& function);

        ///
        /// \brief Split a string into an array of string pieces by a delimiter.
        /// \details When no delimiter is found, only the input string is returned. Several delimiters in a row are treated
        ///          as a single one. The delimeter can be one or more bytes, thus this function is UTF-8 conformant.
        /// \param[in] str The string to be split
        /// \param[in] delimiter The characters to split the string at
        /// \return An array of string tokens
        /// \see b::string::join()
        ///
        static std::vector<b::string> split(b::string str, const b::string& delimiter);

        ///
        /// \brief Join an array of strings into a single one. To be used with `b::split()`.
        /// \details The spacer string is inserted between any two string tokens being joined.
        /// \param[in] strings Array of string tokens to be joined
        /// \param[in] spacer Spacer string
        /// \return The combined string
        /// \see b::string::split()
        ///
        static b::string join(const std::vector<b::string>& strings, const b::string& delimiter);

        ///
        /// \brief Replace parts of a string with another string.
        /// \details The parameter `string` is iterated over and any occurrences of `from` are replaced by `to`.
        /// \param[in] string The string to be modified
        /// \param[in] from A token to be replaced
        /// \param[in] to What to replace the token with
        /// \return The modified string
        /// \see b::string::replace_one()
        ///
        static b::string replace(b::string string, const b::string& from, const b::string& to);

        ///
        /// \brief Replace one part of a string with another string.
        /// \details The parameter `string` is iterated over and only one occurrence of the string `from`
        ///          is replaced with the string `to`.
        /// \param[in] string The string to be modified
        /// \param[in] from A token to be replaced
        /// \param[in] to What to replace the token with
        /// \param[in] occurrence The index of the occurrence. 0 would mean the first occurrence, 1 the second.
        ///                       Additionally, -1 would be the last occurrence and -2 the second-to-last.
        /// \return The modified string
        /// \see b::string::replace()
        /// \see b::string::replace_first()
        ///
        static b::string replace_one(b::string string, const b::string& from, const b::string& to, int occurrence);

        ///
        /// \brief Replace the first occurrence of a string with another string.
        /// \details Please see `b::replace_one()`. This function is just a specialization
        ///          with the last parameter already set.
        /// \see b::string::replace_last()
        ///
        static b::string replace_first(const b::string& string, const b::string& from, const b::string& to);

        ///
        /// \brief Replace the last occurrence of a string with another string.
        /// \details Please see `b::replace_one()`. This function is just a specialization
        ///          with the last parameter already set.
        /// \see b::string::replace_first()
        ///
        static b::string replace_last(const b::string& string, const b::string& from, const b::string& to);

        ///
        /// \brief Make all characters of a UTF-8 string lowercase.
        /// \details Unicode characters are transformed like expected.
        /// \param[in] str The UTF-8 string to be converted
        /// \throw std::invalid_argument on an invalid UTF-8 sequence
        /// \return The converted UTF-8 string
        /// \see b::string::to_upper()
        ///
        static b::string to_lower(const b::string& str);

        ///
        /// \brief Make all characters of a UTF-8 string uppercase.
        /// \details Unicode characters are transformed like expected. Special case: "ß" is transformed to "SS".
        /// \param[in] str The UTF-8 string to be converted
        /// \throw std::invalid_argument on an invalid UTF-8 sequence
        /// \return The converted UTF-8 string
        /// \see b::string::to_lower()
        ///
        static b::string to_upper(const b::string& str);

    private:
        std::string u8_to_str(const std::u8string& str) const;
        std::string u16_to_str(const std::u16string& str) const;
        std::string u32_to_str(const std::u32string& str) const;
        std::string wstr_to_str(const std::wstring& str) const;

        std::wstring internal_wide_string;      // This is set when wstr() is called to manage the lifetime of the wide string
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

// Hash function that lets b::string be used as a key in std::map and std::unordered_map
namespace std {
    template <>
    struct hash<b::string> {
        size_t operator()(const b::string& str) const {
            return std::hash<std::string>()(str);
        }
    };
}
