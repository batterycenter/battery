#ifndef BATTERY_CORE_FS_HPP
#define BATTERY_CORE_FS_HPP

//
// Copyright 2022 Florian Zachs (HerrNamenlos123)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//         http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "string.hpp"
#include "constexpr.hpp"
#include "error.hpp"
#include "exception.hpp"
#include "log.hpp"
#include "format.hpp"
#include "bytearray.hpp"
#include <bit>
#include <filesystem>
#include <fstream>
#include <functional>
#include <sstream>

namespace b::fs {

    // We re-use these types directly
    using std::filesystem::file_time_type;
    using std::filesystem::file_status;
    using std::filesystem::perms;
    using std::filesystem::perm_options;
    using std::filesystem::copy_options;
    using std::filesystem::space_info;

    // ========================================================
    // ================== Begin path class ====================
    // ========================================================

    /// \brief A Unicode aware high-level filesystem path
    /// \details The reason to use this class is that many of the C++ standard library functions are not Unicode aware
    ///          and it seems like they return UTF-8 encoded data, but in reality is is some kind of local encoding.
    ///          For this reason there is not a direct conversion from std::filesystem::path to b::fs::path, as
    ///          std::filesystem::path will happily take any std::string, and can thereby inject any kind of local
    ///          encoding - whatever it happens to be. To convert from std::filesystem::path to b::fs::path, use
    ///          `b::fs::path path2(path.u8string())`.
    class path {
    public:

        using iterator = std::filesystem::path::iterator;
        using const_iterator = std::filesystem::path::const_iterator;

        /// \brief The preferred separator for the current platform (Backslash on Windows, forward slash on others)
#ifdef B_OS_WINDOWS
        inline static std::string const preferred_separator =
                b::narrow(b::native_string(&std::filesystem::path::preferred_separator, 1));
#else
        inline static std::string const preferred_separator =
                b::native_string(&std::filesystem::path::preferred_separator, 1);
#endif

        /// \brief default constructor
        path() = default;

        path(const path&) = default;
        path(path&&) = default;
        path& operator=(const path&) = default;
        path& operator=(path&&) = default;

        /// \brief Construct from a UTF-8 string
        /// \param path The path to construct from
        path(const char* path);

        /// \brief Construct from a UTF-8 string
        /// \param path The path to construct from
        path(const std::string& path);

        /// \brief Construct from a UTF-8 string
        /// \param path The path to construct from
        path(const std::u8string& path);

        /// \brief Assign from a UTF-8 string
        /// \param path The path to assign from
        /// \return A reference to self
        path& operator=(const std::string& path);

        /// \brief Assign from a UTF-8 string
        /// \param path The path to assign from
        /// \return A reference to self
        path& assign(const b::fs::path& path);

        /// \brief Append another path by adding a directory separator
        /// \param path The path to append
        /// \return A reference to self
        path& append(const b::fs::path& path);

        /// \brief Append another path by adding a directory separator
        /// \param path The path to append
        /// \return A reference to self
        path& operator/=(const b::fs::path& path);

        /// \brief Concatenate another path without intentionally adding a directory separator
        /// \param path The path to concatenate
        /// \return A reference to self
        path& concat(const b::fs::path& path);

        /// \brief Concatenate another path without intentionally adding a directory separator
        /// \param path The path to concatenate
        /// \return A reference to self
        path& operator+=(const b::fs::path& path);

        /// \brief Clear the contents of the path
        void clear();

        /// \brief Make preferred: convert to native format (Windows: backslashes, Linux: keep as-is)
        /// \return A reference to self
        path& make_preferred();

        /// \brief Make the path absolute, relative to the current working directory. (If it is relative)
        /// \details If the path is already absolute, this function does nothing.
        /// \throw b::filesystem_error if there is a filesystem error during the call.
        /// \return A reference to self
        path& make_absolute();

        /// \brief Remove the filename portion of the path. (Everything after the last directory separator)
        /// \return A reference to self
        path& remove_filename();

        /// \brief Replace or add the filename portion of the path. (Everything after the last directory separator)
        /// \param filename The filename to replace with
        /// \return A reference to self
        path& replace_filename(const b::fs::path& filename);

        /// \brief Replace or add the filename extension portion of the path.
        /// \param extension The extension to replace with
        /// \return A reference to self
        path& replace_extension(const b::fs::path& extension);

        /// \brief Swap the contents of two paths
        /// \param other The other path to swap with
        void swap(path& other) noexcept;

        /// \brief Get the path in generic format (On Windows, all separators converted to forward slashes)
        /// \return The path in generic string format
        [[nodiscard]] std::string string() const;

        /// \brief Get the path in platform-native format (Keeping backslashes and forward slashes as-is on all systems)
        /// \return The path in platform-native string format
        [[nodiscard]] std::string native_string() const;

        /// \brief Conversion operator to std::string
        /// \return The path in UTF-8 string format
        [[nodiscard]] operator std::string() const {
            return string();
        }

        /// \brief Lexicographical comparison of two paths. (e.g. "/a/b" < "/a/b/c")
        /// \param other The other path to compare with
        /// \return -1 if this path is less, 0 if they are equal, 1 if this path is greater
        [[nodiscard]] int compare(const path& other) const noexcept;

        /// \brief Normalize the path. (e.g. "/a/b/../c" -> "/a/c").
        /// \details This function is purely lexical, meaning it does not access the filesystem, does not check if
        ///          files exist, and does not follow symlinks. For that, see `b::fs::relative()` and `b::fs::proximate()`.
        /// \return The normalized path
        /// \see b::fs::relative()
        /// \see b::fs::proximate()
        [[nodiscard]] path lexically_normal() const;

        /// \brief Make the path relative to another path. (e.g. "/a/b/c" relative to "/a/d" results in "../b/c")
        /// \details This function is purely lexical, meaning it does not access the filesystem, does not check if
        ///          files exist, and does not follow symlinks. For that, see `b::fs::relative()` and `b::fs::proximate()`.
        ///          If no relative path is found (e.g. one of them is relative), an empty path is returned.
        ///          This makes it different from `lexically_proximate()`
        /// \param base The base path to make relative to
        /// \return The relative path, or an empty path if no relative path is found.
        /// \see b::fs::relative()
        /// \see b::fs::proximate()
        [[nodiscard]] path lexically_relative(const path& base) const;

        /// \brief Make the path relative to another path. (e.g. "/a/b/c" relative to "/a/d" results in "../b/c")
        /// \details This function is purely lexical, meaning it does not access the filesystem, does not check if
        ///          files exist, and does not follow symlinks. For that, see `b::fs::relative()` and `b::fs::proximate()`.
        ///          If no relative path is found (e.g. one of them is relative), the path itself is returned unchanged.
        ///          This makes it different from `lexically_relative()`
        /// \param base The base path to make relative to
        /// \return The relative path, or the unchanged path itself if no relative path is found.
        /// \see b::fs::relative()
        /// \see b::fs::proximate()
        [[nodiscard]] path lexically_proximate(const path& base) const;

        /// \brief Get the root portion of the path. (e.g. "/" on Linux, "C:" on Windows)
        /// \details If the path is relative, an empty path is returned. Use it in combination with `root_directory()`
        /// \return The root portion of the path, or an empty path if the path is relative.
        [[nodiscard]] path root_name() const;

        /// \brief Get the root directory of the path. (e.g. "" on Linux, "\" on Windows)
        /// \details If the path is relative, an empty path is returned. Use it in combination with `root_name()`
        /// \return The root directory of the path, or an empty path if the path is relative.
        [[nodiscard]] path root_directory() const;

