#pragma once

#include "Battery/pch.h"
#include "Battery/Core/AllegroContext.h"

#include <string.h>

namespace Battery {
	namespace StringUtils {

		/// <summary>
		/// Split a string into an array of string pieces by a delimeter character. For example: pass "This is a test." 
		/// with delimeter ' ' (space) and get a vector: {"This", "is", "a", "test."}. 
		/// The delimeter character is not included in the results. When no delimeter is found, the entire string is returned
		/// as element 0. Delimeters at the beginning and at the end of the string are ignored and several delimeters in a row are
		/// treated as if there were only one.
		/// </summary>
		/// <param name="str">- The string to split</param>
		/// <param name="delimeter">- A single character at which to split the string</param>
		/// <returns>std::vector&lt;std::string&gt; - An array with the split string fragments</returns>
		std::vector<std::string> SplitString(std::string str, char delimeter);

		/// <summary>
		/// Join any number of strings and combine it into a single long one with spacers inbetween.
		/// The spacer string can be any length and is joined inbetween every string fragment, but not 
		/// at the end. For example: Give {"This", "is", "a", "test"} with spacer " " (space) returns "This is a test".
		/// Can be used to join strings with "\n" inbetween, so it can be saved as a file. When the spacer string
		/// is empty (""), no spacer is inserted.
		/// </summary>
		/// <param name="strings">- A vector with strings to join</param>
		/// <param name="spacer">- The spacer which is inserted between the string fragments, defaults to ""</param>
		/// <returns>std::string - The final joined string</returns>
		std::string JoinStrings(std::vector<std::string> strings, std::string spacer = "");



		std::string ToUpperCase(const std::string& str);
		std::string ToLowerCase(const std::string& str);

		char ToUpperCase(char c);
		char ToLowerCase(char c);

		bool IsUpperCase(char c);
		bool IsLowerCase(char c);

		bool IsInAlphabet(char c);

		std::wstring MultiByteToWideChar(const std::string& mbString);
		std::string WideCharToMultiByte(const std::wstring& wString);
		std::string WideCharToMultiByte(const wchar_t* wString);	// Windows' LPCWSTR

		std::string GetLastErrorAsString();

		template <size_t length>
		void ConvertCodepointToUTF8(int32_t codepoint, char (&buffer)[length]) {

			ALLEGRO_USTR* ustr = al_ustr_new("");
			al_ustr_append_chr(ustr, codepoint);

			strncpy_s(buffer, al_cstr(ustr), length);

			al_ustr_free(ustr);
		}
	}
}
