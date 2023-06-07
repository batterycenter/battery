#pragma once

#include <bit>
#include <fstream>
#include <filesystem>
#include <functional>
#include "battery/core/string.hpp"
#include "battery/core/log.hpp"
#include "battery/core/error.hpp"

namespace b::fs {

    using std::filesystem::exists;					// Checks if a path exists on-disk (file or directory)
    using std::filesystem::create_directory;		// Creates a single directory
    using std::filesystem::create_directories;		// Creates a directory and all above
    using std::filesystem::copy;					// Copy either a file or a directory
    using std::filesystem::remove;                  // Delete a file (symlinks not followed)
    using std::filesystem::remove_all;              // Recursively delete a directory (symlinks not followed)
    using std::filesystem::last_write_time;
    using std::filesystem::file_size;

    using std::filesystem::status;					// Get filesystem status: what type of entry it is
    using std::filesystem::is_regular_file;
    using std::filesystem::is_directory;



    // TODO: filesystem::path seems to be incorrect for UTF-8 (try status())
    class path : public std::filesystem::path {
    public:
        path() = default;
        path(const std::filesystem::path& path) : std::filesystem::path(path.u8string()) {}
        path(const char* path) : std::filesystem::path(b::string(path).u8()) {}
        path(const char8_t* path) : std::filesystem::path(b::string(path).u8()) {}
        path(const char16_t* path) : std::filesystem::path(b::string(path).u8()) {}
        path(const char32_t* path) : std::filesystem::path(b::string(path).u8()) {}
        path(const std::string& path) : std::filesystem::path(b::string(path).u8()) {}
        path(const std::u8string& path) : std::filesystem::path(b::string(path).u8()) {}
        path(const std::u16string& path) : std::filesystem::path(b::string(path).u8()) {}
        path(const std::u32string& path) : std::filesystem::path(b::string(path).u8()) {}
        path(const b::string& path) : std::filesystem::path(path.u8()) {}

        ~path() = default;

        b::string string() const { return std::filesystem::path::u8string(); }

        fs::path extension() const { return std::filesystem::path::extension(); }
        fs::path filename() const { return std::filesystem::path::filename(); }
        fs::path stem() const { return std::filesystem::path::stem(); }
        fs::path parent_path() const { return std::filesystem::path::parent_path(); }
        fs::path make_preferred() { return std::filesystem::path::make_preferred(); }
        fs::path relative_path() const { return std::filesystem::path::relative_path(); }
        fs::path remove_filename() { return std::filesystem::path::remove_filename(); }
        fs::path replace_extension() { return std::filesystem::path::replace_extension(); }
        fs::path replace_extension(const fs::path& p) { return std::filesystem::path::replace_extension(p); }
        fs::path replace_filename(const fs::path& p) { return std::filesystem::path::replace_filename(p); }

        // This function returns the extension, but without the dot.
        // Thus, this either returns the extension like "png" or "txt", or nothing ("")
        fs::path raw_extension() const {
            auto ext = extension().string();
            if (!ext.empty()) {
                if (ext[0] == '.') {
                    ext.erase(ext.begin());
                }
            }
            return ext;
        }

        fs::path& operator+=(const char* path) { this->append(b::string(path).str()); return *this; }
        fs::path& operator+=(const b::string& path) { this->append(path.str()); return *this; }
        fs::path& operator+=(const std::string& path) { this->append(path); return *this; }
        fs::path& operator/=(const char* path) { this->append(b::string(path).str()); return *this; }
        fs::path& operator/(const b::string& path) { this->append(path.str()); return *this; }
        fs::path& operator/(const std::string& path) { this->append(path); return *this; }

        operator b::string() const { return std::filesystem::path::u8string(); }
    };

    inline fs::path operator+(const fs::path& a, const fs::path& b) {
        auto path = a;
        path.append(b.u8string());
        return path;
    }

    inline fs::path operator/(const fs::path& a, const fs::path& b) {
        return a + b;
    }