        /// \brief Get the root path portion of the path. (e.g. "/" on Linux, "C:\" on Windows)
        /// \details This is equivalent to `root_name() / root_directory()`. If the path is relative,
        ///          an empty path is returned.
        /// \return The root path portion of the path, or an empty path if the path is relative.
        [[nodiscard]] path root_path() const;

        /// \brief Make the path relative to the `root_path()`. (e.g. "C:\Users\Test" -> "Users\Test")
        /// \details If the path is relative, the path itself is returned unchanged.
        /// \return A path relative to the `root_path()`, or the path itself if the path is relative.
        [[nodiscard]] path relative_root_path() const;

        /// \brief Get the parent path of the path. (e.g. "/a/b/c" -> "/a/b")
        /// \details If the path has no parent, the path itself is returned unchanged.
        /// \return The parent path of the path, or itself if the path has no parent.
        [[nodiscard]] path parent_path() const;

        /// \brief Get the filename portion of the path. (e.g. "/a/b/c" -> "c")
        /// \details If the path has no filename, an empty path is returned.
        /// \return The filename portion of the path, or an empty path if the path has no filename.
        [[nodiscard]] path filename() const;

        /// \brief Get the filename portion of the path without the extension. (e.g. "/a/b/c.txt" -> "c")
        /// \details If the path has no filename, an empty path is returned.
        /// \return The filename portion of the path without the extension, or an empty path if the path has no filename.
        [[nodiscard]] path stem() const;

        /// \brief Get the extension portion of the path. (e.g. "/a/b/c.txt" -> ".txt")
        /// \details If the path has no extension, an empty path is returned.
        /// \return The extension portion of the path, or an empty path if the path has no extension.
        [[nodiscard]] path extension() const;

        /// \brief Get the extension portion of the path, but without the dot. (e.g. "/a/b/c.txt" -> "txt")
        /// \details This function either returns the extension like "png" or "txt", or nothing ("")
        /// \return The extension portion of the path without the dot, or an empty path if the path has no extension.
        [[nodiscard]] path raw_extension() const;

        /// \brief Check if the path is empty.
        /// \return True if the path is empty, false otherwise.
        [[nodiscard]] bool empty() const noexcept;

        /// \brief Check if the path has a root path.
        /// \return True, false otherwise.
        [[nodiscard]] bool has_root_path() const;

        /// \brief Check if the path has a root name.
        /// \return True, false otherwise.
        [[nodiscard]] bool has_root_name() const;

        /// \brief Check if the path has a root directory.
        /// \return True, false otherwise.
        [[nodiscard]] bool has_root_directory() const;

        /// \brief Check if the path has a relative root path.
        /// \return True, false otherwise.
        [[nodiscard]] bool has_relative_root_path() const;

        /// \brief Check if the path has a parent path.
        /// \return True, false otherwise.
        [[nodiscard]] bool has_parent_path() const;

        /// \brief Check if the path has a filename.
        /// \return True, false otherwise.
        [[nodiscard]] bool has_filename() const;

        /// \brief Check if the path has a stem.
        /// \return True, false otherwise.
        [[nodiscard]] bool has_stem() const;

        /// \brief Check if the path has an extension.
        /// \return True, false otherwise.
        [[nodiscard]] bool has_extension() const;

        /// \brief Check if the path is absolute, i.e. has a root path in its native format
        /// \details Note: "/" is absolute on Linux and relative on Windows. It is the opposite of `is_relative()`.
        /// \return True, false otherwise.
        /// \see b::fs::path::is_relative()
        [[nodiscard]] bool is_absolute() const;

        /// \brief Check if the path is relative, i.e. has no root path in its native format
        /// \details Note: "/" is absolute on Linux and relative on Windows. It is the opposite of `is_absolute()`.
        /// \return True, false otherwise.
        /// \see b::fs::path::is_relative()
        [[nodiscard]] bool is_relative() const;

        /// \brief Get the native underlying path object as a copy.
        /// \details Whenever you must pass a path on to a system std function, make sure that it is Unicode agnostic.
        ///          You should always try to write `std_function(path.std_path().u8string())`, as this will pass it
        ///          as an std::u8string, containing UTF-8. Most system functions will correctly interpret std::u8string,
        ///          but plain std::string is almost always assumed to be a native encoding, like windows-1252 on Windows.
        /// \warning Only use this function if you know what you are doing! The `std::filesystem::path` object will
        ///          easily make your code fragile in terms of Unicode awareness if you are not careful.
        /// \return The native underlying path object as a copy.
        [[nodiscard]] std::filesystem::path std_path() const;

        /// \brief Get an iterator to the beginning of the path.
        /// \warning This function exposes iterators from the underlying `std::filesystem::path` object,
        ///          which is not Unicode aware. To make sure the encoding is correct, make sure to only
        ///          interact with it using wide strings.
        /// \return An iterator to the beginning of the path.
        [[nodiscard]] iterator begin() noexcept;

        /// \brief Get an iterator past the end of the path.
        /// \warning This function exposes iterators from the underlying `std::filesystem::path` object,
        ///          which is not Unicode aware. To make sure the encoding is correct, make sure to only
        ///          interact with it using wide strings.
        /// \return An iterator past the end of the path.
        [[nodiscard]] iterator end() noexcept;

        /// \brief Get an iterator to the beginning of the path.
        /// \warning This function exposes iterators from the underlying `std::filesystem::path` object,
        ///          which is not Unicode aware. To make sure the encoding is correct, make sure to only
        ///          interact with it using wide strings.
        /// \return An iterator to the beginning of the path.
        [[nodiscard]] const_iterator begin() const noexcept;

        /// \brief Get an iterator past the end of the path.
        /// \warning This function exposes iterators from the underlying `std::filesystem::path` object,
        ///          which is not Unicode aware. To make sure the encoding is correct, make sure to only
        ///          interact with it using wide strings.
        /// \return An iterator past the end of the path.
        [[nodiscard]] const_iterator end() const noexcept;

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

    // ========================================================
    // =================== End path class =====================
    // ========================================================



    /// \brief Make a relative path absolute, from the perspective of the current working directory
    /// \details This function is a wrapper around std::filesystem::absolute, with Unicode awareness.
    ///          No filesystem access is performed.
    /// \param path The path to make absolute
    /// \throw b::filesystem_error
    /// \return An absolute path that points to the same file as the input path, or an error code.
    [[nodiscard]] b::fs::path absolute(const b::fs::path& path);

    /// \brief Make a relative path absolute, from the perspective of the current working directory
    /// \details This function is a wrapper around std::filesystem::absolute, with Unicode awareness.
    ///          No filesystem access is performed.
    /// \param path The path to make absolute
    /// \return An absolute path that points to the same file as the input path, or an error code.
    [[nodiscard]] std::expected<b::fs::path,std::error_code> try_absolute(const b::fs::path& path) noexcept;

    /// \brief Canonicalize and absolute a path. The path must exist on-disk.
    /// \details Converts the path to an absolute path (from the current working directory) if it is not already, and
    ///          then removes any "." and ".." segments from the path, so that the path appears in its shortest form.
    /// \param path The path to canonicalize
    /// \throw b::filesystem_error
    /// \return An absolute path that points to the same file as the input path, in its shortest form, or an error code.
    [[nodiscard]] b::fs::path canonical(const b::fs::path& path);

