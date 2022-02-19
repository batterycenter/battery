
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

			size_t bufferSize;
			mbstowcs_s(&bufferSize, nullptr, 0, mbString.c_str(), 0);

			if (bufferSize == 0)
				throw Battery::Exception(__FUNCTION__"(): BufferSize is 0!");

			std::wstring wtext(bufferSize, 0);

			size_t bytesConverted;
			mbstowcs_s(&bytesConverted, &wtext[0], wtext.size(), mbString.c_str(), mbString.length());

			return wtext;
		}

		std::string WideCharToMultiByte(const std::wstring& wString) {

			if (wString.length() == 0)
				return "";

			size_t bufferSize;
			wcstombs_s(&bufferSize, nullptr, 0, wString.c_str(), 0);

			if (bufferSize == 0)
				throw Battery::Exception(__FUNCTION__"(): BufferSize is 0!");

			std::string str(bufferSize, 0);

			size_t bytesConverted;
			wcstombs_s(&bytesConverted, &str[0], str.size(), wString.c_str(), wString.size());

			return str;
		}

		// TODO: Convert to std::wcstombs functions, not C-style

		std::string WideCharToMultiByte(const wchar_t* wString) {

			if (wString == nullptr)
				return "";

			if (wcslen(wString) == 0)
				return "";

			size_t bufferSize;
			wcstombs_s(&bufferSize, nullptr, 0, wString, 0);

			if (bufferSize == 0)
				throw Battery::Exception(__FUNCTION__"(): BufferSize is 0!");

			std::string str(bufferSize, 0);

			size_t bytesConverted;
			wcstombs_s(&bytesConverted, &str[0], str.length(), wString, wcslen(wString));

			return str;
		}

	}
}