    enum class Mode {
        TEXT,
        BINARY
    };

    template<typename... T> fs::path current_path(T&&... args) { return std::filesystem::current_path(std::forward<T>(args)...); }






    class ifstream : public std::ifstream {
    public:
        ifstream(const fs::path& path, enum Mode filemode = Mode::TEXT)
                : std::ifstream(path.string().platform_native(),
                                (filemode == Mode::TEXT) ? std::ios::in : (std::ios::in | std::ios::binary)),
                                path(path)
        {
            binary = (filemode == Mode::BINARY);	// remember for later
        }

        ifstream(const fs::path& path, std::ios_base::openmode mode)
                : std::ifstream(path.string().platform_native(), mode),
                                path(path)
        {
            binary = (mode & std::ios::binary);		// remember for later
        }

        ~ifstream() override = default;

    private:
        fs::path path;
        bool binary = false;
    };

    namespace internal {
        inline static std::optional<b::string> read_file_nothrow(const fs::path &path, enum Mode filemode) {
            ifstream file(path, filemode);
            if (!file.is_open()) return {};
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        inline static b::string read_file(const fs::path& path, enum Mode filemode) {
            if (auto str = read_file_nothrow(path, filemode)) {
                return str.value();
            }
            else {
                throw std::runtime_error(b::format("Cannot read file as b::string: File failed while reading into buffer: {}", b::strerror(errno)));
            }
        }

        template<typename TFunc>
        inline static size_t read_file_in_chunks_nothrow(const fs::path& path, size_t chunk_size, TFunc callback, enum Mode filemode) {
            auto file = fs::ifstream(path, filemode);
            if (file.fail()) return -1;
            b::string buffer(chunk_size, 0);
            size_t total_bytes = 0;

            while (!file.eof()) {
                file.read(buffer.data(), buffer.size());    // Read a chunk of the file
                auto this_chunk_size = file.gcount();
                total_bytes += this_chunk_size;

                if (this_chunk_size != 0) {
                    callback(std::string_view(buffer.data(), static_cast<size_t>(this_chunk_size)));
                }
            }

            return total_bytes;
        }

        template<typename TFunc>
        inline static size_t read_file_in_chunks(const fs::path& path, size_t chunk_size, TFunc callback, enum Mode filemode) {
            auto bytes_read = read_file_in_chunks_nothrow(path, chunk_size, callback, filemode);
            if (bytes_read == -1) {
                throw std::runtime_error(b::format("Cannot read file in chunks: File failed while chunk reading into buffer: {}", b::strerror(errno)));
            }
            else {
                return bytes_read;
            }
        }
    }

    inline static std::optional<b::string> read_text_file_nothrow(const fs::path& path) {
        return internal::read_file_nothrow(path, Mode::TEXT);
    }

    inline static std::optional<b::string> read_binary_file_nothrow(const fs::path& path) {
        return internal::read_file_nothrow(path, Mode::BINARY);
    }

    inline static b::string read_text_file(const fs::path& path) {
        return internal::read_file(path, Mode::TEXT);
    }

    inline static b::string read_binary_file(const fs::path& path) {
        return internal::read_file(path, Mode::BINARY);
    }

    template<typename TFunc>
    inline static size_t read_text_file_in_chunks_nothrow(const fs::path& path, size_t chunk_size, TFunc callback) {
        return internal::read_file_in_chunks_nothrow(path, chunk_size, callback, Mode::TEXT);
    }

    template<typename TFunc>
    inline static size_t read_binary_file_in_chunks_nothrow(const fs::path& path, size_t chunk_size, TFunc callback) {
        return internal::read_file_in_chunks_nothrow(path, chunk_size, callback, Mode::BINARY);
    }

    template<typename TFunc>
    inline static size_t read_text_file_in_chunks(const fs::path& path, size_t chunk_size, TFunc callback) {
        return internal::read_file_in_chunks(path, chunk_size, callback, Mode::TEXT);
    }

