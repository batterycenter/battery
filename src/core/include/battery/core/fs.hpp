#pragma once

#include <bit>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <functional>
#include "battery/core/string.hpp"
#include "battery/core/log.hpp"
#include "battery/core/error.hpp"

namespace b::fs {

    using std::filesystem::copy;					// Copy either a file or a directory
    using std::filesystem::file_size;

    using std::filesystem::status;					// Get filesystem status: what type of entry it is

    // TODO: filesystem::path seems to be incorrect for UTF-8 (try status())

    /// ========================================================
    /// ================== Begin path class ====================
    /// ========================================================

    /// \brief A Unicode aware high-level filesystem path
    /// \details The reason to use this class is that many of the C++ standard library functions are not Unicode aware
    ///          and it seems like they return UTF-8 encoded data, but in reality is is some kind of local encoding.
    ///          For this reason there is not a direct conversion from std::filesystem::path to b::fs::path, as
    ///          std::filesystem::path will happily take any std::string, and can thereby inject any kind of local
    ///          encoding - whatever it happens to be. To convert from std::filesystem::path to b::fs::path, use
    ///          `b::fs::path path2(path.u8string())`.
    class path {
    public:

        /// \brief default constructor
        path() = default;

        path(const path&) = default;
        path(path&&) = default;
        path& operator=(const path&) = default;
        path& operator=(path&&) = default;

        /// \brief Construct from a string (Unicode aware)
        /// \param path The path to construct from
        path(const b::string& path);

        /// \brief Assign from a string (Unicode aware)
        path& operator=(const b::string& path);

        /// \brief Assign from a string (Unicode aware)
        path& assign(const b::string& path);

        /// \brief Assign from a string (Unicode aware)
        path& assign(const b::fs::path& path);

        /// \brief Append another path by adding a directory separator
        path& append(const b::string& path);

        /// \brief Append another path by adding a directory separator
        path& append(const b::fs::path& path);

        /// \brief Append another path by adding a directory separator
        path& operator/=(const b::string& path);

        /// \brief Append another path by adding a directory separator
        path& operator/=(const b::fs::path& path);

        /// \brief Concatenate another path without intentionally adding a directory separator
        path& concat(const b::string& path);

        /// \brief Concatenate another path without intentionally adding a directory separator
        path& concat(const b::fs::path& path);

        /// \brief Concatenate another path without intentionally adding a directory separator
        path& operator+=(const b::string& path);

        /// \brief Concatenate another path without intentionally adding a directory separator
        path& operator+=(const b::fs::path& path);

        /// \brief Clear the contents of the path
        void clear();

        /// \brief Make preferred: convert to native format (Windows: backslashes, Linux: keep as-is)
        path& make_preferred();

        /// \brief Make the path absolute, relative to the current working directory. (If it is relative)
        /// \details If the path is already absolute, this function does nothing.
        path& make_absolute();

        /// \brief Remove the filename portion of the path. (Everything after the last directory separator)
        path& remove_filename();

        /// \brief Replace or add the filename portion of the path. (Everything after the last directory separator)
        /// \param filename The filename to replace with
        path& replace_filename(const b::string& filename);

        /// \brief Replace or add the filename portion of the path. (Everything after the last directory separator)
        /// \param filename The filename to replace with
        path& replace_filename(const b::fs::path& filename);

        /// \brief Replace or add the filename extension portion of the path.
        /// \param extension The extension to replace with
        path& replace_extension(const b::string& extension);

        /// \brief Replace or add the filename extension portion of the path.
        /// \param extension The extension to replace with
        path& replace_extension(const b::fs::path& extension);

        /// \brief Swap the contents of two paths
        void swap(path& other) noexcept;

        /// \brief Get the path as a platform native string (backslashes on Windows, forward slashes on others)
        b::string string() const;

        /// \brief Get the path as a generic string (forward slashes on all systems)
        b::string generic_string() const;

        /// \brief Lexicographical comparison of two paths. (e.g. "/a/b" < "/a/b/c")
        int compare(const path& other) const noexcept;