    /// \brief Canonicalize and absolute a path. The path must exist on-disk.
    /// \details Converts the path to an absolute path (from the current working directory) if it is not already, and
    ///          then removes any "." and ".." segments from the path, so that the path appears in its shortest form.
    /// \param path The path to canonicalize
    /// \return An absolute path that points to the same file as the input path, in its shortest form, or an error code.
    [[nodiscard]] std::expected<b::fs::path,std::error_code> try_canonical(const b::fs::path& path) noexcept;

    /// \brief Make a path relative from the perspective of a base path.
    /// \details If no relative path can be constructed, an empty path is returned.
    ///          This makes this function different from `b::fs::proximate()`.
    /// \param path The path to make relative
    /// \param base The path to make the input path relative to
    /// \throw b::filesystem_error
    /// \return A relative path that points to the same file as the input path, or an error code.
    /// \see b::fs::proximate()
    [[nodiscard]] b::fs::path relative(const b::fs::path& path, const b::fs::path& base);

    /// \brief Make a path relative from the perspective of a base path.
    /// \details If no relative path can be constructed, an empty path is returned.
    ///          This makes this function different from `b::fs::proximate()`.
    /// \param path The path to make relative
    /// \param base The path to make the input path relative to
    /// \return A relative path that points to the same file as the input path, or an error code.
    /// \see b::fs::proximate()
    [[nodiscard]] std::expected<b::fs::path,std::error_code>
            try_relative(const b::fs::path& path, const b::fs::path& base) noexcept;

    /// \brief Make a path relative from the perspective of a base path.
    /// \details If no relative path can be constructed, the input path is returned unchanged.
    ///          This makes this function different from `b::fs::relative()`.
    /// \param path The path to make relative
    /// \param base The path to make the input path relative to
    /// \throw b::filesystem_error
    /// \return A relative path that points to the same file as the input path, or an error code.
    /// \see b::fs::relative()
    [[nodiscard]] b::fs::path proximate(const b::fs::path& path, const b::fs::path& base);

    /// \brief Make a path relative from the perspective of a base path.
    /// \details If no relative path can be constructed, the input path is returned unchanged.
    ///          This makes this function different from `b::fs::relative()`.
    /// \param path The path to make relative
    /// \param base The path to make the input path relative to
    /// \return A relative path that points to the same file as the input path, or an error code.
    /// \see b::fs::relative()
    [[nodiscard]] std::expected<b::fs::path,std::error_code>
            try_proximate(const b::fs::path& path, const b::fs::path& base) noexcept;

    /// \brief Copy a file or directory from one place to another.
    /// \details Use `b::fs::copy_options` to specify how the copy should be performed. use ...::recursive
    ///          to copy directories recursively, or ...::skip_symlinks to skip symbolic links. Only one option can
    ///          be specified at a time. Symlinks are followed by default.

    ///          You are required to use the return value for error checking, even though it does not contain
    ///          a real value. If you want to use exceptions, blindly call `b::fs::copy(...).value()`,
    ///          this will throw an exception if the result contains an error.
    ///          Consult https://en.cppreference.com/w/cpp/filesystem/copy for more.
    /// \param from The path to copy from
    /// \param to The path to copy to
    /// \param options Options to control how the copy is performed (optional)
    /// \return Nothing if the copy was successful, or an error code.
    /// \throw b::filesystem_error
    /// \see b::fs::copy_file()
    /// \see b::fs::copy_symlink()
    void copy(const b::fs::path& from, const b::fs::path& to,
              std::optional<b::fs::copy_options> options = std::nullopt);

    /// \brief Copy a file or directory from one place to another.
    /// \details Use `b::fs::copy_options` to specify how the copy should be performed. use ...::recursive
    ///          to copy directories recursively, or ...::skip_symlinks to skip symbolic links. Only one option can
    ///          be specified at a time. Symlinks are followed by default.

    ///          You are required to use the return value for error checking, even though it does not contain
    ///          a real value. If you want to use exceptions, blindly call `b::fs::copy(...).value()`,
    ///          this will throw an exception if the result contains an error.
    ///          Consult https://en.cppreference.com/w/cpp/filesystem/copy for more.
    /// \param from The path to copy from
    /// \param to The path to copy to
    /// \param options Options to control how the copy is performed (optional)
    /// \return Nothing if the copy was successful, or an error code.
    /// \see b::fs::copy_file()
    /// \see b::fs::copy_symlink()
    [[nodiscard]] std::expected<std::nullopt_t,std::error_code>
            try_copy(const b::fs::path& from, const b::fs::path& to,
                     std::optional<b::fs::copy_options> options = std::nullopt) noexcept;

    /// \brief Copy a regular file. Like `b::fs::copy()`, but throws an error if the source is not a regular file.
    /// \details See `b::fs::copy()` for more details. This function does the exact same thing, except it
    ///          only works for regular files. Use it if you want to copy a file and want to make sure that it is
    ///          certainly a regular file and not a directory or symlink.
    /// \param from The path to copy from
    /// \param to The path to copy to
    /// \param options Options to control how the copy is performed (optional)
    /// \return Nothing if the copy was successful, or an error code.
    /// \throw b::filesystem_error
    /// \see b::fs::copy()
    /// \see b::fs::copy_symlink()
    void copy_file(const b::fs::path& from, const b::fs::path& to,
                   std::optional<b::fs::copy_options> options = std::nullopt);

    /// \brief Copy a regular file. Like `b::fs::copy()`, but throws an error if the source is not a regular file.
    /// \details See `b::fs::copy()` for more details. This function does the exact same thing, except it
    ///          only works for regular files. Use it if you want to copy a file and want to make sure that it is
    ///          certainly a regular file and not a directory or symlink.
    /// \param from The path to copy from
    /// \param to The path to copy to
    /// \param options Options to control how the copy is performed (optional)
    /// \return Nothing if the copy was successful, or an error code.
    /// \see b::fs::copy()
    /// \see b::fs::copy_symlink()
    [[nodiscard]] std::expected<std::nullopt_t,std::error_code>
            try_copy_file(const b::fs::path& from, const b::fs::path& to,
                          std::optional<b::fs::copy_options> options = std::nullopt) noexcept;

    /// \brief Copy a symlink. Like `b::fs::copy()`, but it is used to copy a symlink itself.
    /// \details See `b::fs::copy()` for more details.
    /// \param from The path to copy from
    /// \param to The path to copy to
    /// \return Nothing if the copy was successful, or an error code.
    /// \throw b::filesystem_error
    /// \see b::fs::copy()
    /// \see b::fs::copy_file()
    void copy_symlink(const b::fs::path& from, const b::fs::path& to);

    /// \brief Copy a symlink. Like `b::fs::copy()`, but it is used to copy a symlink itself.
    /// \details See `b::fs::copy()` for more details.
    /// \param from The path to copy from
    /// \param to The path to copy to
    /// \return Nothing if the copy was successful, or an error code.
    /// \see b::fs::copy()
    /// \see b::fs::copy_file()
    [[nodiscard]] std::expected<std::nullopt_t,std::error_code>
            try_copy_symlink(const b::fs::path& from, const b::fs::path& to) noexcept;

