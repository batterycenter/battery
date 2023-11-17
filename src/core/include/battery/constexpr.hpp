#ifndef BATTERY_CORE_CONSTEXPR_HPP
#define BATTERY_CORE_CONSTEXPR_HPP

namespace b {

    template<size_t N>
    struct string_literal {
        constexpr string_literal(const char (&str)[N]) {
            std::copy_n(str, N, value);
        }
        constexpr bool operator!=(const string_literal& other) const {
            return std::equal(value, value + N, other.value);
        }
        char value[N];
    };

} // namespace b

template<size_t N, size_t M>
constexpr bool operator==(const b::string_literal<N>& left, const char (&right)[M]) {
    return std::equal(left.value, left.value + N, right);
}

#endif // BATTERY_CORE_CONSTEXPR_HPP