        /// \brief Normalize the path. (e.g. "/a/b/../c" -> "/a/c").
        /// \details This function is purely lexical, meaning it does not access the filesystem, does not check if
        ///          files exist, and does not follow symlinks. For that, see `b::fs::relative()` and `b::fs::proximate()`.
        /// \see b::fs::relative()
        /// \see b::fs::proximate()
        path lexically_normal() const;

        /// \brief Make the path relative to another path. (e.g. "/a/b/c" relative to "/a/d" results in "../b/c")
        /// \details This function is purely lexical, meaning it does not access the filesystem, does not check if
        ///          files exist, and does not follow symlinks. For that, see `b::fs::relative()` and `b::fs::proximate()`.
        ///          If no relative path is found (e.g. one of them is relative), an empty path is returned.
        ///          This makes it different from `lexically_proximate()`
        /// \return The relative path, or an empty path if no relative path is found.
        /// \see b::fs::relative()
        /// \see b::fs::proximate()
        path lexically_relative(const path& base) const;

        /// \brief Make the path relative to another path. (e.g. "/a/b/c" relative to "/a/d" results in "../b/c")
        /// \details This function is purely lexical, meaning it does not access the filesystem, does not check if
        ///          files exist, and does not follow symlinks. For that, see `b::fs::relative()` and `b::fs::proximate()`.
        ///          If no relative path is found (e.g. one of them is relative), the path itself is returned unchanged.
        ///          This makes it different from `lexically_relative()`
        /// \return The relative path, or the unchanged path itself if no relative path is found.
        /// \see b::fs::relative()
        /// \see b::fs::proximate()
        path lexically_proximate(const path& base) const;

        /// \brief Get the root portion of the path. (e.g. "/" on Linux, "C:" on Windows)
        /// \details If the path is relative, an empty path is returned. Use it in combination with `root_directory()`
        /// \return The root portion of the path, or an empty path if the path is relative.
        path root_name() const;

        /// \brief Get the root directory of the path. (e.g. "" on Linux, "\" on Windows)
        /// \details If the path is relative, an empty path is returned. Use it in combination with `root_name()`
        /// \return The root directory of the path, or an empty path if the path is relative.
        path root_directory() const;

        /// \brief Get the root path portion of the path. (e.g. "/" on Linux, "C:\" on Windows)
        /// \details This is equivalent to `root_name() / root_directory()`. If the path is relative,
        ///          an empty path is returned.
        /// \return The root path portion of the path, or an empty path if the path is relative.
        path root_path() const;

        /// \brief Make the path relative to the `root_path()`. (e.g. "C:\Users\Test" -> "Users\Test")
        /// \details If the path is relative, the path itself is returned unchanged.
        /// \return A path relative to the `root_path()`, or the path itself if the path is relative.
        path relative_root_path() const;

        /// \brief Get the parent path of the path. (e.g. "/a/b/c" -> "/a/b")
        /// \details If the path has no parent, the path itself is returned unchanged.
        /// \return The parent path of the path, or itself if the path has no parent.
        path parent_path() const;

        /// \brief Get the filename portion of the path. (e.g. "/a/b/c" -> "c")
        /// \details If the path has no filename, an empty path is returned.
        /// \return The filename portion of the path, or an empty path if the path has no filename.
        path filename() const;

        /// \brief Get the filename portion of the path without the extension. (e.g. "/a/b/c.txt" -> "c")
        /// \details If the path has no filename, an empty path is returned.
        /// \return The filename portion of the path without the extension, or an empty path if the path has no filename.
        path stem() const;

        /// \brief Get the extension portion of the path. (e.g. "/a/b/c.txt" -> ".txt")
        /// \details If the path has no extension, an empty path is returned.
        /// \return The extension portion of the path, or an empty path if the path has no extension.
        path extension() const;

        /// \brief Get the extension portion of the path, but without the dot. (e.g. "/a/b/c.txt" -> "txt")
        /// \details This function either returns the extension like "png" or "txt", or nothing ("")
        /// \return The extension portion of the path without the dot, or an empty path if the path has no extension.
        path raw_extension() const;

        /// \brief Check if the path is empty.
        /// \return True if the path is empty, false otherwise.
        bool empty() const noexcept;

        /// \brief Check if the path has a root path.
        /// \return True, false otherwise.
        bool has_root_path() const;

