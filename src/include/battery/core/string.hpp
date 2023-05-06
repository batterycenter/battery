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

#include "battery/core/environment.hpp"
#include "battery/core/log.hpp"

///
/// \brief Everything related to string manipulation and encoding
/// \addtogroup String
/// @{
///
namespace b {

#ifdef BATTERY_ARCH_WINDOWS
    ///
    /// \brief A string type used for interfacing with the corresponding operating system.
    /// \details This type is defined as `std::wstring` on Windows, designed to hold UTF-16 encoded strings for working
    ///          with the Win32 API exclusively. On all other platforms it is defined as `std::string`. Thus,
    ///          `b::to_osstring(str).c_str()` will always give you a c-style char array, UTF-8 on Unix and UTF-16 on Windows.
    ///          See `b::to_osstring` for possible caveats!
    /// \see b::to_osstring()
    ///
    using osstring = std::wstring;
#else
    ///
    /// \brief A string type used for interfacing with the corresponding operating system.
    /// \details This type is defined as `std::wstring` on Windows, designed to hold UTF-16 encoded strings for working
    ///          with the Win32 API exclusively. On all other platforms it is defined as `std::string`. Thus,
    ///          `b::to_osstring(str).c_str()` will always give you a c-style char array, UTF-8 on Unix and UTF-16 on Windows.
    ///          See `b::to_osstring` for possible caveats!
    /// \see b::to_osstring()
    ///
    using osstring = std::string;
#endif

    ///
    /// \brief Convert a UTF-8 `std::string` to its platform agnostic version.
    /// \details Converting to UTF-16 on Windows, returning the unchanged string on other platforms.
    ///          `b::to_osstring(str).c_str()` will always give you a c-style char array, UTF-8 on Unix and UTF-16 on Windows.
    ///          But beware of the lifetime, in this case the pointer is only valid as long as the osstring object is alive and it
    ///          only is for the current expression. Thus, if you have a Win32 API or Linux API function that holds onto
    ///          the pointer or you store the pointer in a struct, then you must save the osstring in a variable, call `.c_str()`
    ///          on the variable and make sure that the variable outlives the data structure.
    ///          See `b::to_u8()` and `b::to_u32()` for the reverse operation.
    /// \param[in] str The string to be converted
    /// \throw std::invalid_argument on an invalid utf-8 sequence
    /// \return std::wstring on Windows converted to UTF-16, std::string on other platforms with the unchanged string
    /// \see b::osstring_to_u8()
    /// \see b::osstring_to_u32()
    /// \see b::to_u8()
    /// \see b::to_u32()
    ///
    b::osstring to_osstring(const std::u8string& str);

    ///
    /// \brief u32 overload of `b::to_osstring()`
    /// \param[in] str The string to be converted
    /// \throw std::invalid_argument on an invalid utf-32 sequence
    /// \return std::wstring on Windows converted to UTF-16, std::string on other platforms with the unchanged string
    /// \see b::osstring_to_u8()
    /// \see b::osstring_to_u32()
    /// \see b::to_u8()
    /// \see b::to_u32()
    ///
    b::osstring to_osstring(const std::u32string& str);

    ///
    /// \brief Convert a platform-native string like Windows UTF-16 wide-strings to standardized UTF-8
    /// \param[in] str The string to be converted
    /// \throw std::invalid_argument on an invalid unicode character
    /// \return std::u8string the input string converted to UTF-8
    /// \see b::to_osstring()
    ///
    std::u8string osstring_to_u8(const b::osstring& str);

    ///
    /// \brief Convert a platform-native string like Windows UTF-16 wide-strings to standardized UTF-32
    /// \param[in] str The string to be converted
    /// \throw std::invalid_argument on an invalid unicode character
    /// \return std::u32string the input string converted to UTF-32
    /// \see b::to_osstring()
    ///
    std::u32string osstring_to_u32(const b::osstring& str);

