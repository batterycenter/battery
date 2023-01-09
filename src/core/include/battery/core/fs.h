#pragma once

#include "battery/core/OsString.h"
#include <filesystem>
#include <string>
#include <fstream>
#include <functional>

// First of all, allow std::u8string to be printed to std::cout
inline std::ostream& operator<<(std::ostream& os, const std::u8string& str) {
    os << reinterpret_cast<const char*>(str.c_str());
    return os;
}

namespace battery::fs {

    using std::filesystem::exists;					// Checks if a path exists on-disk (file or directory)
    using std::filesystem::create_directory;		// Creates a single directory
    using std::filesystem::create_directories;		// Creates a directory and all above
    using std::filesystem::copy;					// Copy either a file or a directory

    using std::filesystem::status;					// Get filesystem status: what type of entry it is
    using std::filesystem::is_regular_file;
    using std::filesystem::is_directory;



    // TODO: filesystem::path seems to be incorrect for UTF-8 (try status())
    class path : public std::filesystem::path {
    public:
        path() : std::filesystem::path() {}
        path(const char* path) : std::filesystem::path(path) {}
        path(const std::string& path) : std::filesystem::path(path) {}

        path(const std::filesystem::path& path) : std::filesystem::path(path) {}
        path(std::filesystem::path& path) : std::filesystem::path(path) {}
        path(std::filesystem::path&& path) : std::filesystem::path(path) {}

        virtual ~path() = default;

        std::string string() const = delete;
        std::string to_string() const {
            return reinterpret_cast<const char*>(std::filesystem::path::u8string().c_str());
        }

    };

    enum class Mode {
        TEXT,
        BINARY
    };

    class ifstream : public std::ifstream {
    public:
        ifstream(const std::string& path, enum Mode filemode = Mode::TEXT)
                : std::ifstream(convert(path),
                                (filemode == Mode::TEXT) ? (std::ios::in) : (std::ios::in | std::ios::binary)) {
            this->path = path;
            binary = (filemode == Mode::BINARY);	// remember for later
        }

        ifstream(const std::string& path, std::ios_base::openmode mode)
                : std::ifstream(convert(path), mode) {
            this->path = path;
            binary = (mode & std::ios::binary);		// remember for later
        }

        virtual ~ifstream() {}

        // In Binary mode this function is cheap, it reads the filesize from the filesystem.
        // In Text mode this function is expensive, it reads the entire file into memory, just for the filesize.
        // There is no other way to get the filesize in text mode, because the representation on-disk is different
        // from what is read into memory. (Line endings)
        size_t file_size() {
            if (binary) {		// Binary mode
                return std::filesystem::file_size(convert(path));
            }
            else {				// Text mode: read the entire file into memory to compensate line-endings
                // Buffer
                std::string temp;													// Buffer is as large as the file on-disk
                size_t filesize_temp = std::filesystem::file_size(convert(path));   // -> Reading will make it smaller, but not larger
                temp.resize(filesize_temp);

                // Read file
                fs::ifstream file(path, Mode::TEXT);
                file.read(temp.data(), temp.size());
                return file.gcount();
            }
        }

        size_t read_in_chunks(size_t chunk_size, std::function<void(std::string_view)> callback) {
            std::string buffer(chunk_size, 0);
            size_t total_bytes = 0;

            while (!this->eof()) {
                this->read(buffer.data(), buffer.size());    // Read a chunk of the file
                size_t this_chunk_size = this->gcount();
                total_bytes += this_chunk_size;

                callback(std::string_view(buffer.data(), this_chunk_size));
            }

            return total_bytes;
        }

    private:
        std::wstring convert(const std::string& path) {
            return OsString(path).wstr();
        }

        std::string path;
        bool binary = false;
    };

    class ofstream : public std::ofstream {
    public:
        ofstream(const std::string& path, enum Mode filemode = Mode::TEXT, bool createDirectory = true)
                : std::ofstream((create_directory(createDirectory, path), convert(path)),
                                (filemode == Mode::TEXT) ? (std::ios::out) : (std::ios::out | std::ios::binary)) {}

        ofstream(const std::string& path, std::ios_base::openmode mode, bool createDirectory = true)
                : std::ofstream((create_directory(createDirectory, path), convert(path)), mode) {}

        virtual ~ofstream() {}

    private:
        std::wstring convert(const std::string& path) {
            return OsString(path).wstr();
        }

        static void create_directory(bool create, const std::string& path) {
            if (!create)
                return;

            // File or directory already exists
            if (fs::exists(path)) {
                return;
            }

            // Otherwise, now create a directory
            fs::create_directories(fs::path(path).parent_path());
        }
    };

    /// <summary>
    /// Throws:
    /// Battery::LockfileUnavailableException if the lockfile is already locked or
    /// Battery::NoSuchFileOrDirectoryException if the filename is invalid or contains directories
    /// </summary>
    class Lockfile {
    public:
        Lockfile(const std::string& filename, bool createDirectories = false);
        ~Lockfile();

    private:
        void* fileDescriptor = nullptr;
    };

}