        /// \brief Check if the path has a root name.
        /// \return True, false otherwise.
        bool has_root_name() const;

        /// \brief Check if the path has a root directory.
        /// \return True, false otherwise.
        bool has_root_directory() const;

        /// \brief Check if the path has a relative root path.
        /// \return True, false otherwise.
        bool has_relative_root_path() const;

        /// \brief Check if the path has a parent path.
        /// \return True, false otherwise.
        bool has_parent_path() const;

        /// \brief Check if the path has a filename.
        /// \return True, false otherwise.
        bool has_filename() const;

        /// \brief Check if the path has a stem.
        /// \return True, false otherwise.
        bool has_stem() const;

        /// \brief Check if the path has an extension.
        /// \return True, false otherwise.
        bool has_extension() const;

        /// \brief Check if the path is absolute, i.e. has a root path in its native format
        /// \details Note: "/" is absolute on Linux and relative on Windows. It is the opposite of `is_relative()`.
        /// \return True, false otherwise.
        /// \see b::fs::path::is_relative()
        bool is_absolute() const;

        /// \brief Check if the path is relative, i.e. has no root path in its native format
        /// \details Note: "/" is absolute on Linux and relative on Windows. It is the opposite of `is_absolute()`.
        /// \return True, false otherwise.
        /// \see b::fs::path::is_relative()
        bool is_relative() const;

        /// \brief Get the native underlying path object as a copy.
        /// \details Whenever you must pass a path on to a system std function, make sure that it is Unicode agnostic.
        ///          You should always try to write `std_function(path.std_path().u8string())`, as this will pass it
        ///          as an std::u8string, containing UTF-8. Most system functions will correctly interpret std::u8string,
        ///          but plain std::string is almost always assumed to be a native encoding, like windows-1252 on Windows.
        /// \warning Only use this function if you know what you are doing! The `std::filesystem::path` object will
        ///          easily make your code fragile in terms of portability if you are not careful.
        /// \return The native underlying path object as a copy.
        std::filesystem::path std_path() const;

        // Iterators. TODO: Implement custom iterators to make sure nothing gets leaked in terms of UTF-8 compliance

//        using iterator = std::filesystem::path::iterator;
//
//        /// \brief Get an iterator to the beginning of the path.
//        /// \return An iterator to the beginning of the path.
//        iterator begin() const noexcept;
//
//        /// \brief Get an iterator past the end of the path.
//        /// \return An iterator past the end of the path.
//        iterator end() const noexcept;

        /// \brief Lexically compare with another path. No filesystem access is performed.
        /// \param rhs The path to compare with.
        /// \return True if the path is equal to the other path, false otherwise.
        bool operator==(const path& rhs) const noexcept;

        /// \brief Lexically compare with another path. No filesystem access is performed.
        /// \param rhs The path to compare with.
        /// \return True if the path is not equal to the other path, false otherwise.
        bool operator!=(const path& rhs) const noexcept;

        /// \brief Lexically compare with another path. No filesystem access is performed.
        /// \param rhs The path to compare with.
        /// \return True if the path is less than the other path, false otherwise.
        bool operator<(const path& rhs) const noexcept;

        /// \brief Lexically compare with another path. No filesystem access is performed.
        /// \param rhs The path to compare with.
        /// \return True if the path is less than or equal to the other path, false otherwise.
        bool operator<=(const path& rhs) const noexcept;

        /// \brief Lexically compare with another path. No filesystem access is performed.
        /// \param rhs The path to compare with.
        /// \return True if the path is greater than the other path, false otherwise.
        bool operator>(const path& rhs) const noexcept;

        /// \brief Lexically compare with another path. No filesystem access is performed.
        /// \param rhs The path to compare with.
        /// \return True if the path is greater than or equal to the other path, false otherwise.
        bool operator>=(const path& rhs) const noexcept;

        /// \brief Lexically compare with another path. No filesystem access is performed.
        /// \param rhs The path to compare with.
        /// \return A strong ordering of the path compared to the other path.
        std::strong_ordering operator<=>(const path& rhs) const noexcept;

    private:
        std::filesystem::path m_path;
    };

    /// \brief Swap two paths
    /// \param lhs The first path
    /// \param rhs The second path
    void swap(path& lhs, path& rhs) noexcept;

