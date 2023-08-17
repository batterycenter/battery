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

#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ranges.h>
#include <spdlog/fmt/std.h>
#include <functional>
#include <istream>
#include <optional>
#include <ostream>
#include <regex>
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

    /// \brief Encodings to be used for string encoding, file I/O, etc...
    namespace enc {

        /// \brief `b::enc::utf8`: A flag for specifying UTF-8 encoding
        struct utf8 {};

        /// \brief `b::enc::utf16`: A flag for specifying UTF-16 encoding
        struct utf16 {};

        /// \brief `b::enc::utf16_wide`: A flag for specifying UTF-16 widestring encoding
        struct utf16_wide {};

        /// \brief `b::enc::utf32`: A flag for specifying UTF-32 encoding
        struct utf32 {};

        /// \brief `b::enc::iso_8859_1`: A flag for specifying ISO-8859-1 (also called Latin-1) encoding
        struct iso_8859_1 {};

#ifdef B_OS_WINDOWS
        /// \brief `b::enc::cp1252`: A flag for specifying Windows-1252 encoding (incorrectly known as ANSI)
        /// \note This encoding is only available on Windows
        struct cp1252 {};
#endif

        /// \brief `b::enc::ascii`: A flag for specifying plain 7-bit ASCII encoding
        struct ascii {};

        /// \brief `b::enc::os_native`: The os-native encoding (wide UTF-16 on Windows, UTF-8 on other platforms)
        struct os_native {};

        /// \brief `b::enc::binary`: A flag for specifying binary encoding (referring to non-text data)
        struct binary {};
    }

    /// \brief A higher-level byte array class to be used for binary data
    /// \details This class is a wrapper around std::vector<uint8_t>. The goal is to provide a standalone type for
    ///          binary data, such as binary files, etc. It goes hand-in-hand with the b::string class and the
    ///          filesystem functions. The end goal is that resources are stored as either b::string if they represent
    ///          text, or b::bytearray if they represent binary data (or a text with a specific encoding). Binary data
    ///          should use this class instead of std::string, potentially containing non-printable characters. It
    ///          can also be conveniently formatted and printed.
    class bytearray : public std::vector<uint8_t> {
    public:
        /// \brief Construct an empty byte array
        bytearray() = default;

        /// \brief Construct a byte array from a vector of bytes
        bytearray(const std::vector<uint8_t>& vec) : std::vector<uint8_t>(vec) {}

        /// \brief Construct a byte array from a vector of bytes
        bytearray(std::vector<uint8_t>&& vec) : std::vector<uint8_t>(vec) {}

        /// \brief Construct a byte array from a list of bytes
        bytearray(std::initializer_list<uint8_t> list) : std::vector<uint8_t>(list) {}

        static bytearray from_string(const std::string& str) {
            return { std::vector<uint8_t>(str.begin(), str.end()) };
        }

        std::string str() const {
            return { begin(), end() };
        }

        /// \brief Assign this byte array to another byte array
        bytearray& operator=(const std::vector<uint8_t>& vec) {
            std::vector<uint8_t>::operator=(std::vector<uint8_t>(vec));
            return *this;
        }

        /// \brief Assign this byte array to a vector of bytes
        bytearray& operator=(std::vector<uint8_t>&& vec) {
            std::vector<uint8_t>::operator=(std::vector<uint8_t>(vec));
            return *this;
        }

        /// \brief Assign this byte array to a list of bytes
        bytearray& operator=(std::initializer_list<uint8_t> list) {
            std::vector<uint8_t>::operator=(std::vector<uint8_t>(list));
            return *this;
        }

        /// \brief Decode this byte array from a specific encoding to generic b::string format
        /// \details See b::string::decode for more info
        template<typename encoding>
        auto decode() const;
    };

    // ========================================================
    // =============== Begin b::string class ==================
    // ========================================================

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
    ///          This was chosen over UTF-8 as an internal representation because it makes string manipulation
    ///          significantly easier, since every character is one element in the std::u32string. Iterating over the
    ///          string character by character would be significantly more complicated with UTF-8, as the length of
    ///          each character is variable. Use this class as much as possible throughout your code, and always
    ///          convert to UTF-8 at interfaces. The only exceptions are other libraries, where specific encodings
    ///          are required, such as Windows API functions. Do not ever write text files to disk with anything
    ///          other than UTF-8 encoding, and do not ever send text-based resources over the network in anything
    ///          other than UTF-8. This paradigm is documented at https://utf8everywhere.org/.
    ///
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
        /// \throw b::out_of_range_error if the string is empty
        /// \return A reference to the last character of the string
        char32_t& back();

        /// \brief Get a const reference to the last character of the string (Unicode agnostic)
        /// \throw b::out_of_range_error if the string is empty
        /// \return A const reference to the last character of the string
        const char32_t& back() const;

        /// \brief Get a reference to the first character of the string (Unicode agnostic)
        /// \throw b::out_of_range_error if the string is empty
        /// \return A reference to the first character of the string
        char32_t& front();

        /// \brief Get a const reference to the first character of the string (Unicode agnostic)
        /// \throw b::out_of_range_error if the string is empty
        /// \return A const reference to the first character of the string
        const char32_t& front() const;

        /// \brief Get a reference to a specific character of the string (Unicode agnostic)
        /// \details This function has a const variant. If the index is out of range, b::out_of_range_error is thrown.
        /// \param[in] index The index of the character to get
        /// \throw b::out_of_range_error if the index is out of range
        /// \return A reference to the character at the specified index
        char32_t& at(size_t index);

        /// \brief Get a reference to a specific character of the string (Unicode agnostic)
        /// \details This function has a const variant. If the index is out of range, b::out_of_range_error is thrown.
        /// \param[in] index The index of the character to get
        /// \throw b::out_of_range_error if the index is out of range
        /// \return A reference to the character at the specified index
        [[nodiscard]] const char32_t& at(size_t index) const;

        /// \brief Push a character onto the string and thus append it
        void push_back(char32_t chr);

        /// \brief Pop the last character from the string and return it (Unicode agnostic)
        /// \throw b::out_of_range_error if the string is empty
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
        /// \throw b::out_of_range_error if the start position is out of range
        /// \return The substring
        [[nodiscard]] string substr(size_t pos, size_t len = std::string::npos) const;

        /// \brief static validation function for UTF-8 strings
        /// \param[in] str The UTF-8 encoded string to validate
        /// \return false if the string is not valid UTF-8, true otherwise
        static bool is_valid_utf8(const std::string& str);

        /// \brief An internal struct for `b::string`, not for public use
        struct internal {

            /// \brief Decode a byte series encoded in UTF-8 into generic b::string format.
            /// \throw b::unicode_error if the string is not encoded in valid UTF-8
            /// \return b::string
            static string decode_utf8(const std::string &str);

            /// \brief Decode a 16-bit character string encoded in UTF-16 into generic b::string format.
            /// \throw b::unicode_error if the string is not encoded in valid UTF-16
            /// \return b::string
            static string decode_utf16(const std::u16string &str);

            /// \brief Decode a 32-bit character string encoded in UTF-32 into generic b::string format.
            /// \throw b::unicode_error if the string is not encoded in valid UTF-32
            /// \return b::string
            static string decode_utf32(const std::u32string &str);

            /// \brief Decode a string encoded in ASCII into generic b::string format. (ONLY 7-bit ASCII)
            /// \throw b::unicode_error if the string is not encoded in valid UTF-8
            /// \return b::string
            static string decode_ascii(const std::string &str);

            /// \brief Decode a string encoded in ISO-8859-1 (also called Latin-1) into generic b::string format.
            /// \return b::string
            static string decode_iso8859_1(const std::string &str);

#ifdef B_OS_WINDOWS
            /// \brief Decode a string encoded in Windows-1252 (incorrectly known as ANSI) into generic b::string format.
            /// \return b::string
            static string decode_cp1252(const std::string &str);
#endif

            /// \brief Encode the high-level generic b::string object into a string containing UTF-8 encoded bytes.
            /// \return The string containing UTF-8 encoded data
            static b::bytearray encode_utf8(const b::string& str);

            /// \brief Encode the high-level generic b::string object into a string containing UTF-16 encoded characters.
            /// \return The string containing UTF-16 encoded data
            static b::bytearray encode_utf16(const b::string& str);

            /// \brief Encode the high-level generic b::string object into a string containing UTF-32 encoded characters.
            /// \return The string containing UTF-32 encoded data
            static b::bytearray encode_utf32(const b::string& str);

        }; // struct b::string::internal

        /// \brief Decode a string from a specific encoding into generic b::string format.
        /// \details Encodings are specified as a template parameter. Possible values:
        ///          `b::enc::utf8`, `b::enc::utf16`, `b::enc::utf32`, `b::enc::utf16_wide`,
        ///          `b::enc::ascii`, `b::enc::iso8859_1`, `b::enc::cp1252` (Windows only),
        ///          `b::enc::os_native` and `b::enc::binary`.
        /// \warning Be careful with the distinction between ASCII, cp1252 and ISO-8859-1 (which is Latin-1).
        ///          Encodings are often mislabeled as ASCII when they are in fact something else.
        /// \tparam encoding The encoding of the input string
        /// \param data The input string
        /// \return b::bytearray if b::enc::binary is used, otherwise an encoding-agnostic b::string
        template<typename encoding, typename T>
        static auto decode(const T& data) {
            // !std::is_same_v<encoding, encoding> is used as an assertion trigger
            //   that only triggers when the specific if-branch is compiled

            // UTF-8
            if constexpr (std::is_same_v<encoding, b::enc::utf8>) {
                if constexpr (std::is_convertible_v<T, std::string>) {         // std::string overload
                    return b::string::internal::decode_utf8(data);
                }
                else if constexpr (std::is_convertible_v<T, std::u8string>) {  // std::u8string overload
                    return b::string::internal::decode_utf8(std::string(data.begin(), data.end()));
                }
                else if constexpr (std::is_convertible_v<T, b::bytearray>) {   // b::bytearray overload
                    return b::string::internal::decode_utf8(data.str());
                }
                else {
                    static_assert(!std::is_same_v<encoding, encoding>,
                                  "b::string::decode<>(): `b::enc::utf8` encoding may only take in "
                                  "std::string, std::u8string and b::bytearray");
                }
            }

            // UTF-16
            else if constexpr (std::is_same_v<encoding, b::enc::utf16>) {
                if constexpr (std::is_convertible_v<T, std::u16string>) {         // std::u16string overload
                    return b::string::internal::decode_utf16(data);
                }
                else if constexpr (std::is_convertible_v<T, std::wstring>) {      // std::wstring overload
                    return b::string::internal::decode_utf16(std::u16string(data.begin(), data.end()));
                }
                else if constexpr (std::is_convertible_v<T, b::bytearray>) {      // b::bytearray overload
                    return b::string::internal::decode_utf16(
                            std::u16string(std::bit_cast<const char16_t*>(data.data()), data.length() / 2)
                    );
                }
                else {
                    static_assert(!std::is_same_v<encoding, encoding>,
                                  "b::string::decode<>(): `b::enc::utf16` encoding may only take in "
                                  "std::u16string, std::wstring and b::bytearray");
                }
            }

            // UTF-16 widestring
            else if constexpr (std::is_same_v<encoding, b::enc::utf16_wide>) {
                if constexpr (std::is_convertible_v<T, std::u16string>) {         // std::u16string overload
                    return b::string::internal::decode_utf16(data);
                }
                else if constexpr (std::is_convertible_v<T, std::wstring>) {      // std::wstring overload
                    return b::string::internal::decode_utf16(std::u16string(data.begin(), data.end()));
                }
                else if constexpr (std::is_convertible_v<T, b::bytearray>) {      // b::bytearray overload
                    return b::string::internal::decode_utf16(
                            std::u16string(std::bit_cast<const char16_t*>(data.data()), data.length() / 2)
                    );
                }
                else {
                    static_assert(!std::is_same_v<encoding, encoding>,
                                  "b::string::decode<>(): `b::enc::utf16_wide` encoding may only take in "
                                  "std::u16string, std::wstring and b::bytearray");
                }
            }

            // UTF-32
            else if constexpr (std::is_same_v<encoding, b::enc::utf32>) {
                if constexpr (std::is_convertible_v<T, std::u32string>) {         // std::u32string overload
                    return b::string::internal::decode_utf32(data);
                }
                else if constexpr (std::is_same_v<T, char32_t>) {                 // char32_t overload
                    return b::string::internal::decode_utf32(std::u32string(&data, 1));
                }
                else if constexpr (std::is_convertible_v<T, b::bytearray>) {      // b::bytearray overload
                    return b::string::internal::decode_utf32(
                            std::u32string(std::bit_cast<const char32_t*>(data.data()), data.length() / 4)
                    );
                }
                else {
                    static_assert(!std::is_same_v<encoding, encoding>,
                                  "b::string::decode<>(): `b::enc::utf32` encoding may only take in "
                                  "std::u32string, char32_t and b::bytearray");
                }
            }

            // ISO-8859-1 (also called Latin-1)
            else if constexpr (std::is_same_v<encoding, b::enc::iso_8859_1>) {
                if constexpr (std::is_convertible_v<T, std::string>) {            // std::string overload
                    return b::string::internal::decode_iso8859_1(data);
                }
                else if constexpr (std::is_same_v<T, char>) {                     // char overload
                    return b::string::internal::decode_iso8859_1(std::string(&data, 1));
                }
                else if constexpr (std::is_convertible_v<T, b::bytearray>) {      // b::bytearray overload
                    return b::string::internal::decode_iso8859_1(data.str());
                }
                else {
                    static_assert(!std::is_same_v<encoding, encoding>,
                                  "b::string::decode<>(): `b::enc::iso_8859_1` encoding may only take in "
                                  "std::string, char and b::bytearray");
                }
            }

            // Windows-1252 (Windows only)
#ifdef B_OS_WINDOWS
            else if constexpr (std::is_same_v<encoding, b::enc::cp1252>) {
                if constexpr (std::is_convertible_v<T, std::string>) {            // std::string overload
                    return b::string::internal::decode_cp1252(data);
                }
                else if constexpr (std::is_same_v<T, char>) {                     // char overload
                    return b::string::internal::decode_cp1252(std::string(&data, 1));
                }
                else if constexpr (std::is_convertible_v<T, b::bytearray>) {      // b::bytearray overload
                    return b::string::internal::decode_cp1252(data.str());
                }
                else {
                    static_assert(!std::is_same_v<encoding, encoding>,
                                  "b::string::decode<>(): `b::enc::cp1252` encoding may only take in "
                                  "std::string, char and b::bytearray");
                }
            }
#endif

            // ASCII (in this case parsed as UTF-8 as it is backwards-compatible)
            else if constexpr (std::is_same_v<encoding, b::enc::ascii>) {
                if constexpr (std::is_convertible_v<T, std::string>) {         // std::string overload
                    return b::string::internal::decode_utf8(data);
                }
                else if constexpr (std::is_same_v<T, char>) {           // char overload
                    return b::string::internal::decode_utf8(std::string(&data, 1));
                }
                else if constexpr (std::is_convertible_v<T, b::bytearray>) {   // b::bytearray overload
                    return b::string::internal::decode_utf8(data.str());
                }
                else {
                    static_assert(!std::is_same_v<encoding, encoding>,
                                  "b::string::decode<>(): `b::enc::ascii` encoding may only take in "
                                  "std::string, char and b::bytearray");
                }
            }

            // OS-Native
            else if constexpr (std::is_same_v<encoding, b::enc::os_native>) {
#ifdef B_OS_WINDOWS
                return b::string::decode<b::enc::utf16>(data);
#else
                return b::string::decode<b::enc::utf8>(data);
#endif
            }

            // Binary
            else if constexpr (std::is_same_v<encoding, b::enc::binary>) {
                if constexpr (std::is_convertible_v<T, std::string>) {   // b::bytearray overload
                    return b::bytearray::from_string(data);
                }
                else {
                    static_assert(!std::is_same_v<encoding, encoding>,
                                  "b::string::decode<>(): `b::enc::binary` encoding may only take in "
                                  "std::string");
                }
            }

            else {
                static_assert(!std::is_same_v<encoding, encoding>,
                              "b::string::decode<>(): Unsupported encoding specified in template parameter");
                return b::string();
            }
        }

        /// \brief Encode a string from generic b::string format into a `b::bytearray` with a specified encoding
        /// \details Encodings are specified as a template parameter. Possible values:
        ///          `b::enc::utf8`, `b::enc::utf16`, `b::enc::utf32`, `b::enc::utf16_wide`,
        ///          `b::enc::ascii`, `b::enc::iso8859_1`, `b::enc::cp1252` (Windows only),
        ///          `b::enc::os_native` and `b::enc::binary`.
        /// \warning Be careful with the distinction between ASCII, cp1252 and ISO-8859-1 (which is Latin-1).
        ///          Encodings are often mislabeled as ASCII when they are in fact something else.
        /// \warning Make sure to follow https://utf8everywhere.org/. Do not ever write non-UTF-8 to a file.
        /// \tparam encoding The desired encoding of the output data
        /// \param str The input string
        /// \return b::bytearray containing the encoded string
        template<typename encoding>
        static b::bytearray encode_bytes(const b::string& str) {
            // !std::is_same_v<encoding, encoding> is used as an assertion trigger
            //   that only triggers when the specific if-branch is compiled

            // UTF-8
            if constexpr (std::is_same_v<encoding, b::enc::utf8>) {
                return b::string::internal::encode_utf8(str);
            }

            // UTF-16
            else if constexpr (std::is_same_v<encoding, b::enc::utf16>) {
                return b::string::internal::encode_utf16(str);
            }

            // UTF-16 widestring
            else if constexpr (std::is_same_v<encoding, b::enc::utf16_wide>) {
                return b::string::internal::encode_utf16(str);
            }

            // UTF-32
            else if constexpr (std::is_same_v<encoding, b::enc::utf32>) {
                return b::string::internal::encode_utf32(str);
            }

            // OS-Native
            else if constexpr (std::is_same_v<encoding, b::enc::os_native>) {
#ifdef B_OS_WINDOWS
                return b::string::internal::encode_utf16(str);
#else
                return b::string::internal::encode_utf8(str);
#endif
            }

            // Binary
            else if constexpr (std::is_same_v<encoding, b::enc::binary>) {
                static_assert(!std::is_same_v<encoding, encoding>,
                              "b::string::encode_bytes<>(): There is no such thing as 'binary' string encoding. "
                              "If you just wanted a b::bytearray instead of a string, "
                              "use b::string::encode_bytearray()");
            }

            else {
                static_assert(!std::is_same_v<encoding, encoding>,
                              "b::string::encode_bytes<>(): The specified encoding is intentionally "
                              "not implemented for this function");
            }
        }

        /// \brief Encode this string into a `b::bytearray` with a specified encoding
        /// \details Encodings are specified as a template parameter. Possible values:
        ///          `b::enc::utf8`, `b::enc::utf16`, `b::enc::utf32`, `b::enc::utf16_wide`,
        ///          `b::enc::ascii`, `b::enc::iso8859_1`, `b::enc::cp1252` (Windows only),
        ///          `b::enc::os_native` and `b::enc::binary`.
        /// \warning Be careful with the distinction between ASCII, cp1252 and ISO-8859-1 (which is Latin-1).
        ///          Encodings are often mislabeled as ASCII when they are in fact something else.
        /// \warning Make sure to follow https://utf8everywhere.org/. Do not ever write non-UTF-8 to a file.
        /// \tparam encoding The desired encoding of the output data
        /// \param str The input string
        /// \return b::bytearray
        template<typename encoding>
        b::bytearray encode_bytes() const {
            return b::string::encode_bytes<encoding>(*this);
        }

        /// \brief Encode a string from generic b::string format into a fitting string type with a specified encoding
        /// \details Encodings are specified as a template parameter. Possible values:
        ///          `b::enc::utf8`, `b::enc::utf16`, `b::enc::utf32`, `b::enc::utf16_wide`,
        ///          `b::enc::ascii`, `b::enc::iso8859_1`, `b::enc::cp1252` (Windows only),
        ///          `b::enc::os_native` and `b::enc::binary`.
        /// \warning Be careful with the distinction between ASCII, cp1252 and ISO-8859-1 (which is Latin-1).
        ///          Encodings are often mislabeled as ASCII when they are in fact something else.
        /// \warning Make sure to follow https://utf8everywhere.org/. Do not ever write non-UTF-8 to a file.
        /// \tparam encoding The desired encoding of the output data
        /// \param str The input string
        /// \return The fitting string type for the encoding, such as std::u8string, std::u32string, etc...
        template<typename encoding>
        static auto encode(const b::string& str) {

            // UTF-8
            if constexpr (std::is_same_v<encoding, b::enc::utf8>) {
                auto bytearray = encode_bytes<encoding>(str);
                return std::string(std::bit_cast<char*>(bytearray.data()), bytearray.size());
            }

            // UTF-16
            else if constexpr (std::is_same_v<encoding, b::enc::utf16>) {
                auto bytearray = encode_bytes<encoding>(str);
                return std::u16string(std::bit_cast<char16_t*>(bytearray.data()), bytearray.size() / 2);
            }

            // UTF-16 widestring
            else if constexpr (std::is_same_v<encoding, b::enc::utf16_wide>) {
                auto bytearray = encode_bytes<encoding>(str);
                return std::wstring(std::bit_cast<wchar_t*>(bytearray.data()), bytearray.size() / 2);
            }

            // UTF-32
            else if constexpr (std::is_same_v<encoding, b::enc::utf32>) {
                auto bytearray = encode_bytes<encoding>(str);
                return std::u32string(std::bit_cast<char32_t*>(bytearray.data()), bytearray.size() / 4);
            }

            // OS-Native
            else if constexpr (std::is_same_v<encoding, b::enc::os_native>) {
                auto bytearray = encode_bytes<encoding>(str);
#ifdef B_OS_WINDOWS
                return std::wstring(std::bit_cast<wchar_t*>(bytearray.data()), bytearray.size() / 2);
#else
                return std::string(std::bit_cast<char*>(bytearray.data()), bytearray.size());
#endif
            }

            else {
                static_assert(!std::is_same_v<encoding, encoding>,
                              "b::string::encode<>(): The specified encoding is intentionally "
                              "not implemented for this function");
            }
        }

        /// \brief Encode this string into a fitting string type with a specified encoding
        /// \details Encodings are specified as a template parameter. Possible values:
        ///          `b::enc::utf8`, `b::enc::utf16`, `b::enc::utf32`, `b::enc::utf16_wide`,
        ///          `b::enc::ascii`, `b::enc::iso8859_1`, `b::enc::cp1252` (Windows only),
        ///          `b::enc::os_native` and `b::enc::binary`.
        /// \warning Be careful with the distinction between ASCII, cp1252 and ISO-8859-1 (which is Latin-1).
        ///          Encodings are often mislabeled as ASCII when they are in fact something else.
        /// \warning Make sure to follow https://utf8everywhere.org/. Do not ever write non-UTF-8 to a file.
        /// \tparam encoding The desired encoding of the output data
        /// \param str The input string
        /// \return The fitting string type for the encoding, such as std::string, std::u32string, etc...
        template<typename encoding>
        auto encode() const {
            return b::string::encode<encoding>(*this);
        }

        /// \brief Encode explicitly into UTF-8 std::u8string instead of std::string. Other encodings are unchanged.
        /// \tparam encoding The desired encoding of the output data
        /// \param str The input string
        /// \return The fitting string type for the encoding, such as std::u8string, std::u32string, etc...
        template<typename encoding>
        static auto encode_u8(const b::string& str) {
            if constexpr (std::is_same_v<encoding, b::enc::utf8>) {
                auto std = encode<encoding>(str);
                return std::u8string(std.begin(), std.end());
            }
            else {
                return encode<encoding>(str);
            }
        }

        /// \brief Encode explicitly into UTF-8 std::u8string instead of std::string. Other encodings are unchanged.
        /// \tparam encoding The desired encoding of the output data
        /// \param str The input string
        /// \return The fitting string type for the encoding, such as std::u8string, std::u32string, etc...
        template<typename encoding>
        auto encode_u8() const {
            return b::string::encode_u8<encoding>(*this);
        }

        /// \brief Conversion operator to std::u8string by encoding this string as UTF-8.
        /// \return std::u8string containing UTF-8 encoded data
        [[nodiscard]] operator std::u8string() const;

        /// \brief Conversion operator to std::u16string by encoding this string as UTF-16.
        /// \return std::u8string containing UTF-16 encoded data
        [[nodiscard]] operator std::u16string() const;

        /// \brief Conversion operator to std::u32string by encoding this string as UTF-32.
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
        /// \throw b::out_of_range_error if the index is out of bounds
        /// \return A reference to the character at the index
        char32_t& operator[](size_t index);

        /// \brief Get a character at a specific index.
        /// \param[in] index The index of the character
        /// \throw b::out_of_range_error if the index is out of bounds
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

    /// \brief Implementation of the `b::bytearray::decode` function
    template<typename encoding>
    auto bytearray::decode() const {
        return b::string::decode<encoding>(*this);
    }

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

/// \brief fmt formatter to allow formatting b::bytearray with fmt::format
template <> struct fmt::formatter<b::bytearray> {
    constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator {
        return ctx.begin();
    }
    auto format(const b::bytearray& array, format_context& ctx) const -> format_context::iterator {
        return fmt::format_to(ctx.out(), "{}", static_cast<std::vector<uint8_t>>(array));
    }
};

/// \brief fmt formatter to allow formatting b::string with fmt::format
template <> struct fmt::formatter<b::string> {
    constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator {
        return ctx.begin();
    }
    auto format(const b::string& str, format_context& ctx) const -> format_context::iterator {
        return fmt::format_to(ctx.out(), "{}", str.encode<b::enc::utf8>());
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