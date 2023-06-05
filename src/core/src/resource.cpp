
#include "battery/core/resource.hpp"
#include "battery/core/folders.hpp"
#include "battery/core/fs.hpp"
#include "battery/core/uuid.hpp"

#include "utf8.h"

namespace b {

    Resource Resource::FromTextFile(const fs::path& filepath) {
        Resource res;
        res.m_data = fs::read_text_file(filepath);
        res.m_filetype = b::string::to_lower(filepath.raw_extension().string());
        return res;
    }

    Resource Resource::FromBinaryFile(const fs::path& filepath) {
        Resource res;
        res.m_data = fs::read_binary_file(filepath);
        res.m_filetype = b::string::to_lower(filepath.raw_extension().string());
        return res;
    }

    Resource Resource::FromByteString(const b::string& data, const b::string& filetype) {
        Resource res;
        res.m_data = data;
        res.m_filetype = filetype;
        return res;
    }

    Resource Resource::FromBuffer(const void* buffer, size_t length, const b::string& filetype) {
        Resource res;
        res.m_data.resize(length);
        res.m_filetype = filetype;
        std::memcpy(res.m_data.data(), buffer, length);
        return res;
    }

    Resource Resource::FromBase64(const b::string& base64, const b::string& filetype) {
        return Resource::FromByteString(b::decode_base64(base64), filetype);
    }

    const char* Resource::data() const {
        return m_data.data();
    }

    size_t Resource::size() const {
        return m_data.size();
    }

    b::string Resource::filetype() const {
        return m_filetype;
    }

    b::string Resource::string() const {
        return m_data;
    }

    std::vector<uint8_t> Resource::asVector() const {
        return { m_data.begin(), m_data.end() };
    }

    b::string Resource::base64() const {
        return b::encode_base64(string());
    }

    bool Resource::writeToTextFile(const b::fs::path& filepath) const {
        b::fs::ofstream file(filepath, b::fs::Mode::TEXT);
        if (file.fail()) {
            return false;
        }
        file << m_data;
        return true;
    }

    bool Resource::writeToBinaryFile(const b::fs::path& filepath) const {
        b::fs::ofstream file(filepath, b::fs::Mode::BINARY);
        if (file.fail()) {
            return false;
        }
        file << m_data;
        return true;
    }

    Resource::OnDiskResource::OnDiskResource(const b::fs::path &path, const b::string& data) : m_path(path) {
        b::fs::ofstream file(path, b::fs::Mode::BINARY);
        file << data;
    }

    Resource::OnDiskResource::~OnDiskResource() {
        reset();
    }

    Resource::OnDiskResource Resource::asTemporaryOnDiskResource() const {
        auto cachedir = b::Folders::AppCacheDir();
        cachedir += b::uuid::v4();
        if (!m_filetype.empty()) {
            cachedir.replace_extension(m_filetype);
        }
        return OnDiskResource(cachedir, m_data);
    }

    void Resource::OnDiskResource::reset() {
        if (!m_path.empty()) {
            b::fs::remove(m_path);
            m_path.clear();
        }
    }

} // namespace b