    /// \brief Concatenate two paths with a separator
    /// \param lhs The first path
    /// \param rhs The second path
    /// \return The concatenated path
    fs::path operator/(const fs::path& lhs, const fs::path& rhs);

    /// \brief Concatenate two paths WITHOUT a separator
    /// \param lhs The first path
    /// \param rhs The second path
    /// \return The concatenated path
    fs::path operator+(const fs::path& lhs, const fs::path& rhs);

    /// ========================================================
    /// =================== End path class =====================
    /// ========================================================



    /// \brief Get or set the current working directory
    /// \details This function is a wrapper around std::filesystem::current_path, but is Unicode agnostic
    /// \return The current working directory
    template<typename... T>
    b::fs::path current_path(T&&... args) {
        return b::fs::path(std::filesystem::current_path(std::forward<T>(args)...).u8string());
    }

    /// \brief Get or set the current working directory
    /// \details This function is a wrapper around std::filesystem::absolute, but is Unicode agnostic.
    ///          No filesystem access is performed.
    /// \return An absolute path that points to the same file as the input path
    template<typename... T>
    b::fs::path absolute(const b::fs::path& path, T&&... args) {
        return b::fs::path(std::filesystem::absolute(path.generic_string().encode_u8(), std::forward<T>(args)...).u8string());
    }

    /// \brief Check if a path exists on-disk. This can either be a file or a directory.
    /// \details This function is a wrapper around std::filesystem::exists, but is Unicode agnostic.
    /// \param path The path to check
    /// \todo Implement `bool exists( std::filesystem::file_status s ) noexcept`;
    /// \todo Implement variant that takes std::error_code
    /// \return True if the path exists, false otherwise
    bool exists(const b::fs::path& path);

    /// \brief Check if a path on-disk referse to a directory, by querying the filesystem.
    /// \param path The directory path to check
    /// \todo Implement `bool is_directory( std::filesystem::file_status s ) noexcept`;
    /// \todo Implement variant that takes std::error_code
    /// \return True if the path refers to a directory, false otherwise
    bool is_directory(const b::fs::path& path);

    /// \brief Check if a path on-disk referse to a regular file, by querying the filesystem.
    /// \param path The file path to check
    /// \todo Implement `bool is_regular_file( std::filesystem::file_status s ) noexcept`;
    /// \todo Implement variant that takes std::error_code
    /// \return True if the path refers to a regular file, false otherwise
    bool is_regular_file(const b::fs::path& path);

    /// \brief Create a directory including parents on-disk. If the directory already exists, this function does nothing.
    /// \details Note: `create_directory` and `create_directories` as in `std::filesystem` are not both provided
    ///          separately, as it would not make sense from a high-level. This function is essentially a wrapper around
    ///          `std::filesystem::create_directories`. This function achieves the same goal with less error potential.
    /// \param path The path to create
    /// \todo Add a function to set permissions on the created directory (or on any file)
    /// \return True if the directory was created, false otherwise
    bool create_directory(const b::fs::path& path);

    /// \brief Get the last modification time of a file or directory. Symlinks are followed.
    /// \param path The path to check
    /// \todo Implement non-throwing variant
    /// \todo Investigate if a custom std::filesystem::file_time_type should be written
    /// \return The last modification time of the file or directory
    std::filesystem::file_time_type last_write_time(const b::fs::path& path);

    /// \brief Set the last modification time of a file or directory. Symlinks are followed.
    /// \param path The path to write
    /// \param newTime The new modification time
    /// \todo Implement non-throwing variant
    /// \todo Investigate if a custom std::filesystem::file_time_type should be written
    void last_write_time(const b::fs::path& path, std::filesystem::file_time_type newTime);

    /// \brief Remove a file or an empty directory from disk. If the path does not exist, this function does nothing.
    /// \details Symlinks are not followed, the link itself is removed.
    /// \param path The path to remove
    /// \todo Implement variant that takes std::error_code
    /// \return True if the path was removed, false otherwise
    bool remove(const b::fs::path& path);

