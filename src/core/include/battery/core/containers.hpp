#pragma once

#include <vector>

namespace b {

    template <typename Derived, typename Base>
    concept derived_from = std::is_base_of_v<Base, Derived>;

    template<typename T>
    inline std::vector<T>& erase_nth_element(std::vector<T>& vec, size_t index) {
        if (index >= vec.size()) {
            throw std::out_of_range("b::erase_nth_element: index out of range");
        }
        vec.erase(vec.begin() + index);
        return vec;
    }

}