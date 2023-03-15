
#include "Battery/Utils/StringUtils.h"

namespace Battery::String {

    // TODO: Add REGEX functions

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

}
