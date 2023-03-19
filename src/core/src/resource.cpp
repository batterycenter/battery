
#include "battery/core/resource.hpp"
#include "battery/core/log.hpp"
#include <ios>
#include <iostream>

#include "utf8.h"

namespace b {

    resource resource::from_text_file(const battery::fs::path& filepath) {
        battery::fs::ifstream file(filepath, battery::fs::Mode::TEXT);
        if (file.fail())
            throw std::ios::failure("battery::resource: Failed to open file '" + filepath.to_string() + "' for reading: " + strerror(errno));
        resource res;
        res._data = file.to_string();
        res._filetype = b::to_lower(filepath.extension().to_string());
        if (!res._filetype.empty()) {
            if (res._filetype[0] == '.') {
                res._filetype.erase(res._filetype.begin());
            }
        }
        return res;
    }

    resource resource::from_binary_file(const battery::fs::path& filepath) {
        battery::fs::ifstream file(filepath, battery::fs::Mode::BINARY);
        if (file.fail())
            throw std::ios::failure("battery::resource: Failed to open file '" + filepath.to_string() + "' for reading: " + strerror(errno));
        resource res;
        res._data = file.to_string();
        res._filetype = b::to_lower(filepath.extension().to_string());
        if (!res._filetype.empty()) {
            if (res._filetype[0] == '.') {
                res._filetype.erase(res._filetype.begin());
            }
        }
        return res;
    }

    resource resource::from_byte_string(const std::string& data, const std::string& filetype) {
        resource res;
        res._data = data;
        res._filetype = filetype;
        return res;
    }

    resource resource::from_buffer(const void* buffer, size_t length, const std::string& filetype) {
        resource res;
        res._data.resize(length);
        res._filetype = filetype;
        std::memcpy(res._data.data(), buffer, length);
        return res;
    }

    resource resource::from_base64(const std::string& base64, const std::string& filetype) {
        return resource::from_byte_string(b::decode_base64(base64), filetype);
    }

    const char* resource::data() const {
        return _data.data();
    }

    size_t resource::size() const {
        return _data.size();
    }

    std::string resource::filetype() const {
        return _filetype;
    }

    std::string resource::as_string() const {
        return _data;
    }

    std::vector<uint8_t> resource::as_vector() const {
        return { _data.begin(), _data.end() };
    }

    std::string resource::as_base64() const {
        return b::encode_base64(as_string());
    }

}