    /// \brief Create a directory including parents on-disk. If the directory already exists, do nothing.
    /// \details Note: `create_directory` and `create_directories` as in `std::filesystem` are not both provided
    ///          separately, as it would not make sense from a high-level. This function is a wrapper around
    ///          `std::filesystem::create_directories` and `std::filesystem::create_directory` is not provided as
    ///          this function achieves the same thing with less error potential. Call
    ///          `b::fs::create_directory(...).value()` to only throw an exception if an error occurs.
    /// \param path The path to create
    /// \throw b::filesystem_error
    /// \return True if the directory was created, false otherwise, or an error code.
    bool create_directory(const b::fs::path& path);

    /// \brief Create a directory including parents on-disk. If the directory already exists, do nothing.
    /// \details Note: `create_directory` and `create_directories` as in `std::filesystem` are not both provided
    ///          separately, as it would not make sense from a high-level. This function is a wrapper around
    ///          `std::filesystem::create_directories` and `std::filesystem::create_directory` is not provided as
    ///          this function achieves the same thing with less error potential. Call
    ///          `b::fs::create_directory(...).value()` to only throw an exception if an error occurs.
    /// \param path The path to create
    /// \return True if the directory was created, false otherwise, or an error code.
    [[nodiscard]] std::expected<bool,std::error_code> try_create_directory(const b::fs::path& path) noexcept;

    /// \brief Create a hard link. Once created, the hard link is indistinguishable from the original file.
    /// \details Even if the original file is deleted, the file still exists and can be accessed through the hard link.
    ///          You are required to use the return value for error checking.
    ///          Call `b::fs::create_hard_link(...).value()` to throw an exception if an error occurs.
    /// \param target The path to create a hard link to
    /// \param link The path to the hard link
    /// \throw b::filesystem_error
    /// \return Nothing if the creation was successful, or an error code.
    void create_hard_link(const b::fs::path& target, const b::fs::path& link);

    /// \brief Create a hard link. Once created, the hard link is indistinguishable from the original file.
    /// \details Even if the original file is deleted, the file still exists and can be accessed through the hard link.
    ///          You are required to use the return value for error checking.
    ///          Call `b::fs::create_hard_link(...).value()` to throw an exception if an error occurs.
    /// \param target The path to create a hard link to
    /// \param link The path to the hard link
    /// \return Nothing if the creation was successful, or an error code.
    [[nodiscard]] std::expected<std::nullopt_t,std::error_code>
            try_create_hard_link(const b::fs::path& target, const b::fs::path& link) noexcept;

    /// \brief Create a symbolic link between regular files. The target file can be accessed through the symlink.
    /// \param target The path to create a symlink to
    /// \param link The path of the symlink
    /// \throw b::filesystem_error
    /// \return Nothing if the creation was successful, or an error code.
    void create_symlink(const b::fs::path& target, const b::fs::path& link);

    /// \brief Create a symbolic link between regular files. The target file can be accessed through the symlink.
    /// \param target The path to create a symlink to
    /// \param link The path of the symlink
    /// \return Nothing if the creation was successful, or an error code.
    [[nodiscard]] std::expected<std::nullopt_t,std::error_code>
            try_create_symlink(const b::fs::path& target, const b::fs::path& link) noexcept;

    /// \brief Create a symbolic link between directories. The target directory can be accessed through the symlink.
    /// \param target The path to create a symlink to
    /// \param link The path to the symlink
    /// \throw b::filesystem_error
    /// \return Nothing if the creation was successful, or an error code.
    void create_directory_symlink(const b::fs::path& target, const b::fs::path& link);

    /// \brief Create a symbolic link between directories. The target directory can be accessed through the symlink.
    /// \param target The path to create a symlink to
    /// \param link The path to the symlink
    /// \return Nothing if the creation was successful, or an error code.
    [[nodiscard]] std::expected<std::nullopt_t,std::error_code>
            try_create_directory_symlink(const b::fs::path& target, const b::fs::path& link) noexcept;

    /// \brief Get the current working directory
    /// \return The current working directory or an error code
    /// \throw b::filesystem_error
    /// \see b::fs::current_path()
    [[nodiscard]] b::fs::path current_path();

    /// \brief Get the current working directory
    /// \return The current working directory or an error code
    /// \see b::fs::current_path()
    [[nodiscard]] std::expected<b::fs::path,std::error_code> try_current_path() noexcept;

    /// \brief Set the current working directory and return it
    /// \param path The new current working directory
    /// \return The new current working directory or an error code
    /// \throw b::filesystem_error
    /// \see b::fs::current_path()
    void current_path(const b::fs::path& path);

    /// \brief Set the current working directory and return it
    /// \param path The new current working directory
    /// \return The new current working directory or an error code
    /// \see b::fs::current_path()
    [[nodiscard]] std::expected<std::nullopt_t,std::error_code> try_current_path(const b::fs::path& path) noexcept;

    /// \brief Check if a path exists on-disk. This can either be a file or a directory. Symlinks are followed.
    /// \param path The path to check
    /// \return True if the path exists, false otherwise, or an error code.
    /// \throw b::filesystem_error
    [[nodiscard]] bool exists(const b::fs::path& path);

    /// \brief Check if a path exists on-disk. This can either be a file or a directory. Symlinks are followed.
    /// \param path The path to check
    /// \return True if the path exists, false otherwise, or an error code.
    [[nodiscard]] std::expected<bool,std::error_code> try_exists(const b::fs::path& path) noexcept;

    /// \brief Check if a path exists on-disk. This can either be a file or a directory. Symlinks are followed.
    ///        This function cannot fail.
    /// \param path The path to check
    /// \return True if the path exists, false otherwise, or an error code.
    [[nodiscard]] bool exists(b::fs::file_status status) noexcept;

    /// \brief Check if two file paths resolve to the same target. (file or directory)
    /// \details An error is reported if either of the paths does not exist. All hard links are equivalent and symlinks
    ///          are equivalent with their respective target.
    /// \param path1 The first path to check
    /// \param path2 The second path to check
    /// \throw b::filesystem_error
    /// \return True if both file paths have the same file status, false otherwise, or an error code.
    [[nodiscard]] bool equivalent(const b::fs::path& path1, const b::fs::path& path2);

    /// \brief Check if two file paths resolve to the same target. (file or directory)
    /// \details An error is reported if either of the paths does not exist. All hard links are equivalent and symlinks
    ///          are equivalent with their respective target.
    /// \param path1 The first path to check
    /// \param path2 The second path to check
    /// \return True if both file paths have the same file status, false otherwise, or an error code.
    [[nodiscard]] std::expected<bool,std::error_code> try_equivalent(const b::fs::path& path1,
                                                                     const b::fs::path& path2) noexcept;

    /// \brief Get the on-disk filesize of a file in bytes. This might be different from the string length.
    /// \warning The returned size is the size in bytes a file has on-disk. If you read the file as text,
    ///          the string length after reading will be different. Firstly, because it is usually stored in UTF-8
    ///          and the number of bytes does not correspond to the number of characters, and secondly because
    ///          CRLF line endings on Windows are converted to LF when reading a text file, which also changes the
    ///          number of bytes. Use this function only if you specifically need the number of bytes on-disk.
    ///          Otherwise, read the file as text and retrieve the string length.
    /// \param path The path to the file
    /// \throw b::filesystem_error
    /// \return The on-disk filesize in bytes or an error code.
    [[nodiscard]] size_t file_size(const b::fs::path& path);

