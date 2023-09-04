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

#include <functional>
#include <regex>
#include <string>
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

    /// \brief Convert a string to a wide string
    /// \param[in] str The string to convert
    /// \return The wide string
    [[nodiscard]] std::wstring widen(const std::string& str);

    /// \brief Convert a wide string to a string
    /// \param[in] str The wide string to convert
    /// \return The string
    [[nodiscard]] std::string narrow(const std::wstring& str);

    /// \brief Convert a string to a platform native string
    /// \details On Windows, this is equivalent to `b::widen()`.
    ///          On other platforms, the unchanged string is returned.
    /// \param[in] str The string to convert
    /// \return The platform native string
#ifdef B_OS_WINDOWS
    inline auto to_native = [](const std::string& str) -> b::native_string { return b::widen(str); };
#else
    inline auto to_native = [](const std::string& str) -> b::native_string { return str; };
#endif

    /// \brief Convert a string to a platform native string
    /// \details On Windows, this is equivalent to `b::widen()`.
    ///          On other platforms, the unchanged string is returned.
    /// \param[in] str The string to convert
    /// \return The platform native string
#ifdef B_OS_WINDOWS
    inline auto from_native = [](const std::wstring& str) -> std::string { return b::narrow(str); };
#else
    inline auto from_native = [](const b::native_string& str) -> std::string { return str; };
#endif

    /// \brief Reverse a string
    /// \param[0] str The string to reverse
    /// \return The reversed string
    [[nodiscard]] std::string reverse(std::string str);

    /// \brief Validation for UTF-8 strings
    /// \param[in] str The UTF-8 encoded string to validate
    /// \return false if the string is not valid UTF-8, true otherwise
    [[nodiscard]] bool is_utf8(const std::string& str);

    /// \brief Conversion from UTF-8 to UTF-32. Useful for iterating over Unicode codepoints.
    /// \param[in] str The UTF-8 encoded string to convert
    /// \return the UTF-32 encoded string
    [[nodiscard]] std::u32string to_utf32(const std::string& str);

    /// \brief Conversion from UTF-32 to UTF-8
    /// \param[in] str The UTF-32 encoded string to convert
    /// \return the UTF-8 encoded string
    [[nodiscard]] std::string to_utf8(const std::u32string& str);

    /// \brief Split a string into an array of string pieces by a delimiter.
    /// \details When no delimiter is found, only the input string is returned. Several delimiters in a row
    ///          are treated as a single one. The delimiter can be one or more characters.
    /// \param[in] str The string to be split
    /// \param[in] delimiter The characters to split the string at
    /// \return An array of string tokens
    /// \see b::string::join()
    [[nodiscard]] std::vector<std::string> split(std::string str, const std::string& delimiter);

    /// \brief Join an array of strings into a single one.
    /// \details The spacer string is inserted between any two string tokens being joined.
    /// \param[in] strings Array of string tokens to be joined
    /// \param[in] spacer Spacer string
    /// \return The combined string
    /// \see b::string::split()
    /// \see b::string::rjoin()
    [[nodiscard]] std::string join(const std::vector<std::string>& strings, const std::string& spacer);

    /// \brief Join an array of strings in reversed order into a single one.
    /// \details The spacer string is inserted between any two string tokens being joined.
    /// \param[in] strings Array of string tokens to be joined
    /// \param[in] spacer Spacer string
    /// \return The combined string
    /// \see b::string::join()
    [[nodiscard]] std::string rjoin(std::vector<std::string> strings, const std::string& spacer);

    /// \brief Replace all matches of a Regular Expression (RegEx)
    /// \details The last parameter is used to further define the processing of the regex.
    ///          Use it to only replace the first match, specify non-global regex, multiline, etc.
    ///          If the regex in generic form is `/[a-z]/g`, pass `"[a-z]"_b` as the pattern, and 'g'
    ///          is already a default flag. If there were any other characters after the last '/',
    ///          pass the corresponding flags as the last parameter. It is advised to use raw string
    ///          literals for the pattern, to avoid having to escape backslashes.
    ///          e.g. `/a-z\(\)0-9/g` can be written as `R"(a-z\(\)0-9)"_b` instead of `"a-z\\(\\)0-9"_b`
    /// \param[in] str The string to be manipulated
    /// \param[in] pattern A regex pattern
    /// \param[in] replace What to replace it with
    /// \param[in] flags Regex flags of type std::regex_constants::...
    /// \return The string with all matches replaced
    [[nodiscard]] std::string regex_replace(const std::string& str, const std::string& pattern,
                                            const std::string& replace,
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
    /// \param[in] str The string to be manipulated
    /// \param[in] pattern A regex pattern
    /// \param[in] flags Regex flags of type std::regex_constants::...
    /// \return An array with all matches
    [[nodiscard]] std::vector<std::string> regex_match(const std::string& str,
                                                       const std::string& pattern,
                                                       std::regex_constants::match_flag_type flags =
                                                             std::regex_constants::match_default);

    /// \brief Replace all occurrences of a string with another string using a custom replacer function.
    /// \details The replacer function is called for every occurrence of the pattern.
    /// \param[in] str The string to be modified
    /// \param[in] pattern A token to be replaced
    /// \param[in] replacer A small function taking the matched string and the match index,
    ///            and returning the replacement string
    /// \return The modified string
    /// \see b::string::regex_replace()
    [[nodiscard]] std::string replace(const std::string& str, const std::string& pattern,
                                      std::function<std::string(int)> replacer);

    /// \brief Replace all occurrences of a string with another string (simple find and replace)
    /// \param[in] str The string to be modified
    /// \param[in] pattern A token to be replaced
    /// \param[in] value What to replace the token with
    /// \return The modified string
    /// \see b::string::regex_replace()
    [[nodiscard]] std::string replace(const std::string& str, const std::string& pattern, const std::string& value);

    /// \brief Replace the first occurrence of a string with another string (simple find and replace)
    /// \param[in] str The string to be modified
    /// \param[in] pattern A token to be replaced
    /// \param[in] value What to replace the token with
    /// \return The modified string
    /// \see b::string::regex_replace()
    [[nodiscard]] std::string replace_first(const std::string& str, const std::string& pattern, const std::string& value);

    /// \brief Replace the last occurrence of a string with another string (simple find and replace)
    /// \param[in] str The string to be modified
    /// \param[in] pattern A token to be replaced
    /// \param[in] value What to replace the token with
    /// \return The modified string
    /// \see b::string::regex_replace()
    [[nodiscard]] std::string replace_last(const std::string& str, const std::string& pattern, const std::string& value);

    /// \brief Make all characters lowercase
    /// \details Unicode characters are transformed like expected.
    /// \return Reference to self
    /// \see b::string::to_upper()
    [[nodiscard]] std::string to_lower(const std::string& str);

    /// \brief Make all characters uppercase
    /// \details Unicode characters are transformed like expected. Special case: "ß" is transformed to "SS".
    /// \return Reference to self
    /// \see b::string::to_lower()
    [[nodiscard]] std::string to_upper(const std::string& str);

} // namespace b

#endif // BATTERY_CORE_STRING_HPP