    ///
    /// \brief Convert an std::u8string to a plain std::string
    /// \details No conversion is happening in this function call. The bytes of the u8string are transferred
    ///          to the std::string one by one. This results in an std::string that contains utf-8 encoded bytes.
    ///          This function cannot fail.
    /// \param[in] str The string to be converted
    /// \return std::string containing the same bytes as the input string
    /// \see b::u8_from_std_string()
    ///
    std::string u8_as_str(const std::u8string& str);

    ///
    /// \brief Interpret an std::string as a utf-8 encoded string and return it as an std::u8string
    /// \details The input string is parsed as a utf-8 encoded string and checked for errors. If no invalid utf-8
    ///          sequence is found, the bytes are transferred directly into an std::u8string and returned.
    ///          Otherwise, an exception is thrown.
    /// \param[in] str The string to be converted
    /// \throw std::invalid_argument on an invalid utf-8 sequence
    /// \return std::u8string containing the same bytes as the input string
    /// \see b::u8_as_str()
    ///
    std::u8string u8_from_std_string(const std::string& str);

    ///
    /// \brief Check if a string contains valid utf-8 encoding
    /// \param[in] str The string to be checked
    /// \return False on an invalid utf-8 sequence, true otherwise
    ///
    bool is_valid_u8(const std::string& str);

    ///
    /// \brief Check if a string contains valid utf-8 encoding
    /// \param[in] str The string to be checked
    /// \return False on an invalid utf-8 sequence, true otherwise
    ///
    bool is_valid_u8(const std::u8string& str);

    ///
    /// \brief Check if a string contains valid utf-8 encoding
    /// \param[in] str The string to be checked
    /// \return False on an invalid utf-8 sequence, true otherwise
    ///
    bool is_valid_u32(const std::u32string& str);

    ///
    /// \brief Convert a UTF-32 encoded string back to a UTF-8 encoded string.
    /// \param[in] str The string to be converted
    /// \throw std::invalid_argument on an invalid UTF-32 character
    /// \return The converted string
    /// \see b::to_u32()
    ///
    std::u8string to_u8(const std::u32string& str);

    ///
    /// \brief Convert a single unicode character into its UTF-8 encoded multibyte form
    /// \param[in] c The character to be converted
    /// \throw std::invalid_argument on an invalid unicode character
    /// \return The UTF-8 multibyte string representing the unicode character
    /// \see b::to_u32()
    ///
    std::u8string to_u8(char32_t c);

    ///
    /// \brief Convert a UTF-8 encoded multibyte string to a UTF-32 encoded string.
    /// \param[in] str The string to be converted
    /// \throw std::invalid_argument on an invalid UTF-8 sequence
    /// \return The converted string
    /// \see b::to_u8()
    ///
    std::u32string to_u32(const std::u8string& str);

    ///
    /// \brief Convert a single unicode character into a UTF-32 encoded string containing only that character
    /// \param[in] c The character to be converted
    /// \return The converted string
    /// \see b::to_u8()
    ///
    std::u32string to_u32(char32_t c);




    ///
    /// \brief Call a function for every character in a UTF-8 encoded string, not for every byte.
    /// \details This can be used to do operations on a string
    ///          whereas the function is called for every character (not for every byte). The return values are appended
    ///          to one another and returned as another string. Thus, directly returning the character in the callback
    ///          would make this function return the string unchanged. The callback can return either a char32_t or a UTF-8
    ///          string, whereas the string can also have more or fewer characters. Returning `""` essentially removes it.
    ///          Example usage: `auto new = b::u8_foreach(str, [] (std::string c) { return c == "ß" ? "SS" : c; })`
    /// \param[in] str The string over which to iterate
    /// \param[in] function A callback which is called for every character or codepoint in the string
    /// \throw std::invalid_argument on an invalid UTF-8 sequence
    /// \return All return values from the callbacks appended into a string
    /// \see b::u32_foreach()
    ///
    std::u8string u8_foreach(const std::u8string& str, const std::function<std::variant<std::u8string,char32_t>(std::u8string)>& function);