    /// \brief Get the on-disk filesize of a file in bytes. This might be different from the string length.
    /// \warning The returned size is the size in bytes a file has on-disk. If you read the file as text,
    ///          the string length after reading will be different. Firstly, because it is usually stored in UTF-8
    ///          and the number of bytes does not correspond to the number of characters, and secondly because
    ///          CRLF line endings on Windows are converted to LF when reading a text file, which also changes the
    ///          number of bytes. Use this function only if you specifically need the number of bytes on-disk.
    ///          Otherwise, read the file as text and retrieve the string length.
    /// \param path The path to the file
    /// \return The on-disk filesize in bytes or an error code.
    [[nodiscard]] std::expected<size_t,std::error_code> try_file_size(const b::fs::path& path) noexcept;

    /// \brief Get how many hard links refer to a file or directory.
    /// \details See https://en.cppreference.com/w/cpp/filesystem/hard_link_count
    /// \param path The path to the file or directory
    /// \throw b::filesystem_error
    /// \return The number of hard links or an error code.
    [[nodiscard]] size_t hard_link_count(const b::fs::path& path);

    /// \brief Get how many hard links refer to a file or directory.
    /// \details See https://en.cppreference.com/w/cpp/filesystem/hard_link_count
    /// \param path The path to the file or directory
    /// \return The number of hard links or an error code.
    [[nodiscard]] std::expected<size_t,std::error_code> try_hard_link_count(const b::fs::path& path) noexcept;

    /// \brief Get the last modification time of a file or directory. Symlinks are followed.
    /// \param path The path to check
    /// \throw b::filesystem_error
    /// \return The last modification time of the file or directory, or an error code.
    [[nodiscard]] b::fs::file_time_type last_write_time(const b::fs::path& path);

    /// \brief Get the last modification time of a file or directory. Symlinks are followed.
    /// \param path The path to check
    /// \return The last modification time of the file or directory, or an error code.
    [[nodiscard]] std::expected<b::fs::file_time_type,std::error_code>
            try_last_write_time(const b::fs::path& path) noexcept;

    /// \brief Set the last modification time of a file or directory to a specified value. Symlinks are followed.
    /// \param path The path to check
    /// \param newTime The new last modification time
    /// \throw b::filesystem_error
    /// \return The newly set last modification time of the file or directory, or an error code.
    void last_write_time(const b::fs::path& path, b::fs::file_time_type newTime);

    /// \brief Set the last modification time of a file or directory to a specified value. Symlinks are followed.
    /// \param path The path to check
    /// \return The newly set last modification time of the file or directory, or an error code.
    [[nodiscard]] std::expected<std::nullopt_t,std::error_code>
            try_last_write_time(const b::fs::path& path, b::fs::file_time_type newTime) noexcept;

    /// \brief Set permissions on a file or directory. Symlinks are followed unless the `nofollow` option is set.
    /// \details See https://en.cppreference.com/w/cpp/filesystem/permissions. You are required to use the return
    ///          value for error handling. Call `b::fs::permissions(...).value()` to throw an exception on error.
    /// \param path The path to investigate
    /// \param perms The new permissions
    /// \param opts The options to use (optional)
    /// \throw b::filesystem_error
    /// \return Nothing or an error code.
    void permissions(const b::fs::path& path,
                     b::fs::perms perms,
                     b::fs::perm_options opts = b::fs::perm_options::replace);

    /// \brief Set permissions on a file or directory. Symlinks are followed unless the `nofollow` option is set.
    /// \details See https://en.cppreference.com/w/cpp/filesystem/permissions. You are required to use the return
    ///          value for error handling. Call `b::fs::permissions(...).value()` to throw an exception on error.
    /// \param path The path to investigate
    /// \param perms The new permissions
    /// \param opts The options to use (optional)
    /// \return Nothing or an error code.
    [[nodiscard]] std::expected<std::nullopt_t,std::error_code>
            try_permissions(const b::fs::path& path,
                            b::fs::perms perms,
                            b::fs::perm_options opts = b::fs::perm_options::replace) noexcept;

    /// \brief Read a symlink and return the path it points to. It is an error if the path is not an existing symlink.
    /// \param path The path to investigate
    /// \throw b::filesystem_error
    /// \return The path the symlink points to or an error code.
    [[nodiscard]] b::fs::path read_symlink(const b::fs::path& path);

    /// \brief Read a symlink and return the path it points to. It is an error if the path is not an existing symlink.
    /// \param path The path to investigate
    /// \return The path the symlink points to or an error code.
    [[nodiscard]] std::expected<b::fs::path,std::error_code> try_read_symlink(const b::fs::path& path) noexcept;

    /// \brief Remove a file or a directory from disk, recursively. If the path does not exist, do nothing.
    /// \details Symlinks are not followed, the link itself is removed. This function maps to
    ///          `std::filesystem::remove_all`. `std::filesystem::remove` is not provided separately, because this
    ///          function does the same thing with less error potential. Throwing an error if the file to be removed
    ///          does not exist, does not make sense from a high-level perspective.
    /// \param path The path to remove (file, directory or symlink)
    /// \throw b::filesystem_error
    /// \return The number of files and directories removed or an error code.
    size_t remove(const b::fs::path& path);

    /// \brief Remove a file or a directory from disk, recursively. If the path does not exist, do nothing.
    /// \details Symlinks are not followed, the link itself is removed. This function maps to
    ///          `std::filesystem::remove_all`. `std::filesystem::remove` is not provided separately, because this
    ///          function does the same thing with less error potential. Throwing an error if the file to be removed
    ///          does not exist, does not make sense from a high-level perspective.
    /// \param path The path to remove (file, directory or symlink)
    /// \return The number of files and directories removed or an error code.
    [[nodiscard]] std::expected<size_t,std::error_code> try_remove(const b::fs::path& path) noexcept;

    /// \brief Rename a file or directory on-disk. Symlinks are not followed.
    /// \details You are required to use the return value for error handling. Call `b::fs::rename(...).value()` to
    ///          simply throw an exception on error.
    /// \param oldPath The path to rename
    /// \param newPath The new path
    /// \throw b::filesystem_error
    /// \return Nothing or an error code.
    void rename(const b::fs::path& oldPath, const b::fs::path& newPath);

    /// \brief Rename a file or directory on-disk. Symlinks are not followed.
    /// \details You are required to use the return value for error handling. Call `b::fs::rename(...).value()` to
    ///          simply throw an exception on error.
    /// \param oldPath The path to rename
    /// \param newPath The new path
    /// \return Nothing or an error code.
    [[nodiscard]] std::expected<std::nullopt_t,std::error_code> try_rename(const b::fs::path& oldPath,
                                                                           const b::fs::path& newPath) noexcept;

    /// \brief Change the size of a file on-disk by discarding bytes or adding zero-bytes at the end.
    /// \details You are required to use the return value for error handling. Call `b::fs::resize_file(...).value()` to
    ///          simply throw an exception on error. Symlinks are followed.
    /// \param path The path to the file
    /// \param newSize The new size of the file in bytes
    /// \throw b::filesystem_error
    /// \return Nothing or an error code.
    void resize_file(const b::fs::path& path, size_t newSize);

    /// \brief Change the size of a file on-disk by discarding bytes or adding zero-bytes at the end.
    /// \details You are required to use the return value for error handling. Call `b::fs::resize_file(...).value()` to
    ///          simply throw an exception on error. Symlinks are followed.
    /// \param path The path to the file
    /// \param newSize The new size of the file in bytes
    /// \return Nothing or an error code.
    [[nodiscard]] std::expected<std::nullopt_t,std::error_code>
            try_resize_file(const b::fs::path& path, size_t newSize) noexcept;

