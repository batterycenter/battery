#pragma once

#include "battery/core/fs.hpp"
#include <string>

namespace b {

    class resource {
    public:
        resource() = default;

        static resource from_text_file(const fs::path& filepath);
        static resource from_binary_file(const fs::path& filepath);
        static resource from_byte_string(const std::string& data, const std::u8string& filetype = u8"");
        static resource from_buffer(const void* buffer, size_t length, const std::u8string& filetype = u8"");
        static resource from_base64(const std::string& base64, const std::u8string& filetype = u8"");

        [[nodiscard]] const char* data() const;
        [[nodiscard]] size_t size() const;
        // If specified correctly, the filetype will be a string hinting the encoding of the data. Empty is unknown,
        // it's parsed from the file path or specified manually. Guaranteed to be lowercase. Example: "" or "png" or "txt".
        // Can be used in the receiving function to parse the data differently based on the file type
        [[nodiscard]] std::u8string filetype() const;
        [[nodiscard]] std::string as_string() const;
        [[nodiscard]] std::vector<uint8_t> as_vector() const;
        [[nodiscard]] std::string as_base64() const;
        [[nodiscard]] bool to_text_file(const fs::path& filepath) const;
        [[nodiscard]] bool to_binary_file(const fs::path& filepath) const;

        class on_disk_resource {
        public:
            on_disk_resource() = default;
            explicit on_disk_resource(const b::fs::path& path, const std::string& data);
            ~on_disk_resource();

            on_disk_resource(const on_disk_resource&) = delete;     // We cannot allow a copy. A move is allowed
            on_disk_resource& operator=(const on_disk_resource&) = delete;
            on_disk_resource(on_disk_resource&& other) {
                reset();
                path = std::move(other.path);
                other.path.clear();
            }
            on_disk_resource& operator=(on_disk_resource&& other) noexcept {
                reset();
                path = std::move(other.path);
                other.path.clear();
                return *this;
            }

            std::u8string str() {
                return path.u8string();
            }

            operator b::fs::path() {
                return path;
            }

            operator std::u8string() {
                return path.u8string();
            }

            void reset();

        private:
            b::fs::path path;
        };

        [[nodiscard]] on_disk_resource as_temporary_on_disk_resource() const;

    private:
        std::string _data;
        std::u8string _filetype;
    };

}