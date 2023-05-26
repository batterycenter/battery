#pragma once

#include "battery/core/fs.hpp"
#include <string>

namespace b {

    class resource {
    public:
        resource() = default;

        static resource from_text_file(const fs::path& filepath);
        static resource from_binary_file(const fs::path& filepath);
        static resource from_byte_string(const b::string& data, const b::string& filetype = "");
        static resource from_buffer(const void* buffer, size_t length, const b::string& filetype = "");
        static resource from_base64(const b::string& base64, const b::string& filetype = "");

        [[nodiscard]] void* data() const;
        [[nodiscard]] size_t size() const;
        // If specified correctly, the filetype will be a string hinting the encoding of the data. Empty is unknown,
        // it's parsed from the file path or specified manually. Guaranteed to be lowercase. Example: "" or "png" or "txt".
        // Can be used in the receiving function to parse the data differently based on the file type
        [[nodiscard]] b::string filetype() const;
        [[nodiscard]] b::string string() const;
        [[nodiscard]] std::vector<uint8_t> as_vector() const;
        [[nodiscard]] b::string base64() const;
        [[nodiscard]] bool write_to_text_file(const fs::path& filepath) const;
        [[nodiscard]] bool write_to_binary_file(const fs::path& filepath) const;

        class on_disk_resource {
        public:
            on_disk_resource() = default;
            explicit on_disk_resource(const b::fs::path& path, const b::string& data);
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

            b::string str() {
                return path;
            }

            operator b::fs::path() {
                return path;
            }

            operator b::string() {
                return path;
            }

            void reset();

        private:
            b::fs::path path;
        };

        [[nodiscard]] on_disk_resource as_temporary_on_disk_resource() const;

    private:
        b::string _data;
        b::string _filetype;
    };

}