    /// \brief Check how much space is available on the filesystem on which said path is located.
    /// \details See https://en.cppreference.com/w/cpp/filesystem/space
    /// \param path The path to the file or directory
    /// \throw b::filesystem_error
    /// \return The space info or an error code.
    [[nodiscard]] b::fs::space_info space(const b::fs::path& path);

    /// \brief Check how much space is available on the filesystem on which said path is located.
    /// \details See https://en.cppreference.com/w/cpp/filesystem/space
    /// \param path The path to the file or directory
    /// \return The space info or an error code.
    [[nodiscard]] std::expected<b::fs::space_info,std::error_code> try_space(const b::fs::path& path) noexcept;

    /// \brief Get the file status of a file or directory. Symlinks are followed.
    /// \details See https://en.cppreference.com/w/cpp/filesystem/status
    /// \param path The path to the file or directory
    /// \throw b::filesystem_error
    /// \return The file status or an error code.
    [[nodiscard]] b::fs::file_status status(const b::fs::path& path);

    /// \brief Get the file status of a file or directory. Symlinks are followed.
    /// \details See https://en.cppreference.com/w/cpp/filesystem/status
    /// \param path The path to the file or directory
    /// \return The file status or an error code.
    [[nodiscard]] std::expected<b::fs::file_status,std::error_code> try_status(const b::fs::path& path) noexcept;

    /// \brief Get the file status of a symlink.
    /// \details See https://en.cppreference.com/w/cpp/filesystem/status
    /// \param path The path to the symlink
    /// \throw b::filesystem_error
    /// \return The file status or an error code.
    [[nodiscard]] b::fs::file_status symlink_status(const b::fs::path& path);

    /// \brief Get the file status of a symlink.
    /// \details See https://en.cppreference.com/w/cpp/filesystem/status
    /// \param path The path to the symlink
    /// \return The file status or an error code.
    [[nodiscard]] std::expected<b::fs::file_status,std::error_code>
            try_symlink_status(const b::fs::path& path) noexcept;

    // std::filesystem::temp_directory_path is not provided. Use the b::folders module instead.

    /// \brief Check if a path on-disk refers to a block device.
    /// \details If the path does not exist, false is returned.
    ///          Files such as `/dev/sda` or `/dev/loop0` are checked for.
    /// \param path The directory path to check
    /// \throw b::filesystem_error
    /// \return True if the path refers to a block device, otherwise false
    [[nodiscard]] bool is_block_file(const b::fs::path& path);

    /// \brief Check if a path on-disk refers to a block device.
    /// \details If the path does not exist, false is returned.
    ///          Files such as `/dev/sda` or `/dev/loop0` are checked for.
    /// \param path The directory path to check
    /// \return True if the path refers to a block device, otherwise false, or an error code.
    [[nodiscard]] std::expected<bool,std::error_code> try_is_block_file(const b::fs::path& path) noexcept;

    /// \brief Check if a file status refers to a block device.
    /// \details If the path does not exist, false is returned. This function cannot fail.
    ///          Files such as `/dev/sda` or `/dev/loop0` are checked for.
    /// \param status The file status to check
    /// \return True if the path refers to a block device, otherwise false.
    [[nodiscard]] bool is_block_file(b::fs::file_status status) noexcept;

    /// \brief Check if a path on-disk refers to a character special file.
    /// \details If the path does not exist, false is returned.
    ///          Files such as `/dev/null` or `/dev/tty` are checked for.
    /// \param path The directory path to check
    /// \throw b::filesystem_error
    /// \return True if the path refers to a character special file, otherwise false
    [[nodiscard]] bool is_character_file(const b::fs::path& path);

    /// \brief Check if a path on-disk refers to a character special file.
    /// \details If the path does not exist, false is returned.
    ///          Files such as `/dev/null` or `/dev/tty` are checked for.
    /// \param path The directory path to check
    /// \return True if the path refers to a character special file, otherwise false, or an error code.
    [[nodiscard]] std::expected<bool,std::error_code> try_is_character_file(const b::fs::path& path) noexcept;

    /// \brief Check if a file status refers to a character special file.
    /// \details If the path does not exist, false is returned. This function cannot fail.
    ///          Files such as `/dev/null` or `/dev/tty` are checked for.
    /// \param status The file status to check
    /// \return True if the path refers to a character special file, otherwise false.
    [[nodiscard]] bool is_character_file(b::fs::file_status status) noexcept;

    /// \brief Check if a path on-disk refers to a directory.
    /// \details If the path does not exist, false is returned.
    /// \param path The directory path to check
    /// \throw b::filesystem_error
    /// \return True if the path refers to a directory, otherwise false
    [[nodiscard]] bool is_directory(const b::fs::path& path);

    /// \brief Check if a path on-disk refers to a directory.
    /// \details If the path does not exist, false is returned.
    /// \param path The directory path to check
    /// \return True if the path refers to a directory, otherwise false, or an error code.
    [[nodiscard]] std::expected<bool,std::error_code> try_is_directory(const b::fs::path& path) noexcept;

    /// \brief Check if a file status refers to a directory.
    /// \details If the path does not exist, false is returned. This function cannot fail.
    /// \param status The file status to check
    /// \return True if the path refers to a directory, otherwise false.
    [[nodiscard]] bool is_directory(b::fs::file_status status) noexcept;

    /// \brief Check if a path on-disk is an empty file or an empty directory.
    /// \details If the path does not exist, false is returned.
    /// \param path The directory path to check
    /// \throw b::filesystem_error
    /// \return True if the condition is true, otherwise false
    [[nodiscard]] bool is_empty(const b::fs::path& path);

    /// \brief Check if a path on-disk is an empty file or an empty directory.
    /// \details If the path does not exist, false is returned.
    /// \param path The directory path to check
    /// \return True if the condition is true, otherwise false, or an error code.
    [[nodiscard]] std::expected<bool,std::error_code> try_is_empty(const b::fs::path& path) noexcept;

    /// \brief Check if a path on-disk refers to a FIFO pipe.
    /// \details If the path does not exist, false is returned.
    /// \param path The directory path to check
    /// \throw b::filesystem_error
    /// \return True if the condition is true, otherwise false
    [[nodiscard]] bool is_fifo(const b::fs::path& path);

    /// \brief Check if a path on-disk refers to a FIFO pipe.
    /// \details If the path does not exist, false is returned.
    /// \param path The directory path to check
    /// \return True if the condition is true, otherwise false, or an error code.
    [[nodiscard]] std::expected<bool,std::error_code> try_is_fifo(const b::fs::path& path) noexcept;

    /// \brief Check if a file status refers to a FIFO pipe.
    /// \details If the path does not exist, false is returned. This function cannot fail.
    /// \param status The file status to check
    /// \return True if the condition is true, otherwise false.
    [[nodiscard]] bool is_fifo(b::fs::file_status status) noexcept;

    /// \brief Check if a path on-disk refers to a file of type 'other'.
    /// \details If the path does not exist, false is returned. True is returned if the path is neither a file,
    ///          nor a directory, nor a symlink.
    /// \param path The directory path to check
    /// \throw b::filesystem_error
    /// \return True if the condition is true, otherwise false
    [[nodiscard]] bool is_other(const b::fs::path& path);

    /// \brief Check if a path on-disk refers to a file of type 'other'.
    /// \details If the path does not exist, false is returned. True is returned if the path is neither a file,
    ///          nor a directory, nor a symlink.
    /// \param path The directory path to check
    /// \return True if the condition is true, otherwise false, or an error code.
    [[nodiscard]] std::expected<bool,std::error_code> try_is_other(const b::fs::path& path) noexcept;

