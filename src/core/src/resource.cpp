
#include <ios>
#include <iostream>
#include "battery/core/resource.hpp"
#include "battery/core/log.hpp"
#include "battery/core/fs.hpp"
#include "battery/core/uuid.hpp"
#include "battery/core/folders.hpp"

#include "utf8.h"

namespace b {

    resource resource::from_text_file(const fs::path& filepath) {
        fs::ifstream file(filepath, fs::Mode::TEXT);
        if (file.fail())
            throw std::ios::failure("battery::resource: Failed to open file '" + filepath.to_string() + "' for reading: " + strerror(errno));
        resource res;
        res._data = file.to_string();
        res._filetype = b::to_lower(filepath.raw_extension().to_string());
        return res;
    }

    resource resource::from_binary_file(const fs::path& filepath) {
        fs::ifstream file(filepath, fs::Mode::BINARY);
        if (file.fail())
            throw std::ios::failure("battery::resource: Failed to open file '" + filepath.to_string() + "' for reading: " + strerror(errno));
        resource res;
        res._data = file.to_string();
        res._filetype = b::to_lower(filepath.raw_extension().to_string());
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

    bool resource::to_text_file(const b::fs::path& filepath) const {
        b::fs::ofstream file(filepath, b::fs::Mode::TEXT);
        if (file.fail())
            return false;
        file << _data;
        return true;
    }

    bool resource::to_binary_file(const b::fs::path& filepath) const {
        b::fs::ofstream file(filepath, b::fs::Mode::BINARY);
        if (file.fail())
            return false;
        file << _data;
        return true;
    }

    resource::on_disk_resource::on_disk_resource(const b::fs::path &path, const std::string& data) : path(path) {
        b::fs::ofstream file(path, b::fs::Mode::BINARY);
        file << data;
    }

    resource::on_disk_resource::~on_disk_resource() {
        reset();
    }

    resource::on_disk_resource resource::as_temporary_on_disk_resource() const {
        auto path = b::folders::get_cache();
        path += b::uuid::v4() + (_filetype.empty() ? "" : "." + _filetype);
        return on_disk_resource(path, _data);
    };

    void resource::on_disk_resource::reset() {
        if (!path.empty()) {
            b::fs::remove(path);
            path.clear();
        }
    };

}