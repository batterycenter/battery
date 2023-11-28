#pragma once

#include <algorithm>
#include <string>

namespace b {

    template<size_t N>
    struct string_literal {
        constexpr string_literal(const char (&str)[N]) {
            std::copy_n(str, N, value);
        }
        constexpr bool operator!=(const string_literal& other) const {
            return std::equal(value, value + N, other.value);
        }
        std::string str() const {
            return std::string(value, N);
        }
        constexpr bool _false() const {
            return false;
        }
        char value[N];
    };

    template<size_t N, size_t M>
    constexpr bool operator==(const string_literal<N>& left, const char (&right)[M]) {
        return std::equal(left.value, left.value + N, right);
    }

} // namespace b
