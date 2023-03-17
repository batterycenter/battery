#pragma once

#include "battery/core/fs.h"
#include <string>

namespace battery {

    class resource {
    public:
        resource() = default;

        static resource from_text_file(const fs::path& filepath);
        static resource from_binary_file(const fs::path& filepath);
        static resource from_byte_string(const std::string& data, const std::string& filetype = "");
        static resource from_buffer(const void* buffer, size_t length, const std::string& filetype = "");
        static resource from_base64(const std::string& base64, const std::string& filetype = "");

        [[nodiscard]] const char* data() const;
        [[nodiscard]] size_t size() const;
        // If specified correctly, the filetype will be a string hinting the encoding of the data. Empty is unknown,
        // it's parsed from the file path or specified manually. Guaranteed to be lowercase. Example: "" or "png" or "txt".
        // Can be used in the receiving function to parse the data differently based on the file type
        [[nodiscard]] std::string filetype() const;
        [[nodiscard]] std::string as_string() const;
        [[nodiscard]] std::vector<uint8_t> as_vector() const;

    private:
        std::string _data;
        std::string _filetype;
    };

}