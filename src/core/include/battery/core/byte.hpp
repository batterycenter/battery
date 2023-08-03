#pragma once

#include <vector>
#include <string>

namespace b {

    class bytearray : public std::vector<uint8_t> {
    public:
        bytearray() = default;
        bytearray(const std::vector<uint8_t>& vec) : std::vector<uint8_t>(vec) {}
        bytearray(std::vector<uint8_t>&& vec) : std::vector<uint8_t>(std::move(vec)) {}

        bytearray(const std::string& str) : std::vector<uint8_t>(str.begin(), str.end()) {}

//        template<typename... Args>
//        bytearray(Args&&... args) : std::vector<uint8_t>(std::forward<Args>(args)...) {}

        bytearray& operator=(const std::vector<uint8_t>& vec) {
            std::vector<uint8_t>::operator=(vec);
            return *this;
        }

        bytearray& operator=(std::vector<uint8_t>&& vec) {
            std::vector<uint8_t>::operator=(std::move(vec));
            return *this;
        }

        bytearray& operator=(const std::string& str) {
            std::vector<uint8_t>::operator=(std::vector<uint8_t>(str.begin(), str.end()));
            return *this;
        }

        operator std::string() const {
            return std::string(begin(), end());
        }
    };

} // namespace b