///
/// \file string.h
/// \author Florian Zachs
///
#pragma once

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <functional>

#include "battery/core/environment.h"

///
/// \brief battery::string module -> Various string manipulation utilities
/// \addtogroup string
/// @{
///
namespace battery::string {

#ifdef BATTERY_ARCH_WINDOWS
    using osstring = std::wstring;
#else
    using osstring = std::string;
#endif

    ///
    /// \brief Convert an std::string containing utf-8 encoded bytes to utf-16 on Windows, unchanged on other platforms.
    ///        This function is exclusively for use with the Win32 API
    /// \param[in] str The string to be converted
    /// \throw std::invalid_argument on an invalid utf-8 sequence on Windows, none on other platforms
    /// \return The converted string, std::wstring on Windows, std::string on other platforms
    /// \see battery::string::osstring_to_utf8()
    ///
    osstring utf8_to_osstring(const std::string& str);

    ///
    /// \brief Convert an std::wstring to an std::string on Windows, returns the unchanged string on other platforms.
    ///        This function is exclusively for use with the Win32 API
    /// \param[in] str The string to be converted, std::wstring on Windows, std::string on other platforms
    /// \throw std::invalid_argument on an invalid utf-16 sequence on Windows, none on other platforms
    /// \return The converted string
    /// \see battery::string::utf8_to_osstring()
    ///
    std::string osstring_to_utf8(const osstring& str);

    ///
    /// \brief Convert a utf-8 encoded multibyte string to a utf-32 encoded string.
    /// \param[in] str The string to be converted
    /// \throw std::invalid_argument on an invalid utf-8 sequence
    /// \return The converted string
    /// \see battery::string::utf32_to_utf8()
    ///
    std::u32string utf8_to_utf32(const std::string& str);

    ///
    /// \brief Convert a utf-32 encoded string back to a utf-8 encoded string.
    /// \param[in] str The string to be converted
    /// \throw std::invalid_argument on an invalid utf-32 sequence
    /// \return The converted string
    /// \see battery::string::utf8_to_utf32()
    ///
    std::string utf32_to_utf8(const std::u32string& str);

    ///
    /// \brief Convert a single unicode character into its utf-8 encoded form
    /// \param[in] c The character to be converted
    /// \throw std::invalid_argument on an invalid unicode character
    /// \return The utf-8 multibyte string representing the unicode character
    ///
    std::string to_utf8(char32_t c);

    ///
    /// \brief Call a function for every codepoint in the utf-8 sequence. This can be used to do operations on a string
    ///        whereas the function is called for every character (not for every byte). The return values are appended
    ///        to one another and returned as another string. Thus, directly returning the character in the callback
    ///        would make this function return the string unchanged.
    /// \param[in] str The string over which to iterate
    /// \param[in] function A callback which is called for every character or codepoint in the string
    /// \throw std::invalid_argument on an invalid utf-8 sequence
    /// \see battery::string::utf8_foreach_char32()
    /// \return All return values from the callbacks appended into a string
    ///
    std::string foreach(const std::string& str, std::function<std::variant<std::string,char32_t>(std::string)> function);
    std::string foreach(const std::string& str, std::function<std::variant<std::string,char32_t>(char32_t)> function);

    ///
    /// \brief Split a string into an array of string pieces by a delimeter character. When no delimeter is found,
    /// 	   only the input string is returned. Several delimeters in a row are treated as a single one
    /// \param[in] str The string to be split
    /// \param[in] delimeter The character to split the string at
    /// \return An array of string tokens
    /// \see battery::string::join()
    ///
    std::vector<std::string> split(const std::string& str, char delimeter);

    ///
    /// \brief Takes an array of string tokens and joins them into a single string. The spacer string
    ///        is inserted between any two string tokens being joined (empty by default)
    /// \param[in] strings Array of string tokens to be joined
    /// \param[in] spacer Spacer string
    /// \return The combined string
    /// \see battery::string::split()
    ///
    std::string join(const std::vector<std::string>& strings, const std::string& spacer = "");

    ///
    /// \brief Replace parts of a string with another string. The parameter `string` is iterated over
    ///        and any occurrences of the string `from` are replaced with the string `to`.
    /// \param[in] string The string to be modified
    /// \param[in] from A token to be replaced
    /// \param[in] to What to replace the token with
    /// \return The modified string
    /// \see battery::string::replace_one()
    ///
    std::string replace(std::string string, const std::string& from, const std::string& to);

    ///
    /// \brief Replace one part of a string with another string. The parameter `string` is iterated over
    ///        and only one occurrence of the string `from` is replaced with the string `to`.
    /// \param[in] string The string to be modified
    /// \param[in] from A token to be replaced
    /// \param[in] to What to replace the token with
    /// \param[in] occurrence The index of the occurrence. 0 would mean the first occurrence, 2 the third.
    ///                       Additionally, -1 would be the last occurrence and -2 the second-to-last.
    /// \todo Implement this function properly. At the moment, it does the same as Battery::String::Replace()
    /// 	  and replaces all occurrences
    /// \return The modified string
    /// \see battery::string::replace()
    ///
    std::string replace_one(std::string string, const std::string& from, const std::string& to, int occurrence = 0);

    ///
    /// \brief Convert an std::u8string to an std::string. The returned string is a one-to-one copy of the
    ///        utf-8 encoded bytes. No encoding is changed. This function cannot fail.
    /// \param[in] str The string to be converted
    /// \return The converted string
    /// \see battery::string::string_to_u8string()
    ///
    std::string u8string_to_string(const std::u8string& str);

    ///
    /// \brief Convert an std::string to an std::u8string. The input is expected to be a string containing utf-8 encoded bytes.
    ///        If the input contains an invalid utf-8 sequence, an exception is thrown.
    /// \param[in] str The string to be converted
    /// \throw std::invalid_argument on an invalid utf-8 sequence
    /// \return The converted string.
    /// \see battery::string::u8string_to_string()
    ///
    std::u8string string_to_u8string(const std::string& str);

    ///
    /// \brief Make all characters of a utf-8 string lowercase. Throws on error
    /// \param[in] str The utf-8 string to be converted
    /// \throw std::invalid_argument on an invalid utf-8 sequence
    /// \return The converted utf-8 string
    /// \see battery::string::to_uppercase()
    ///
    std::string to_lowercase(const std::string& str);

    ///
    /// \brief Make all characters of a utf-8 string uppercase. Throws on error
    /// \param[in] str The utf-8 string to be converted
    /// \throw std::invalid_argument on an invalid utf-8 sequence
    /// \return The converted utf-8 string
    /// \see battery::string::to_lowercase()
    ///
    std::string to_uppercase(const std::string& str);

    ///
    /// \brief Encode a string as Base-64. This string can either be a text or a binary-like byte-series (loaded from a file).
    ///        Especially useful for embedding small resource files in a source file or exchanging binary resources over a network.
    /// \param[in] str The string or byte-series to encode
    /// \return Encoded Base-64 string
    /// \see battery::string::decode_base64()
    ///
    std::string encode_base64(const std::string& str);

    ///
    /// \brief Decode a Base-64 encoded string back to its original form. See battery::string::decode_base64()
    /// \param[in] str A Base-64 encoded string representing a resource
    /// \return Decoded resource as a string containing a series of bytes (not necessarily printable characters).
    /// \see battery::string::encode_base64()
    ///
    std::string decode_base64(const std::string& str);

}
