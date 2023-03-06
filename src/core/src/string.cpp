
#include "battery/core/string.h"
#include "utf8.h"

namespace battery::string {

    std::vector<std::string_view> split(const std::string_view& str, char delimeter) {
        std::string::size_type b = 0;
        std::vector<std::string_view> result;

        while ((b = str.find_first_not_of(delimeter, b)) != std::string::npos) {
            auto e = str.find_first_of(delimeter, b);
            result.push_back(str.substr(b, e - b));
            b = e;
        }

        return result;
    }

    std::string join(const std::vector<std::string>& strings, const std::string_view& spacer) {
        std::string str = "";

        for (size_t i = 0; i < strings.size(); i++) {
            str += strings[i];

            if (i < strings.size() - 1 && spacer != "") {
                str += spacer;
            }
        }

        return str;
    }

    std::string replace(std::string string, const std::string_view& from, const std::string_view& to) {
        if (from.empty())
            return string;
        size_t start_pos = 0;
        while ((start_pos = string.find(from, start_pos)) != std::string::npos) {
            string.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return string;
    }

    std::string replace_one(std::string string, const std::string_view& from, const std::string_view& to, [[maybe_unused]] int occurrence) {
        if (from.empty())
            return string;

        size_t start_pos = 0;
        while ((start_pos = string.find(from, start_pos)) != std::string::npos) {
            string.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return string;
    }

    std::string u8string_to_string(const std::u8string_view& str) {      // Simply copy the bytes as-is
        std::string target;
        target.resize(str.length());
        std::memcpy(target.data(), str.data(), str.length());
        return target;
    }

    std::u8string string_to_u8string(const std::string_view& str) {  // Check for validity and then copy the bytes as-is
        if (!utf8::is_valid(str)) {
            throw std::invalid_argument("Invalid utf-8 sequence");
        }
        std::u8string u8target;
        u8target.resize(str.length());
        std::memcpy(u8target.data(), str.data(), str.length());
        return u8target;
    }

    // TODO: Support REGEX

}
