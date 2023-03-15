#pragma once

#include "battery/core/fs.h"
#include <string>

namespace battery {

    class resource {
    public:
        resource() = default;

        static resource from_text_file(const fs::path& filepath);
        static resource from_binary_file(const fs::path& filepath);
        static resource from_byte_string(const std::string& data);
        static resource from_buffer(const void* buffer, size_t length);
        static resource from_base64(const std::string& base64);

        [[nodiscard]] const char* data() const;
        [[nodiscard]] size_t size() const;
        [[nodiscard]] std::string as_string() const;
        [[nodiscard]] std::vector<uint8_t> as_vector() const;

    private:
        std::string _data;
    };

}