    /// \brief Remove a file or a directory including all content from disk. Does nothing, if the path does not exist.
    /// \details Symlinks are not followed, the link itself is removed. If the path refers to a directory, the directory
    ///          is removed recursively.
    /// \param path The path to remove
    /// \todo Implement variant that takes std::error_code
    /// \return The number of files and directories removed, which is 0 if the path did not exist to begin with
    size_t remove_all(const b::fs::path& path);




    enum class Mode {
        TEXT,
        BINARY
    };

    class ifstream : public std::ifstream {
    public:
        ifstream(const fs::path& path, enum Mode filemode = Mode::TEXT)
                : std::ifstream(path.string().encode_native(),
                                (filemode == Mode::TEXT) ? std::ios::in : (std::ios::in | std::ios::binary)),
                                path(path)
        {
            binary = (filemode == Mode::BINARY);	// remember for later
        }

        ifstream(const fs::path& path, std::ios_base::openmode mode)
                : std::ifstream(path.string().encode_native(), mode),
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
            b::fs::ifstream file(path, filemode);
            if (!file.is_open()) {
                return {};
            }
            std::string str((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
            return string::decode_utf8(str);      // TODO: Do this properly with encodings
        }

        inline static b::string read_file(const fs::path& path, enum Mode filemode) {
            if (auto str = read_file_nothrow(path, filemode)) {
                return str.value();
            }
            else {
                throw std::runtime_error(b::format("Cannot read file as b::string: File failed while reading into buffer: {}", b::strerror(errno)).encode_utf8());
            }
        }

        template<typename TFunc>
        inline static size_t read_file_in_chunks_nothrow(const fs::path& path, size_t chunk_size, TFunc callback, enum Mode filemode) {
            auto file = fs::ifstream(path, filemode);
            if (file.fail()) {
                return -1;
            }
            std::string buffer(chunk_size, 0);
            size_t total_bytes = 0;

            while (!file.eof()) {
                file.read(buffer.data(), buffer.size());    // Read a chunk of the file
                auto this_chunk_size = file.gcount();
                total_bytes += this_chunk_size;

                if (this_chunk_size != 0) {
                    callback(b::string::decode_utf8(std::string(buffer.data(), static_cast<size_t>(this_chunk_size))));
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

        b::native_string create_dir_return_path(const fs::path& path, bool createDirectory) const {
            if (path.has_parent_path() && createDirectory) {
                if (!fs::exists(path.parent_path())) {
                    fs::create_directory(path.parent_path());
                }
            }
            return path.string().encode_native();
        }
    };

    namespace internal {
//        inline static bool write_file_nothrow(const fs::path &path, const b::string &str, enum Mode filemode, bool createDirectory = true) {
//            ofstream file(path, filemode, createDirectory);
//            if (file.fail()) return false;
//            file << str;
//            return true;
//        }
    }

//    inline static bool write_text_file_nothrow(const fs::path& path, const b::string& str) {
//        return internal::write_file_nothrow(path, str, Mode::TEXT);
//    }
//
//    inline static bool write_binary_file_nothrow(const fs::path& path, const b::string& str) {
//        return internal::write_file_nothrow(path, str, Mode::BINARY);
//    }

//    inline static void write_text_file(const fs::path& path, const b::string& str) {
//        if (!write_text_file_nothrow(path, str)) {
//            throw std::runtime_error(b::format("Cannot write file from b::string: File failed for writing: {}", b::strerror(errno)));
//        }
//    }
//
//    inline static void write_binary_file(const fs::path& path, const b::string& str) {
//        if (!write_binary_file_nothrow(path, str)) {
//            throw std::runtime_error(b::format("Cannot write file from b::string: File failed for writing: {}", b::strerror(errno)));
//        }
//    }

} // namespace b::fs

// Hash function that lets b::fs::path be used as a key in std::map and std::unordered_map
namespace std {
    template <> struct hash<b::fs::path> {
        size_t operator()(const b::fs::path& path) const {
            return std::hash<std::string>()(path.generic_string().encode_utf8());
        }
    };
} // namespace std

// Allow b::fs::path to be printed with fmt::format and in logging messages
template <> struct fmt::formatter<b::fs::path> {
    constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator {
        return ctx.begin();
    }
    auto format(const b::fs::path& path, format_context& ctx) const -> format_context::iterator {
        return fmt::format_to(ctx.out(), "{}", path.string());
    }
};