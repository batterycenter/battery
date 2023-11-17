
#include "battery/exception.hpp"
#include "battery/string.hpp"
#include "battery/extern/utf8proc.h"
#include "battery/internal/windows.hpp"
#include "battery/log.hpp"
#include "utf8.h"
#include <codecvt>
#include <regex>

namespace b {

    // Here we use <codecvt> instead of the utf8cpp library, because the std is supposedly better in
    // dealing with the wide strings on different platforms. (taking std::u16string from utf8cpp and
    // converting to std::wstring might work on Windows but might cause issues on others.)
    // For all other conversions, we use utf8cpp.
    // <codecvt> is deprecated since C++17, but will not be removed any time soon as no alternative
    // has been found yet. We will keep using it until it is removed or replaced by a better alternative.

    std::wstring widen(const std::string& str) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(str);
    }

    std::string narrow(const std::wstring& str) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.to_bytes(str);
    }

    std::string reverse(std::string str) {
        std::reverse(str.begin(), str.end());
        return str;
    }

    bool is_utf8(const std::string& str) {
        return utf8::is_valid(str);
    }

    std::u32string to_utf32(const std::string& str) {
        std::u32string result;
        utf8::utf8to32(str.begin(), str.end(), std::back_inserter(result));
        return result;
    }

    std::string to_utf8(const std::u32string& str) {
        std::string result;
        utf8::utf32to8(str.begin(), str.end(), std::back_inserter(result));
        return result;
    }

    std::vector<std::string> split(std::string str, const std::string& delimiter) {
        size_t pos = 0;
        std::vector<std::string> result;
        while ((pos = str.find(delimiter)) != std::string::npos) {
            result.emplace_back(str.substr(0, pos));
            str.erase(0, pos + delimiter.length());
        }
        if (!str.empty()) {
            result.emplace_back(str);
        }
        return result;
    }

    std::string join(const std::vector<std::string>& strings, const std::string& spacer) {
        std::string result;
        for (const auto& token : strings) {
            result += token;
            if (!spacer.empty()) {
                result += spacer;
            }
        }
        if (!result.empty()) {
            for (size_t i = 0; i < spacer.size(); ++i) {
                result.pop_back();
            }
        }
        return result;
    }

    std::string rjoin(std::vector<std::string> strings, const std::string& spacer) {
        std::reverse(strings.begin(), strings.end());
        return join(strings, spacer);
    }

    std::string regex_replace(const std::string& str,
                                    const std::string& pattern,
                                    const std::string& replace,
                                    std::regex_constants::match_flag_type flags) {
        auto result = std::regex_replace(
                widen(str),
                std::wregex(widen(pattern)),
                widen(replace),
                flags
        );
        return narrow(result);
    }

    std::vector<std::string> regex_match(const std::string& str, const std::string& pattern,
                                         std::regex_constants::match_flag_type flags) {
        std::vector<std::string> matches;
        std::wsmatch match;
        auto wide = widen(str);
        while (std::regex_search(wide, match, std::wregex(widen(pattern)), flags)) {
            matches.emplace_back(narrow(match.str(0)));
            wide = match.suffix().str();
        }
        return matches;
    }

    std::string replace(const std::string& str, const std::string& pattern, std::function<std::string(int)> replacer) {
        std::string data = str;
        std::string buf;
        std::size_t pos = 0;
        std::size_t prevPos = 0;

        // Reserves rough estimate of final size of string.
        buf.reserve(str.size());

        int occurrence = 0;
        while (true) {
            prevPos = pos;
            pos = data.find(pattern, pos);
            if (pos == std::string::npos) {
                break;
            }
            buf.append(data, prevPos, pos - prevPos);
            buf += replacer(occurrence);
            pos += pattern.size();
            occurrence++;
        }

        buf.append(data, prevPos, data.size() - prevPos);
        return buf;
    }

    std::string replace(const std::string& str, const std::string& pattern, const std::string& value) {
        return replace(str, pattern, [&value](int) { return value; });
    }

    std::string replace_first(const std::string& str, const std::string& pattern, const std::string& value) {
        return replace(str, pattern, [&value,&pattern](int occurrence) {
            return occurrence == 0 ? value : pattern;
        });
    }

    std::string replace_last(const std::string& str, const std::string& pattern, const std::string& value) {
        return b::reverse(b::replace_first(b::reverse(str), b::reverse(pattern), b::reverse(value)));
    }

    std::string to_lower(const std::string& str) {
        std::u32string result;
        for (const auto& chr : b::to_utf32(str)) {
            result += static_cast<char32_t>(utf8proc_tolower(static_cast<int32_t>(chr)));
        }
        return b::to_utf8(result);
    }

    std::string to_upper(const std::string& str) {
        std::u32string result;
        for (const auto& chr : b::to_utf32(str)) {
            if (chr == U'ß') {  // This special case is not handled well in the library (at least not how we want)
                result += U"SS";
                continue;
            }
            result += static_cast<char32_t>(utf8proc_toupper(static_cast<int32_t>(chr)));
        }
        return b::to_utf8(result);
    }

} // namespace b
