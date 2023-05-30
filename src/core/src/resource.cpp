
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
        resource res;
        res.m_data = fs::read_text_file(filepath);
        res.m_filetype = b::string::to_lower(filepath.raw_extension().string());
        return res;
    }

    resource resource::from_binary_file(const fs::path& filepath) {
        resource res;
        res.m_data = fs::read_binary_file(filepath);
        res.m_filetype = b::string::to_lower(filepath.raw_extension().string());
        return res;
    }

    resource resource::from_byte_string(const b::string& data, const b::string& filetype) {
        resource res;
        res.m_data = data;
        res.m_filetype = filetype;
        return res;
    }

    resource resource::from_buffer(const void* buffer, size_t length, const b::string& filetype) {
        resource res;
        res.m_data.resize(length);
        res.m_filetype = filetype;
        std::memcpy(res.m_data.data(), buffer, length);
        return res;
    }

    resource resource::from_base64(const b::string& base64, const b::string& filetype) {
        return resource::from_byte_string(b::decode_base64(base64), filetype);
    }

    void* resource::data() const {
        return (void*)m_data.data();
    }

    size_t resource::size() const {
        return m_data.size();
    }

    b::string resource::filetype() const {
        return m_filetype;
    }

    b::string resource::string() const {
        return m_data;
    }

    std::vector<uint8_t> resource::as_vector() const {
        return { m_data.begin(), m_data.end() };
    }

    b::string resource::base64() const {
        return b::encode_base64(string());
    }

    bool resource::write_to_text_file(const b::fs::path& filepath) const {
        b::fs::ofstream file(filepath, b::fs::Mode::TEXT);
        if (file.fail())
            return false;
        file << m_data;
        return true;
    }

    bool resource::write_to_binary_file(const b::fs::path& filepath) const {
        b::fs::ofstream file(filepath, b::fs::Mode::BINARY);
        if (file.fail())
            return false;
        file << m_data;
        return true;
    }

    resource::on_disk_resource::on_disk_resource(const b::fs::path &path, const b::string& data) : m_path(path) {
        b::fs::ofstream file(path, b::fs::Mode::BINARY);
        file << data;
    }

    resource::on_disk_resource::~on_disk_resource() {
        reset();
    }

    resource::on_disk_resource resource::as_temporary_on_disk_resource() const {
        auto path = b::folders::get_cache();
        path += b::uuid::v4();
        if (!m_filetype.empty())
            path += "." + m_filetype;
        return on_disk_resource(path, m_data);
    };

    void resource::on_disk_resource::reset() {
        if (!m_path.empty()) {
            b::fs::remove(m_path);
            m_path.clear();
        }
    };

}