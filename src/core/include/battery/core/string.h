#pragma once
///
/// \file StringUtils.h
/// \author Florian Zachs
///

#include "battery/core/common.h"

///
/// \brief Battery::String module -> Various string manipulation utilities
/// \addtogroup string
/// @{
///
namespace battery::string {

    ///
    /// \brief Split a string into an array of string pieces by a delimeter character. When no delimeter is found,
    /// 	   only the input string is returned. Several delimeters in a row are treated as a single one
    /// \param[in] str The string to be split
    /// \param[in] delimeter The character to split the string at
    /// \return An array of string tokens
    /// \see battery::string::join()
    ///
    std::vector<std::string> split(std::string str, char delimeter);

    ///
    /// \brief Takes an array of string tokens and joins them into a single string. The spacer string
    ///        is inserted between any two string tokens being joined (empty by default)
    /// \param[in] strings Array of string tokens to be joined
    /// \param[in] spacer Spacer string
    /// \return The combined string
    /// \see battery::string::split()
    ///
    std::string join(std::vector<std::string> strings, std::string spacer = "");

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

}
