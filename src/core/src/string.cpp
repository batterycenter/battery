
#include "battery/core/string.h"

namespace battery::string {

    std::vector<std::string> split(std::string str, char delimeter) {
        std::string::size_type b = 0;
        std::vector<std::string> result;

        while ((b = str.find_first_not_of(delimeter, b)) != std::string::npos) {
            auto e = str.find_first_of(delimeter, b);
            result.push_back(str.substr(b, e - b));
            b = e;
        }

        return result;
    }

    std::string join(std::vector<std::string> strings, std::string spacer) {
        std::string str = "";

        for (size_t i = 0; i < strings.size(); i++) {
            str += strings[i];

            if (i < strings.size() - 1 && spacer != "") {
                str += spacer;
            }
        }

        return str;
    }

    std::string replace(std::string string, const std::string& from, const std::string& to) {
        if (from.empty())
            return string;
        size_t start_pos = 0;
        while ((start_pos = string.find(from, start_pos)) != std::string::npos) {
            string.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return string;
    }

    std::string replace_one(std::string string, const std::string& from, const std::string& to, int occurrence) {
        if (from.empty())
            return string;
        size_t start_pos = 0;
        while ((start_pos = string.find(from, start_pos)) != std::string::npos) {
            string.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return string;
    }

    // TODO: Support REGEX

}
