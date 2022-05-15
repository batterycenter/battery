
#include "Battery/pch.h"
#include "Battery/StringUtils.h"
#include "Battery/Core/AllegroContext.h"

namespace Battery {
	namespace StringUtils {

		std::vector<std::string> SplitString(std::string str, char delimeter) {
			std::string::size_type b = 0;
			std::vector<std::string> result;

			while ((b = str.find_first_not_of(delimeter, b)) != std::string::npos) {
				auto e = str.find_first_of(delimeter, b);
				result.push_back(str.substr(b, e - b));
				b = e;
			}

			return result;
		}

		std::string JoinStrings(std::vector<std::string> strings, std::string spacer) {
			std::string str = "";

			for (size_t i = 0; i < strings.size(); i++) {
				str += strings[i];

				if (i < strings.size() - 1 && spacer != "") {
					str += spacer;
				}
			}

			return str;
		}





		std::string ToUpperCase(const std::string& str) {
			std::string s;
			for (size_t i = 0; i < str.length(); i++) {
				s += ToUpperCase(str[i]);
			}
			return s;
		}

		std::string ToLowerCase(const std::string& str) {
			std::string s;
			for (size_t i = 0; i < str.length(); i++) {
				s += ToLowerCase(str[i]);
			}
			return s;
		}

		char ToUpperCase(char c) {
			if (IsLowerCase(c)) {
				return c - 32;
			}
			else {
				return c;
			}
		}

		char ToLowerCase(char c) {
			if (IsUpperCase(c)) {
				return c + 32;
			}
			else {
				return c;
			}
		}

		bool IsUpperCase(char c) {
			return c >= 'A' && c <= 'Z';
		}

		bool IsLowerCase(char c) {
			return c >= 'a' && c <= 'z';
		}

		bool IsInAlphabet(char c) {
			return IsUpperCase(c) || IsLowerCase(c);
		}

		std::wstring MultiByteToWideChar(const std::string& mbString) {

			if (mbString.length() == 0)
				return L"";

			size_t bufferSize = std::mbstowcs(nullptr, mbString.c_str(), 0);
			std::wstring wtext(bufferSize, 0);

			std::mbstowcs(&wtext[0], mbString.c_str(), wtext.size());

			return wtext;
		}

		std::string WideCharToMultiByte(const std::wstring& wString) {
			return WideCharToMultiByte(wString.c_str());
		}

		std::string WideCharToMultiByte(const wchar_t* wString) {

			if (wcslen(wString) == 0)
				return "";

			size_t bufferSize = std::wcstombs(nullptr, wString, 0);
			std::string text(bufferSize, 0);

			std::wcstombs(&text[0], wString, text.size());

			return text;
		}

		void ConvertCodepointToUTF8(int32_t codepoint, char* buffer, size_t length) {

			ALLEGRO_USTR* ustr = al_ustr_new("");
			al_ustr_append_chr(ustr, codepoint);

			strncpy(buffer, al_cstr(ustr), length);

			al_ustr_free(ustr);
		}

	}
}
