#pragma once

#include "fs.hpp"
#include "string.hpp"

namespace b {

    class Resource {
    public:
        Resource() = default;

        static Resource FromTextFile(const fs::path& filepath);
        static Resource FromBinaryFile(const fs::path& filepath);
        static Resource FromBytes(const b::bytearray& bytes, const std::string& filetype = "");
        static Resource FromBuffer(const void* buffer, size_t length, const std::string& filetype = "");
        static Resource FromBase64(const std::string& base64, const std::string& filetype = "");

        [[nodiscard]] const unsigned char* data() const;
        [[nodiscard]] size_t size() const;
        // If specified correctly, the filetype will be a string hinting the encoding of the data. Empty is unknown,
        // it's parsed from the file path or specified manually. Guaranteed to be lowercase. Example: "" or "png" or "txt".
        // Can be used in the receiving function to parse the data differently based on the file type
        [[nodiscard]] std::string filetype() const;
        [[nodiscard]] std::string string() const;
        [[nodiscard]] b::bytearray bytes() const;
        [[nodiscard]] std::string base64() const;
        [[nodiscard]] bool writeToFile(const fs::path& filepath) const;

        class OnDiskResource {
        public:
            OnDiskResource() = default;
            explicit OnDiskResource(const b::fs::path& path, const std::string& data);
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

            std::string str() {
                return m_path.string();
            }

            operator b::fs::path() {
                return m_path;
            }

            operator std::string() {
                return m_path.string();
            }

            void reset();

        private:
            b::fs::path m_path;
        };

        [[nodiscard]] OnDiskResource asTemporaryOnDiskResource() const;

    private:
        b::bytearray m_data;
        std::string m_filetype;
    };

} // namespace b