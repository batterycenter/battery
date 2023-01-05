#pragma once
///
/// \file StringUtils.h
/// \author Florian Zachs
///

#include "Battery/common.h"

/// 
/// \brief Battery::String module -> Various string manipulation utilities
/// \addtogroup String
/// @{
/// 
namespace Battery::String {

	/// 
	/// \brief Split a string into an array of string pieces by a delimeter character. When no delimeter is found,
	/// 	   only the input string is returned. Several delimeters in a row are treated as a single one
	/// \param[in] str The string to be split
	/// \param[in] delimeter The character to split the string at
	/// \return An array of string tokens
	/// \see Battery::String::Join()
	/// 
	std::vector<std::string> Split(std::string str, char delimeter);
	
	/// 
	/// \brief Takes an array of string tokens and joins them into a single string. The spacer string
	///        is inserted between any two string tokens being joined (empty by default)
	/// \param[in] strings Array of string tokens to be joined
	/// \param[in] spacer Spacer string
	/// \return The combined string
	/// \see Battery::String::Split()
	/// 
	std::string Join(std::vector<std::string> strings, std::string spacer = "");

	/// 
	/// \brief Replace parts of a string with another string. The parameter `string` is iterated over
	///        and any occurrences of the string `from` are replaced with the string `to`.
	/// \param[in] string The string to be modified
	/// \param[in] from A token to be replaced
	/// \param[in] to What to replace the token with
	/// \return The modified string
	/// \see Battery::String::ReplaceOne()
	/// 
    std::string Replace(std::string string, const std::string& from, const std::string& to);

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
	/// \see Battery::String::Replace()
	/// 
    std::string ReplaceOne(std::string string, const std::string& from, const std::string& to, int occurrence = 0);

	/// 
	/// \brief Convert a string to all-uppercase. Only ASCII-letters between 'a' and 'z' are influenced.
	///        No Unicode support.
	/// \param[in] str The string to be modified
	/// \return The modified string
	/// \see Battery::String::ToLowerCase()
	/// 
	std::string ToUpperCase(const std::string& str);
	
	/// 
	/// \brief Convert a string to all-lowercase. Only ASCII-letters between 'A' and 'Z' are influenced.
	///        No Unicode support.
	/// \param[in] str The string to be modified
	/// \return The modified string
	/// \see Battery::String::ToUpperCase()
	/// 
	std::string ToLowerCase(const std::string& str);

	/// 
	/// \brief Convert a character to uppercase. Only ASCII-letters between 'a' and 'z' are influenced.
	///        No Unicode support.
	/// \param[in] str The character to be modified
	/// \return The modified character
	/// \see Battery::String::ToLowerCase()
	/// 
	char ToUpperCase(char c);
	
	/// 
	/// \brief Convert a character to lowercase. Only ASCII-letters between 'A' and 'Z' are influenced.
	///        No Unicode support.
	/// \param[in] str The character to be modified
	/// \return The modified character
	/// \see Battery::String::ToUpperCase()
	/// 
	char ToLowerCase(char c);

	/// 
	/// \brief Check if a character is uppercase. Returns true only if the character 
	///        is part of the uppercase ASCII-alphabet, in other words between 'A' and 'Z'.
	///        No Unicode support.
	/// \param[in] str The character to be checked
	/// \return True if it's alphabetical and uppercase, false otherwise
	/// \see Battery::String::IsLowerCase()
	/// \see Battery::String::IsInAlphabet()
	/// 
	bool IsUpperCase(char c);

	/// 
	/// \brief Check if a character is lowercase. Returns true only if the character 
	///        is part of the lowercase ASCII-alphabet, in other words between 'a' and 'z'.
	///        No Unicode support.
	/// \param[in] str The character to be checked
	/// \return True if it's alphabetical and lowercase, false otherwise
	/// \see Battery::String::IsUpperCase()
	/// \see Battery::String::IsInAlphabet()
	/// 
	bool IsLowerCase(char c);

	/// 
	/// \brief Check if a character is part of the ASCII-alphabet. Returns true if the character 
	///        is either a lowercase ASCII-letter or an uppercase ASCII-letter. (A to Z). No Unicode support.
	/// \param[in] str The character to be checked
	/// \return True if it's alphabetical, false otherwise
	/// \see Battery::String::IsLowerCase()
	/// \see Battery::String::IsUpperCase()
	/// 
	bool IsInAlphabet(char c);

	/// 
	/// \brief Encode a string as Base-64. This string can either be a text or a binary-like byte-series (loaded from a file).
	///        Especially useful for embedding small resource files in a source file or exchanging binary resources over a network.
	/// \param[in] str The string or byte-series to encode
	/// \return Encoded Base-64 string
	/// \see Battery::String::DecodeBase64()
	/// 
    std::string EncodeBase64(const std::string& str);
	
	/// 
	/// \brief Encode a string as Base-64. This string can either be a text or a binary-like byte-series (loaded from a file).
	///        Especially useful for embedding small resource files in a source file or exchanging binary resources over a network.
	/// \param[in] buffer A pointer pointing to an array of characters or bytes
	/// \param[in] buffer_size The length of the array
	/// \return Encoded Base-64 string
	/// \see Battery::String::DecodeBase64()
	/// 
	std::string EncodeBase64(const void* buffer, size_t buffer_size);
	
	/// 
	/// \brief Decode a Base-64 encoded string back to its original form.
	/// \param[in] data A Base-64 encoded string representing a resource
	/// \return Decoded resource as a C++11 byte array. Use `std::string(vec.begin(),vec.end())`
	///         to convert return value to a string.
	/// \see Battery::String::EncodeBase64()
	/// 
    std::vector<uint8_t> DecodeBase64(const std::string& data);
	
	/// 
	/// \brief Decode a Base-64 encoded string back to its original form.
	/// \param[in] buffer A pointer pointing to an array of characters, containing a Base-64 encoded string
	/// \param[in] buffer_size The length of the string
	/// \return Decoded resource as a C++11 byte array. Use `std::string(vec.begin(),vec.end())` 
	///         to convert return value to a string.
	/// \see Battery::String::EncodeBase64()
	/// 
	std::vector<uint8_t> DecodeBase64(const void* buffer, size_t buffer_size);

}