    template<typename TFunc>
    inline static size_t read_binary_file_in_chunks(const fs::path& path, size_t chunk_size, TFunc callback) {
        return internal::read_file_in_chunks(path, chunk_size, callback, Mode::BINARY);
    }
//
//    // In Binary mode this function is cheap, it reads the filesize from the filesystem.
//    // In Text mode this function is expensive, it reads the entire file into memory, just for the filesize.
//    // There is no other way to get the filesize in text mode, because the representation on-disk is different
//    // from what is read into memory. (Due to line endings) Be aware that it returns the number of bytes in UTF-8
//    inline static std::streamsize file_size_nothrow(const fs::path& path, enum Mode filemode) {
//        if (filemode == b::fs::Mode::BINARY) {
//            return std::filesystem::file_size(b::string(path).str());
//        }
//        else {
//            auto str = fs::read_text_file_nothrow(path);
//            if (str) {
//                return str.value().length();
//            }
//            else {
//                return -1;
//            }
//        }
//    }
//
//    inline static std::streamsize file_size(const fs::path& path, enum Mode filemode) {
//        auto size = file_size_nothrow(path, filemode);
//        if (size == -1) {
//            throw std::runtime_error(b::format("Cannot get compensated file size: File failed while reading into buffer: {}", b::strerror(errno)));
//        }
//        else {
//            return size;
//        }
//    }

    class ofstream : public std::ofstream {
    public:
        ofstream(const fs::path& path, enum Mode filemode = Mode::TEXT, bool createDirectory = true)
                : std::ofstream(create_dir_return_path(path, createDirectory), parse_mode(filemode)) {}

        ofstream(const fs::path& path, std::ios_base::openmode filemode, bool createDirectory = true)
                : std::ofstream(create_dir_return_path(path, createDirectory), filemode) {}

        ~ofstream() override = default;

    private:
        std::ios::openmode parse_mode(enum Mode filemode) const {
            if (filemode == Mode::TEXT) {
                return std::ios::out;
            }
            else {
                return std::ios::out | std::ios::binary;
            }
        }

        b::platform_native_string create_dir_return_path(const fs::path& path, bool createDirectory) const {
            if (path.has_parent_path() && createDirectory) {
                auto parent = path.parent_path();
                if (!fs::exists(parent)) {
                    fs::create_directories(fs::path(path).parent_path());
                }
            }
            return path.string().platform_native();
        }
    };

    namespace internal {
        inline static bool write_file_nothrow(const fs::path &path, const b::string &str, enum Mode filemode, bool createDirectory = true) {
            ofstream file(path, filemode, createDirectory);
            if (file.fail()) return false;
            file << str;
            return true;
        }
    }

    inline static bool write_text_file_nothrow(const fs::path& path, const b::string& str) {
        return internal::write_file_nothrow(path, str, Mode::TEXT);
    }

    inline static bool write_binary_file_nothrow(const fs::path& path, const b::string& str) {
        return internal::write_file_nothrow(path, str, Mode::BINARY);
    }

    inline static void write_text_file(const fs::path& path, const b::string& str) {
        if (!write_text_file_nothrow(path, str)) {
            throw std::runtime_error(b::format("Cannot write file from b::string: File failed for writing: {}", b::strerror(errno)));
        }
    }

    inline static void write_binary_file(const fs::path& path, const b::string& str) {
        if (!write_binary_file_nothrow(path, str)) {
            throw std::runtime_error(b::format("Cannot write file from b::string: File failed for writing: {}", b::strerror(errno)));
        }
    }

}

inline std::ostream& operator<<(std::ostream& stream, const b::fs::path& path) {
    stream << path.string();
    return stream;
}

template <> struct fmt::formatter<b::fs::path> {
    constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator {
        return ctx.begin();
    }
    auto format(const b::fs::path& path, format_context& ctx) const -> format_context::iterator {
        return fmt::format_to(ctx.out(), "{}", path.string());
    }
};
