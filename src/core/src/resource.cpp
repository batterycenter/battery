
#include "battery/core/resource.hpp"
#include "battery/core/folders.hpp"
#include "battery/core/fs.hpp"
#include "battery/core/uuid.hpp"
#include "battery/core/base64.hpp"

#include "utf8.h"

namespace b {

//    Resource Resource::FromTextFile(const fs::path& filepath) {
//        Resource res;
//        res.m_data = fs::read_text_file(filepath).encode_utf8();
//        res.m_filetype = filepath.raw_extension().string().to_lower();
//        return res;
//    }
//
//    Resource Resource::FromBinaryFile(const fs::path& filepath) {
//        Resource res;
//        res.m_data = fs::read_binary_file(filepath).encode_utf8();
//        res.m_filetype = filepath.raw_extension().string().to_lower();
//        return res;
//    }

    Resource Resource::FromBytes(const b::bytearray& bytes, const std::string& filetype) {
        Resource res;
        res.m_data = bytes;
        res.m_filetype = filetype;
        return res;
    }

    Resource Resource::FromBuffer(const void* buffer, size_t length, const std::string& filetype) {
        Resource res;
        res.m_data.resize(length);
        res.m_filetype = filetype;
        std::memcpy(res.m_data.data(), buffer, length);
        return res;
    }

    Resource Resource::FromBase64(const std::string& base64, const std::string& filetype) {
        if (auto bytes = b::decode_base64_binary(base64); bytes) {
            return Resource::FromBytes(*bytes, filetype);
        }
        return {};
    }

    const unsigned char* Resource::data() const {
        return m_data.data();
    }

    size_t Resource::size() const {
        return m_data.size();
    }

    std::string Resource::filetype() const {
        return m_filetype;
    }

    std::string Resource::string() const {
        return { m_data.begin(), m_data.end() };
    }

    b::bytearray Resource::bytes() const {
        return m_data;
    }

    std::string Resource::base64() const {
        return b::encode_base64(string());
    }

    bool Resource::writeToFile(const b::fs::path& filepath) const {
        return b::fs::try_write(filepath, m_data).has_value();
    }

    Resource::OnDiskResource::OnDiskResource(const b::fs::path &path, const std::string& data) : m_path(path) {
//        b::fs::ofstream file(path, b::fs::Mode::BINARY);
//        file << data.encode_utf8();
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
        return OnDiskResource();
//        return OnDiskResource(cachedir, m_data);
    }

    void Resource::OnDiskResource::reset() {
        if (!m_path.empty()) {
//            b::fs::remove(m_path);
            m_path.clear();
        }
    }

} // namespace b