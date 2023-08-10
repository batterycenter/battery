
#include "battery/core/base64.hpp"

namespace b {

    static constexpr std::string_view CHAR_TO_BASE64 =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    static constexpr std::string_view BASE64_TO_CHAR =
            "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
            "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
            "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x3E\xFF\xFF\xFF\x3F"
            "\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\xFF\xFF\xFF\xFF\xFF\xFF"
            "\xFF\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E"
            "\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\xFF\xFF\xFF\xFF\xFF"
            "\xFF\x1A\x1B\x1C\x1D\x1E\x1F\x20\x21\x22\x23\x24\x25\x26\x27\x28"
            "\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\xFF\xFF\xFF\xFF\xFF"
            "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
            "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
            "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x3E\xFF\xFF\xFF\x3F"
            "\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\xFF\xFF\xFF\xFF\xFF\xFF"
            "\xFF\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E"
            "\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\xFF\xFF\xFF\xFF\xFF"
            "\xFF\x1A\x1B\x1C\x1D\x1E\x1F\x20\x21\x22\x23\x24\x25\x26\x27\x28"
            "\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\xFF\xFF\xFF\xFF\xFF";

    b::string encode_base64(const b::string &str) {
        return encode_base64(b::bytearray(str.encode_utf8()));
    }

    b::string encode_base64(const b::bytearray &data) {
        b::string result;
        for (size_t index = 0; index < data.size(); index += 3) {
            size_t const remaining = std::min<size_t>(data.size() - index, 3u); // 1, 2 or 3 bytes remaining each pass

            // Load 3 bytes (24 bits) of input data into the temporary buffer
            uint32_t buffer = data[index] << 16u;
            buffer |= remaining >= 2u ? data[index + 1] << 8u : 0u;
            buffer |= remaining >= 3u ? data[index + 2] << 0u : 0u;

            // Separate the 24 bits into 4 6-bit groups and append to the result (Add Padding character if needed)
            result += CHAR_TO_BASE64[(buffer & (63u << 18u)) >> 18u];
            result += CHAR_TO_BASE64[(buffer & (63u << 12u)) >> 12u];
            result += remaining >= 2u ? CHAR_TO_BASE64[(buffer & (63u << 6u)) >> 6u] : '=';
            result += remaining >= 3u ? CHAR_TO_BASE64[(buffer & 63u)] : '=';
        }
        return result;
    }

    std::optional<b::string> decode_base64_text(const b::string &str) {
        if (auto data = decode_base64_binary(str)) {
            return data->decode_utf8();
        }
        return {};
    }

    std::optional<b::bytearray> decode_base64_binary(const b::string &str) {
        std::vector<uint8_t> result;
        for (size_t index = 0; index < str.size(); index += 4) {
            size_t const remaining = std::min<size_t>(str.size() - index, 4u); // 1, 2, 3 or 4 characters remaining

            // Load 4 6-bit characters (24 bits) of input data into the temporary buffer
            uint32_t buf = 0;
            uint32_t shift = 18u;
            for (int i = 0; i < 4; i++) {
                if (i < remaining) {
                    auto chr = str[index + i];
                    uint8_t const val = BASE64_TO_CHAR[chr];
                    if (val == 0xFF && chr != '=') {
                        return {};
                    }
                    buf |= chr != '=' ? (val << shift) : 0u;
                    shift -= 6;
                }
            }
            result.push_back((buf & (0xFFu << 16u)) >> 16u);
            if (remaining >= 3) {
                if (str[index + 2] != '=') {
                    result.push_back((buf & (0xFFu << 8u)) >> 8u);
                }
            }
            if (remaining >= 4) {
                if (str[index + 3] != '=') {
                    result.push_back(buf & 0xFFu);
                }
            }
        }
        return b::bytearray(result);
    }

} // namespace b