#pragma once

#include "battery/core/OsString.h"
#include "battery/core/string.h"
#include <filesystem>
#include <string>
#include <fstream>
#include <functional>
#include <bit>

namespace battery::fs {

    using std::filesystem::exists;					// Checks if a path exists on-disk (file or directory)
    using std::filesystem::create_directory;		// Creates a single directory
    using std::filesystem::create_directories;		// Creates a directory and all above
    using std::filesystem::copy;					// Copy either a file or a directory
    using std::filesystem::remove;                  // Delete a file (symlinks not followed)
    using std::filesystem::remove_all;              // Recursively delete a directory (symlinks not followed)

    using std::filesystem::status;					// Get filesystem status: what type of entry it is
    using std::filesystem::is_regular_file;
    using std::filesystem::is_directory;
    using std::filesystem::current_path;



    // TODO: filesystem::path seems to be incorrect for UTF-8 (try status())
    class path : public std::filesystem::path {
    public:
        path() = default;
        path(const char* path) : std::filesystem::path(battery::string::string_to_u8string(path)) {}
        path(const char8_t* path) : std::filesystem::path(path) {}
        path(const std::string& path) : std::filesystem::path(battery::string::string_to_u8string(path)) {}
        path(const std::u8string& path) : std::filesystem::path(path) {}

        ~path() = default;

        std::string string() const = delete;
        std::string to_string() const {
            return std::bit_cast<const char*>(this->u8string().c_str());
        }

        fs::path& operator+=(const std::string& path) {
            this->append(path);
            return *this;
        }

    };

    inline fs::path operator+(const fs::path& a, const std::string& b) {
        fs::path path = a;
        path.append(b);
        return path;
    }

    enum class Mode {
        TEXT,
        BINARY
    };

    class ifstream : public std::ifstream {
    public:
        ifstream(const battery::fs::path& path, enum Mode filemode = Mode::TEXT)
                : std::ifstream(convert(path), (filemode == Mode::TEXT) ? std::ios::in : (std::ios::in | std::ios::binary)),
                  path(path)
        {
            binary = (filemode == Mode::BINARY);	// remember for later
        }

        ifstream(const battery::fs::path& path, std::ios_base::openmode mode) : std::ifstream(convert(path), mode), path(path)  {
            binary = (mode & std::ios::binary);		// remember for later
        }

        ~ifstream() override = default;

        // In Binary mode this function is cheap, it reads the filesize from the filesystem.
        // In Text mode this function is expensive, it reads the entire file into memory, just for the filesize.
        // There is no other way to get the filesize in text mode, because the representation on-disk is different
        // from what is read into memory. (Line endings)
        std::streamsize file_size() const {
            if (binary) {		// Binary mode
                return std::filesystem::file_size(convert(path));
            }
            else {				// Text mode: read the entire file into memory to compensate line-endings
                std::string temp;	// Buffer is as large as the file on-disk -> Reading will make it smaller, but not larger
                auto filesize_temp = std::filesystem::file_size(convert(path));
                temp.resize(static_cast<size_t>(filesize_temp));

                // Read file
                fs::ifstream file(path, Mode::TEXT);
                file.read(temp.data(), temp.size());
                return static_cast<size_t>(file.gcount());
            }
        }

        template<typename TFunc>
        size_t read_in_chunks(size_t chunk_size, TFunc callback) {
            std::string buffer(chunk_size, 0);
            size_t total_bytes = 0;

            while (!this->eof()) {
                this->read(buffer.data(), buffer.size());    // Read a chunk of the file
                auto this_chunk_size = this->gcount();
                total_bytes += this_chunk_size;

                if (this_chunk_size != 0) {
                    callback(std::string_view(buffer.data(), static_cast<size_t>(this_chunk_size)));
                }
            }

            return total_bytes;
        }

        std::string to_string() {   // NOLINT
            return std::string((std::istreambuf_iterator<char>(*this)), std::istreambuf_iterator<char>());
        }

        void return_to_beginning() {
            this->clear();                   // clear fail and eof bits
            this->seekg(0, std::ios::beg);   // back to the start!
        }

        explicit operator std::string() {
            return to_string();
        }

    private:
        std::wstring convert(const battery::fs::path& _path) const {
            return OsString(_path.to_string()).wstr();
        }

        battery::fs::path path;
        bool binary = false;
    };

    class ofstream : public std::ofstream {
    public:
        ofstream(const battery::fs::path& path, enum Mode filemode = Mode::TEXT, bool createDirectory = true)
                : std::ofstream(create_dir_return_path(path, createDirectory), parse_mode(filemode)) {}

        ofstream(const battery::fs::path& path, std::ios_base::openmode filemode, bool createDirectory = true)
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

        std::wstring create_dir_return_path(const battery::fs::path& path, bool createDirectory) const {
            if (path.has_parent_path() && createDirectory) {
                auto parent = path.parent_path();
                if (!fs::exists(parent)) {
                    fs::create_directories(fs::path(path).parent_path());
                }
            }
            return OsString(path).wstr();
        }
    };

    class Lockfile {
    public:
        Lockfile(const std::string& filename, bool createDirectories = false);
        ~Lockfile();

    private:
        void* fileDescriptor = nullptr;
    };

}
