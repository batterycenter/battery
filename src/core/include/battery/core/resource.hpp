#pragma once

#include "battery/core/fs.hpp"
#include "battery/core/bytearray.hpp"

namespace b {

    class Resource {
    public:
        Resource() = default;

        static Resource FromTextFile(const fs::path& filepath);
        static Resource FromBinaryFile(const fs::path& filepath);
        static Resource FromBytes(const b::bytearray& bytes, const b::string& filetype = ""_b);
        static Resource FromBuffer(const void* buffer, size_t length, const b::string& filetype = ""_b);
        static Resource FromBase64(const b::string& base64, const b::string& filetype = ""_b);

        [[nodiscard]] const unsigned char* data() const;
        [[nodiscard]] size_t size() const;
        // If specified correctly, the filetype will be a string hinting the encoding of the data. Empty is unknown,
        // it's parsed from the file path or specified manually. Guaranteed to be lowercase. Example: "" or "png" or "txt".
        // Can be used in the receiving function to parse the data differently based on the file type
        [[nodiscard]] b::string filetype() const;
        [[nodiscard]] b::string string() const;
        [[nodiscard]] std::vector<uint8_t> asVector() const;
        [[nodiscard]] b::string base64() const;
        [[nodiscard]] bool writeToTextFile(const fs::path& filepath) const;
        [[nodiscard]] bool writeToBinaryFile(const fs::path& filepath) const;

        class OnDiskResource {
        public:
            OnDiskResource() = default;
            explicit OnDiskResource(const b::fs::path& path, const b::string& data);
            ~OnDiskResource();

            OnDiskResource(const OnDiskResource&) = delete;     // We cannot allow a copy. A move is allowed
            OnDiskResource& operator=(const OnDiskResource&) = delete;
            OnDiskResource(OnDiskResource&& other) noexcept {
                reset();
                m_path = other.m_path;
                other.m_path.clear();
            }
            OnDiskResource& operator=(OnDiskResource&& other) noexcept {
                reset();
                m_path = other.m_path;
                other.m_path.clear();
                return *this;
            }

            b::string str() {
                return m_path.generic_string();
            }

            operator b::fs::path() {
                return m_path;
            }

            operator b::string() {
                return m_path.generic_string();
            }

            void reset();

        private:
            b::fs::path m_path;
        };

        [[nodiscard]] OnDiskResource asTemporaryOnDiskResource() const;

    private:
        b::bytearray m_data;
        b::string m_filetype;
    };

}