    /// \brief Check if a file status refers to a file of type 'other'.
    /// \details If the path does not exist, false is returned. This function cannot fail. True is returned if
    ///          the path is neither a file, nor a directory, nor a symlink.
    /// \param status The file status to check
    /// \return True if the condition is true, otherwise false.
    [[nodiscard]] bool is_other(b::fs::file_status status) noexcept;

    /// \brief Check if a path on-disk refers to a regular file.
    /// \details If the path does not exist, false is returned.
    /// \param path The directory path to check
    /// \throw b::filesystem_error
    /// \return True if the condition is true, otherwise false
    [[nodiscard]] bool is_regular_file(const b::fs::path& path);

    /// \brief Check if a path on-disk refers to a regular file.
    /// \details If the path does not exist, false is returned.
    /// \param path The directory path to check
    /// \return True if the condition is true, otherwise false, or an error code.
    [[nodiscard]] std::expected<bool,std::error_code> try_is_regular_file(const b::fs::path& path) noexcept;

    /// \brief Check if a file status refers to a regular file.
    /// \details If the path does not exist, false is returned. This function cannot fail.
    /// \param status The file status to check
    /// \return True if the condition is true, otherwise false.
    [[nodiscard]] bool is_regular_file(b::fs::file_status status) noexcept;

    /// \brief Check if a path on-disk refers to a named IPC socket.
    /// \details If the path does not exist, false is returned.
    /// \param path The directory path to check
    /// \throw b::filesystem_error
    /// \return True if the condition is true, otherwise false
    [[nodiscard]] bool is_socket(const b::fs::path& path);

    /// \brief Check if a path on-disk refers to a named IPC socket.
    /// \details If the path does not exist, false is returned.
    /// \param path The directory path to check
    /// \return True if the condition is true, otherwise false, or an error code.
    [[nodiscard]] std::expected<bool,std::error_code> try_is_socket(const b::fs::path& path) noexcept;

    /// \brief Check if a file status refers to a named IPC socket.
    /// \details If the path does not exist, false is returned. This function cannot fail.
    /// \param status The file status to check
    /// \return True if the condition is true, otherwise false.
    [[nodiscard]] bool is_socket(b::fs::file_status status) noexcept;

    /// \brief Check if a path on-disk refers to a symlink.
    /// \details If the path does not exist, false is returned.
    /// \param path The directory path to check
    /// \throw b::filesystem_error
    /// \return True if the condition is true, otherwise false
    [[nodiscard]] bool is_symlink(const b::fs::path& path);

    /// \brief Check if a path on-disk refers to a symlink.
    /// \details If the path does not exist, false is returned.
    /// \param path The directory path to check
    /// \return True if the condition is true, otherwise false, or an error code.
    [[nodiscard]] std::expected<bool,std::error_code> try_is_symlink(const b::fs::path& path) noexcept;

    /// \brief Check if a file status refers to a symlink.
    /// \details If the path does not exist, false is returned. This function cannot fail.
    /// \param status The file status to check
    /// \return True if the condition is true, otherwise false.
    [[nodiscard]] bool is_symlink(b::fs::file_status status) noexcept;

    /// \brief Check if a file status is known. This function cannot fail.
    /// \param status The file status to check.
    /// \return True if the status is known, otherwise false.
    [[nodiscard]] bool status_known(b::fs::file_status status) noexcept;

    /// \brief An improved file input stream, that guarantees to work with Unicode file paths on every platform.
    /// \note You are encouraged to use `b::fs::read()` and `b::fs::write()` instead of this class, wherever possible.
    /// \note Be aware that the number of bytes needed to represent the data on disk may be different from the number
    ///       of characters in the string. Do not take the file size as a measure of the number of characters.
    class ifstream : public std::ifstream {
    public:
        ifstream(const path& path, std::ios_base::openmode mode = std::ios_base::in)
                : std::ifstream(b::to_native(path.string()).c_str(), mode) {}
        ~ifstream() override = default;
    };

    /// \brief An improved file output stream, that guarantees to work with Unicode file paths on every platform.
    /// \note You are encouraged to use `b::fs::read()` and `b::fs::write()` instead of this class, wherever possible.
    ///       The only situation where you must use this class is when you want to write a file in chunks.
    /// \note Be aware that the number of bytes needed to represent the data on disk may be different from the number
    ///       of characters in the string. Do not take the file size as a measure of the number of characters.
    class ofstream : public std::ofstream {
    public:
        ofstream(const fs::path& path, std::ios_base::openmode filemode, bool createDirectory = true)
                : std::ofstream(create_dir_return_path(path, createDirectory).c_str(), filemode) {}
        ~ofstream() override = default;

    private:
        [[nodiscard]] b::native_string create_dir_return_path(const fs::path& path, bool createDirectory) const {
            if (path.has_parent_path() && createDirectory) {
                if (!fs::exists(path.parent_path())) {
                    fs::create_directory(path.parent_path());
                }
            }
            return b::to_native(path.string());
        }
    };

    /// \brief Read the contents of a text file as a string
    /// \details The file is read in binary mode and stored in the string as-is. No encoding conversion is performed.
    ///          Line Endings are preserved. Any file should always be written with LF line endings anyways.
    ///          You are required to interpret the returned string as UTF-8 as enforced by https://utf8everywhere.org.
    /// \note Be aware that the number of bytes needed to represent the data on disk may be different from the number
    ///       of characters in the string. Do not take the file size as a measure of the number of characters.
    /// \param path The b::fs::path file path to load the file from
    /// \return std::string with the UTF-8 encoded file contents
    /// \see b::fs::read()
    std::expected<std::string, b::filesystem_error> try_read(const b::fs::path &path);

    /// \brief Read the contents of a text file as a string
    /// \details The file is read in binary mode and stored in the string as-is. No encoding conversion is performed.
    ///          You are required to interpret the returned string as UTF-8 as enforced by https://utf8everywhere.org.
    /// \note Be aware that the number of bytes needed to represent the data on disk may be different from the number
    ///       of characters in the string. Do not take the file size as a measure of the number of characters.
    /// \param path The b::fs::path file path to load the file from
    /// \throw b::filesystem_error if loading the file fails
    /// \return std::string with the UTF-8 encoded file contents
    /// \see b::fs::try_read()
    std::string read(const fs::path &path);

    /// \brief Read the contents of a binary file as a b::bytearray
    /// \details Use this function for all non-text files such as images, audio, video, etc.
    ///          Do not ever store binary data in an `std::string`, use `b::bytearray` for that instead.
    /// \param path The b::fs::path file path to load the file from
    /// \return std::string with the UTF-8 encoded file contents
    /// \see b::fs::read()
    std::expected<b::bytearray, b::filesystem_error> try_read_binary(const b::fs::path &path);

    /// \brief Read the contents of a binary file as a b::bytearray
    /// \details Use this function for all non-text files such as images, audio, video, etc.
    ///          Do not ever store binary data in an `std::string`, use `b::bytearray` for that instead.
    /// \param path The b::fs::path file path to load the file from
    /// \throw b::filesystem_error if loading the file fails
    /// \return std::string with the UTF-8 encoded file contents
    /// \see b::fs::try_read()
    b::bytearray read_binary(const fs::path &path);

