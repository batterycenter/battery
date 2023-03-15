#pragma once
///
/// \file StringUtils.h
/// \author Florian Zachs
///

#include <string>
#include <vector>
#include <optional>

#include "battery/core/environment.h"

///
/// \brief Battery::String module -> Various string manipulation utilities
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
