#pragma once

#include "Battery/common.h"

namespace Battery::String {

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

}
