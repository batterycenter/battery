
#include "battery/core/resource.h"
#include <ios>
#include <iostream>

namespace battery {

    resource resource::from_text_file(const fs::path& filepath) {
        battery::fs::ifstream file(filepath, battery::fs::Mode::TEXT);
        if (file.fail())
            throw std::ios::failure("battery::resource: Failed to open file '" + filepath.to_string() + "' for reading: " + strerror(errno));
        resource res;
        res._data = file.to_string();
        return res;
    }

    resource resource::from_binary_file(const fs::path& filepath) {
        battery::fs::ifstream file(filepath, battery::fs::Mode::BINARY);
        if (file.fail())
            throw std::ios::failure("battery::resource: Failed to open file '" + filepath.to_string() + "' for reading: " + strerror(errno));
        resource res;
        res._data = file.to_string();
        return res;
    }

    resource resource::from_byte_string(const std::string& data) {
        resource res;
        res._data = data;
        return res;
    }

    resource resource::from_buffer(const void* buffer, size_t length) {
        resource res;
        res._data.resize(length);
        std::memcpy(res._data.data(), buffer, length);
        return res;
    }

    resource resource::from_base64(const std::string& base64) {
        return resource::from_byte_string(battery::string::decode_base64(base64));
    }

    const char* resource::data() const {
        return _data.data();
    }

    size_t resource::size() const {
        return _data.size();
    }

    std::string resource::as_string() const {
        return _data;
    }

    std::vector<uint8_t> resource::as_vector() const {
        return { _data.begin(), _data.end() };
    }

}