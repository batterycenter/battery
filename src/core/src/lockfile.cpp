
#include "battery/core/lockfile.hpp"
#include "battery/core/string.hpp"
#include "battery/core/fs.hpp"
#include "battery/core/log.hpp"
#include "battery/core/time.hpp"
#include "battery/core/messages.hpp"
#include "battery/core/environment.hpp"

#ifdef BATTERY_ARCH_WINDOWS
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/file.h>
#endif

namespace b {

    lockfile::lockfile(const b::fs::path& filename) : filename(filename) {
        b::fs::create_directories(filename.parent_path());
        // In Windows, we create the file as non-exclusive and then lock it separately using LockFile(),
        // because this is apparently more robust than just opening the file in exclusive write mode.
#ifdef BATTERY_ARCH_WINDOWS
        this->fileHandle = CreateFileW(
                b::to_osstring(filename.to_string()).c_str(),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                nullptr,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                nullptr);
        if (this->fileHandle == INVALID_HANDLE_VALUE) {
            throw std::runtime_error(fmt::format("Failed to create lockfile '{}': Failed to open file for writing: {}", filename, b::get_last_win32_error()));
        }
#else
#endif
    }

    lockfile::~lockfile() {
        unlock();
        CloseHandle(this->fileHandle);
    }

    bool lockfile::lock(bool return_instead_of_throw) {
        if (!return_instead_of_throw) {
            if (timeout.has_value()) lock_polling();
            else lock_blocking();
        }
        else {
            try {
                if (timeout.has_value()) lock_polling();
                else lock_blocking();
            } catch (...) {
                return false;
            }
        }
        locked = true;
        return true;
    }

    bool lockfile::try_lock(bool return_instead_of_throw) {
        if (!return_instead_of_throw) {
            lock_immediate();
        }
        else {
            try {
                lock_immediate();
            } catch (...) {
                return false;
            }
        }
        locked = true;
        return true;
    }

    void lockfile::unlock() {
#ifdef BATTERY_ARCH_WINDOWS
        OVERLAPPED overlapped = {0};
        UnlockFileEx(fileHandle, 0, 0xFFFFFFFF, 0xFFFFFFFF, &overlapped);
#else
        flock((size_t)fileDescriptor, LOCK_UN);
		close((size_t)fileDescriptor);
        #error "Not implemented"
#endif
        locked = false;
    }

    bool lockfile::is_locked() const {
        return locked;
    }

    lockfile::operator bool() const {
        return is_locked();
    }

    void lockfile::lock_blocking() {
#ifdef BATTERY_ARCH_WINDOWS
        OVERLAPPED overlapped = { 0 };
        overlapped.Offset = 0;
        overlapped.OffsetHigh = 0;
        DWORD flags = LOCKFILE_EXCLUSIVE_LOCK;
        if (!LockFileEx(this->fileHandle, flags, 0, 0xFFFFFFFF, 0xFFFFFFFF, &overlapped)) {
            throw std::runtime_error(fmt::format("Failed to aquire lockfile '{}': Lock already in use", filename));
        }
#else
        fileDescriptor = open(file.c_str(), O_CREAT | O_RDWR, 0666);
        if (fileDescriptor <= 0) {
            throw std::runtime_error(fmt::format("Failed to aquire lockfile '{}': No such file or directory", filename));
        }
        if (flock(fileDescriptor, LOCK_EX) != 0) {
            flock((size_t)fileDescriptor, LOCK_UN);
            close((size_t)fileDescriptor);
            throw std::runtime_error(fmt::format("Failed to aquire lockfile '{}': Lock already in use", filename));
        }
#endif
    }

    void lockfile::lock_immediate() {
#ifdef BATTERY_ARCH_WINDOWS
        OVERLAPPED overlapped = { 0 };
        overlapped.Offset = 0;
        overlapped.OffsetHigh = 0;
        DWORD flags = LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY;
        if (!LockFileEx(this->fileHandle, flags, 0, 0xFFFFFFFF, 0xFFFFFFFF, &overlapped)) {
            throw std::runtime_error(fmt::format("Failed to aquire lockfile '{}': Lock already in use", filename));
        }
#else
        fileDescriptor = open(file.c_str(), O_CREAT | O_RDWR, 0666);
		if (fileDescriptor <= 0) {
            throw std::runtime_error(fmt::format("Failed to aquire lockfile '{}': No such file or directory", filename));
		}
		if (flock(fileDescriptor, LOCK_EX | LOCK_NB) != 0) {
            flock((size_t)fileDescriptor, LOCK_UN);
            close((size_t)fileDescriptor);
            throw std::runtime_error(fmt::format("Failed to aquire lockfile '{}': Lockfile unavailable", filename));
		}
#endif
    }

    void lockfile::lock_polling() {
        auto start = b::time();
        while (b::time() < start + timeout.value()) {
            try {
                try_lock();
                return;
            } catch (...) {}
            b::sleep(poll_interval);
        }
        throw std::runtime_error(fmt::format("Failed to aquire lockfile '{}': Timeout expired", filename));
    }



    scoped_lockfile::scoped_lockfile(const b::fs::path& filename, bool blocking, std::optional<double> timeout)
            : lockfile(filename), blocking(blocking) {
        lockfile.timeout = timeout;
        if (blocking) {
            lockfile.lock();
        }
        else {
            lockfile.try_lock();
        }
    }

    scoped_lockfile_nothrow::scoped_lockfile_nothrow(const b::fs::path& filename, bool blocking, std::optional<double> timeout)
            : lockfile(filename), blocking(blocking) {
        lockfile.timeout = timeout;
        if (blocking) {
            lockfile.lock(true);
        }
        else {
            lockfile.try_lock(true);
        }
    }

}