    /// \brief Read the contents of a text file in chunks
    /// \details The chunk size is specified, as well as a callback function (usually a lambda), that is
    ///          called for every chunk being read. When the read is finished, the total number of bytes read
    ///          is being returned, or a b::filesystem_error object. To write a file in chunks, use
    ///          `b::fs::ofstream` directly.
    /// \note Be aware that the number of bytes needed to represent the data on disk may be different from the number
    ///       of characters in the string. Do not take the file size as a measure of the number of characters.
    /// \tparam TFunc The type of the callback function (do not specify)
    /// \param path The b::fs::path file path to load the file from
    /// \param chunk_size The size of the chunks to read
    /// \param callback The callback function to call for every chunk read
    /// \return The total number of bytes read, or a b::filesystem_error exception object
    template<typename TFunc>
    inline static std::expected<size_t, b::filesystem_error>
            try_read_chunked(const fs::path& path, size_t chunk_size, TFunc callback) {
        b::fs::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return std::unexpected(b::filesystem_error(
                    b::format("Failed loading file {}: {}", path.string(), b::strerror(errno))
            ));
        }

        std::string buffer(chunk_size, 0);
        size_t totalBytes = 0;
        while (!file.eof()) {
            file.read(buffer.data(), buffer.size());    // Read a chunk of the file
            auto thisChunkSize = file.gcount();
            totalBytes += thisChunkSize;

            if (thisChunkSize != 0) {
                auto chunk = std::string(buffer.data(), static_cast<size_t>(thisChunkSize));
                callback(chunk);
            }
        }

        return totalBytes;
    }

    /// \brief Read the contents of a text file in chunks
    /// \details The chunk size is specified, as well as a callback function (usually a lambda), that is
    ///          called for every chunk being read. When the read is finished, the total number of bytes read
    ///          is being returned, or a b::filesystem_error object.
    /// \note Be aware that the number of bytes needed to represent the data on disk may be different from the number
    ///       of characters in the string. Do not take the file size as a measure of the number of characters.
    /// \tparam TFunc The type of the callback function (do not specify)
    /// \param path The b::fs::path file path to load the file from
    /// \param chunk_size The size of the chunks to read
    /// \param callback The callback function to call for every chunk read
    /// \throw b::filesystem_error if loading the file fails
    /// \return The total number of bytes read
    template<typename TFunc>
    inline static size_t read_chunked(const fs::path& path, size_t chunk_size, TFunc callback) {
        auto result = try_read_chunked(path, chunk_size, callback);
        if (!result) {
            throw result.error();
        }
        return result.value();
    }

    /// \brief Read the contents of a binary file in chunks
    /// \details The chunk size is specified, as well as a callback function (usually a lambda), that is
    ///          called for every chunk being read. When the read is finished, the total number of bytes read
    ///          is being returned, or a b::filesystem_error object. To write a file in chunks, use
    ///          `b::fs::ofstream` directly.
    /// \note Be aware that the number of bytes needed to represent the data on disk may be different from the number
    ///       of characters in the string. Do not take the file size as a measure of the number of characters.
    /// \tparam TFunc The type of the callback function (do not specify)
    /// \param path The b::fs::path file path to load the file from
    /// \param chunk_size The size of the chunks to read
    /// \param callback The callback function to call for every chunk read
    /// \return The total number of bytes read, or a b::filesystem_error exception object
    template<typename TFunc>
    inline static std::expected<size_t, b::filesystem_error>
            try_read_binary_chunked(const fs::path& path, size_t chunk_size, TFunc callback) {
        b::fs::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return std::unexpected(b::filesystem_error(
                    b::format("Failed loading file {}: {}", path, b::strerror(errno))
            ));
        }

        std::string buffer(chunk_size, 0);
        size_t totalBytes = 0;
        while (!file.eof()) {
            file.read(buffer.data(), buffer.size());    // Read a chunk of the file
            auto thisChunkSize = file.gcount();
            totalBytes += thisChunkSize;

            if (thisChunkSize != 0) {
                auto chunk = std::string(buffer.data(), static_cast<size_t>(thisChunkSize));
                callback(b::bytearray(chunk.begin(), chunk.end()));
            }
        }

        return totalBytes;
    }

    /// \brief Read the contents of a binary file in chunks
    /// \details The chunk size is specified, as well as a callback function (usually a lambda), that is
    ///          called for every chunk being read. When the read is finished, the total number of bytes read
    ///          is being returned, or a b::filesystem_error object.
    /// \note Be aware that the number of bytes needed to represent the data on disk may be different from the number
    ///       of characters in the string. Do not take the file size as a measure of the number of characters.
    /// \tparam TFunc The type of the callback function (do not specify)
    /// \param path The b::fs::path file path to load the file from
    /// \param chunk_size The size of the chunks to read
    /// \param callback The callback function to call for every chunk read
    /// \throw b::filesystem_error if loading the file fails
    /// \return The total number of bytes read
    template<typename TFunc>
    inline static size_t read_binary_chunked(const fs::path& path, size_t chunk_size, TFunc callback) {
        auto result = try_read_binary_chunked(path, chunk_size, callback);
        if (!result) {
            throw result.error();
        }
        return result.value();
    }

    /// \brief Write into a file on-disk. Parent folders are automatically created.
    /// \details You must make sure that the content being written to the file is UTF-8 encoded, if the data
    ///          represents text. Line Endings are preserved by this function.
    /// \note Be aware that the number of bytes needed to represent the data on disk may be different from the number
    ///       of characters in the string. Do not take the file size as a measure of the number of characters.
    /// \warning Always remember to respect https://utf8everywhere.org/. Never write a file to disk with anything
    ///          other than UTF-8 encoding, unless you have a very good reason to do so.
    /// \param path The b::fs::path file path to open the file from
    /// \param content The content to write into the file (b::bytearray or b::string, depending on the encoding)
    /// \return The number of bytes written to disk, or a b::filesystem_error exception object
    /// \see b::fs::write()
    std::expected<size_t, b::filesystem_error> try_write(const fs::path &path, const std::string& content);

    /// \brief Overload of `b::fs::try_write()`
    /// \see b::fs::try_write()
    std::expected<size_t, b::filesystem_error> try_write(const fs::path &path, const b::bytearray& content);

    /// \brief Write into a file on-disk. Parent folders are automatically created.
    /// \details You must make sure that the content being written to the file is UTF-8 encoded, if the data
    ///          represents text. Line Endings are preserved by this function.
    /// \note Be aware that the number of bytes needed to represent the data on disk may be different from the number
    ///       of characters in the string. Do not take the file size as a measure of the number of characters.
    /// \warning Always remember to respect https://utf8everywhere.org/. Never write a file to disk with anything
    ///          other than UTF-8 encoding, unless you have a very good reason to do so.
    /// \param path The b::fs::path file path to open the file from
    /// \param content The content to write into the file (b::bytearray or b::string, depending on the encoding)
    /// \throw b::filesystem_error if writing the file fails
    /// \return The number of bytes written to disk, or a b::filesystem_error exception object
    /// \see b::fs::try_write()
    size_t write(const fs::path &path, const std::string& content);

    /// \brief Overload of `b::fs::try_write()`
    /// \see b::fs::try_write()
    size_t write(const fs::path &path, const b::bytearray& content);

} // namespace b::fs

// Hash function that lets b::fs::path be used as a key in std::map and std::unordered_map
namespace std {
    template <> struct hash<b::fs::path> {
        size_t operator()(const b::fs::path& path) const {
            return std::hash<std::string>()(path.string());
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

#endif // BATTERY_CORE_FS_HPP