    ///
    /// \brief Call a function for every character in a UTF-8 encoded string. Check the overload for more info.
    /// \details This is an overload that supplies the callback with `char32_t` instead of `std::string`.
    ///          Please see `b::u8_foreach()` for any other details.
    /// \see b::u32_foreach()
    ///
    std::u8string u8_foreach(const std::u8string& str, const std::function<std::variant<std::u8string,char32_t>(char32_t)>& function);




    ///
    /// \brief Call a function for every character in a UTF-32 encoded string
    /// \details Please see `b::u8_foreach()` for any other details. This function is equivalent,
    ///          but for UTF-32 instead of UTF-8.
    /// \param[in] str The string over which to iterate
    /// \param[in] function A callback which is called for every character or codepoint in the string
    /// \throw std::invalid_argument on an invalid UTF-32 sequence
    /// \return All return values from the callbacks appended into a string
    /// \see b::u8_foreach()
    ///
    std::u32string u32_foreach(const std::u32string& str, const std::function<std::variant<std::u32string,char32_t>(std::u32string)>& function);

    ///
    /// \brief Call a function for every character in a UTF-32 encoded string. Check the overload for more info.
    /// \details This is an overload that supplies the callback with `char32_t` instead of `std::u32string`.
    ///          Please see `b::u32_foreach()` for any other details.
    /// \see b::u8_foreach()
    ///
    std::u32string u32_foreach(const std::u32string& str, const std::function<std::variant<std::u32string,char32_t>(char32_t)>& function);



    ///
    /// \brief Split a string into an array of string pieces by a delimiter.
    /// \details When no delimiter is found, only the input string is returned. Several delimiters in a row are treated
    ///          as a single one. The delimeter can be one or more bytes, thus this function is UTF-8 conformant.
    /// \param[in] str The string to be split
    /// \param[in] delimiter The characters to split the string at
    /// \return An array of string tokens
    /// \see b::join()
    ///
    std::vector<std::string> split(std::string str, const std::string& delimiter);

    ///
    /// \brief Overload for `b::split()` for std::u8string
    /// \see b::join()
    ///
    std::vector<std::u8string> split(std::u8string str, const std::u8string& delimiter);

    ///
    /// \brief Overload for `b::split()` for std::u32string
    /// \see b::join()
    ///
    std::vector<std::u32string> split(std::u32string str, const std::u32string& delimiter);



    ///
    /// \brief Join an array of strings into a single one. To be used with `b::split()`.
    /// \details The spacer string is inserted between any two string tokens being joined.
    /// \param[in] strings Array of string tokens to be joined
    /// \param[in] spacer Spacer string
    /// \return The combined string
    /// \see b::split()
    ///
    std::string join(const std::vector<std::string>& strings, const std::string& spacer = "");

    ///
    /// \brief Overload for `b::join()` for std::u8string
    /// \see b::split()
    ///
    std::u8string join(const std::vector<std::u8string>& strings, const std::u8string& spacer = u8"");

    ///
    /// \brief Overload for `b::join()` for std::u32string
    /// \see b::split()
    ///
    std::u32string join(const std::vector<std::u32string>& strings, const std::u32string& spacer = U"");



    ///
    /// \brief Replace parts of a string with another string.
    /// \details The parameter `string` is iterated over and any occurrences of `from` are replaced by `to`.
    /// \param[in] string The string to be modified
    /// \param[in] from A token to be replaced
    /// \param[in] to What to replace the token with
    /// \return The modified string
    /// \see b::replace_one()
    ///
    std::string replace(std::string string, const std::string& from, const std::string& to);

    ///
    /// \brief Overload for `b::replace()` for std::u8string
    /// \see b::replace_one()
    ///
    std::u8string replace(std::u8string string, const std::u8string& from, const std::u8string& to);

