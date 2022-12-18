
#include "Battery/Utils/StringUtils.h"

namespace Battery::String {

	std::vector<std::string> Split(std::string str, char delimeter) {
		std::string::size_type b = 0;
		std::vector<std::string> result;

		while ((b = str.find_first_not_of(delimeter, b)) != std::string::npos) {
			auto e = str.find_first_of(delimeter, b);
			result.push_back(str.substr(b, e - b));
			b = e;
		}

		return result;
	}

	std::string Join(std::vector<std::string> strings, std::string spacer) {
		std::string str = "";

		for (size_t i = 0; i < strings.size(); i++) {
			str += strings[i];

			if (i < strings.size() - 1 && spacer != "") {
				str += spacer;
			}
		}

		return str;
	}

    std::string Replace(std::string string, const std::string& from, const std::string& to) {
        if (from.empty())
            return string;
        size_t start_pos = 0;
        while ((start_pos = string.find(from, start_pos)) != std::string::npos) {
            string.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return string;
    }

    std::string ReplaceOne(std::string string, const std::string& from, const std::string& to, int occurrence) {
        if (from.empty())
            return string;
        size_t start_pos = 0;
        while ((start_pos = string.find(from, start_pos)) != std::string::npos) {
            string.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return string;
    }

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
	
	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	static inline bool is_base64(uint8_t c) {
		return (isalnum(c) || (c == '+') || (c == '/'));
	}

    std::string EncodeBase64(const std::string& str) {
        return EncodeBase64(str.data(), str.size());
    }

	std::string EncodeBase64(const void* buffer, size_t buffer_size) {
		uint8_t* buf = (uint8_t*)buffer;
		int i = 0;
		int j = 0;
		uint8_t char_array_3[3];
		uint8_t char_array_4[4];

		std::string ret;
		while (buffer_size--) {
			char_array_3[i++] = *(buf++);
			if (i == 3) {
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i < 4); i++)
					ret += base64_chars[char_array_4[i]];
				i = 0;
			}
		}

		if (i) {
			for (j = i; j < 3; j++)
				char_array_3[j] = '\0';

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++)
				ret += base64_chars[char_array_4[j]];

			while ((i++ < 3))
				ret += '=';
		}

		return ret;
	}

	std::vector<uint8_t> DecodeBase64(const std::string& data) {
		return DecodeBase64(data.data(), data.size());
	}

	std::vector<uint8_t> DecodeBase64(const void* buffer, size_t buffer_size) {
        const uint8_t* buf = (const uint8_t*)buffer;
		int i = 0;
		int j = 0;
		int in_ = 0;
		uint8_t char_array_4[4], char_array_3[3];
		std::vector<uint8_t> ret;

		while (buffer_size-- && (buf[in_] != '=') && is_base64(buf[in_])) {
			char_array_4[i++] = buf[in_]; in_++;
			if (i == 4) {
				for (i = 0; i < 4; i++)
					char_array_4[i] = (uint8_t)base64_chars.find(char_array_4[i]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++)
					ret.push_back(char_array_3[i]);
				i = 0;
			}
		}

		if (i) {
			for (j = i; j < 4; j++)
				char_array_4[j] = 0;

			for (j = 0; j < 4; j++)
				char_array_4[j] = (uint8_t)base64_chars.find(char_array_4[j]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
		}

		return ret;
	}

}