    ///
    /// \brief Overload for `b::replace()` for std::u32string
    /// \see b::replace_one()
    ///
    std::u32string replace(std::u32string string, const std::u32string& from, const std::u32string& to);



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
    /// \see b::replace()
    /// \see b::replace_first()
    ///
    std::string replace_one(std::string string, const std::string& from, const std::string& to, int occurrence);

    ///
    /// \brief Overload for `b::replace_one()` for std::u8string
    /// \see b::replace_one()
    ///
    std::u8string replace_one(std::u8string string, const std::u8string& from, const std::u8string& to, int occurrence);

    ///
    /// \brief Overload for `b::replace_one()` for std::u32string
    /// \see b::replace_one()
    ///
    std::u32string replace_one(std::u32string string, const std::u32string& from, const std::u32string& to, int occurrence);




    ///
    /// \brief Replace the first occurrence of a string with another string.
    /// \details Please see `b::replace_one()`. This function is just a specialization
    ///          with the last parameter already set.
    /// \see b::replace_last()
    ///
    std::string replace_first(const std::string& string, const std::string& from, const std::string& to);

    ///
    /// \brief Overload for `b::replace_first()` for std::u8string
    /// \see b::replace_last()
    ///
    std::u8string replace_first(const std::u8string& string, const std::u8string& from, const std::u8string& to);

    ///
    /// \brief Overload for `b::replace_first()` for std::u32string
    /// \see b::replace_last()
    ///
    std::u32string replace_first(const std::u32string& string, const std::u32string& from, const std::u32string& to);




    ///
    /// \brief Replace the last occurrence of a string with another string.
    /// \details Please see `b::replace_one()`. This function is just a specialization
    ///          with the last parameter already set.
    /// \see b::replace_first()
    ///
    std::string replace_last(const std::string& string, const std::string& from, const std::string& to);

    ///
    /// \brief Overload for `b::replace_last()` for std::u8string
    /// \see b::replace_first()
    ///
    std::u8string replace_last(const std::u8string& string, const std::u8string& from, const std::u8string& to);

    ///
    /// \brief Overload for `b::replace_last()` for std::u32string
    /// \see b::replace_first()
    ///
    std::u32string replace_last(const std::u32string& string, const std::u32string& from, const std::u32string& to);






    ///
    /// \brief Make all characters of a UTF-8 string lowercase.
    /// \details Unicode characters are transformed like expected.
    /// \param[in] str The UTF-8 string to be converted
    /// \throw std::invalid_argument on an invalid UTF-8 sequence
    /// \return The converted UTF-8 string
    /// \see b::to_upper()
    ///
    std::string to_lower(const std::string& str);

    ///
    /// \brief Overload for `b::to_lower()` for std::u8string
    /// \see b::to_upper()
    ///
    std::u8string to_lower(const std::u8string& str);

    ///
    /// \brief Overload for `b::to_lower()` for std::u32string
    /// \see b::to_upper()
    ///
    std::u32string to_lower(const std::u32string& str);




    ///
    /// \brief Make all characters of a UTF-8 string uppercase.
    /// \details Unicode characters are transformed like expected. "ß" is transformed to "SS".
    /// \param[in] str The UTF-8 string to be converted
    /// \throw std::invalid_argument on an invalid UTF-8 sequence
    /// \return The converted UTF-8 string
    /// \see b::to_lower()
    ///
    std::string to_upper(const std::string& str);

    ///
    /// \brief Overload for `b::to_upper()` for std::u8string
    /// \see b::to_lower()
    ///
    std::u8string to_upper(const std::u8string& str);

    ///
    /// \brief Overload for `b::to_upper()` for std::u32string
    /// \see b::to_lower()
    ///
    std::u32string to_upper(const std::u32string& str);




    // TODO: Support REGEX

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

// Make std::u8string printable in streams
inline std::ostream& operator<<(std::ostream& os, const std::u8string& str) {
    os << b::u8_as_str(str);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const std::u32string& str) {
    os << b::to_u8(str);
    return os;
}
