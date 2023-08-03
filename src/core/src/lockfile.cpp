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

#include "battery/core/lockfile.hpp"
#include "battery/core/string.hpp"
#include "battery/core/fs.hpp"
#include "battery/core/time.hpp"
#include "battery/core/messages.hpp"

#include "battery/core/log.hpp"

#ifdef B_OS_WINDOWS
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/file.h>
#endif

namespace b {

    lockfile::lockfile(const b::fs::path& filename) : filename(filename), mutex(std::make_unique<std::mutex>()) {
        if (filename.has_parent_path()) {
            b::fs::create_directory(filename.parent_path());
        }
        // In Windows, we create the file as non-exclusive and then lock it separately using LockFile(),
        // because this is apparently more robust than just opening the file in exclusive write mode.
#ifdef B_OS_WINDOWS
        auto nativeFilename = filename.string().to_native();
        this->fileHandle = CreateFileW(
                nativeFilename.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                nullptr,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                nullptr);
        if (this->fileHandle == INVALID_HANDLE_VALUE) {
            throw std::runtime_error(b::format("Failed to create lockfile '{}': Failed to open file for writing: {}", filename, b::internal::get_last_win32_error()));
        }
#else
        this->fileHandle = reinterpret_cast<void *>(open(filename.c_str(), O_CREAT | O_RDWR, 0666));
        if (reinterpret_cast<int64_t>(this->fileHandle) <= -1) {
            throw std::runtime_error(b::format("Failed to create lockfile '{}': Failed to open file for writing: {}", filename, strerror(errno)));
        }
#endif
    }

    lockfile::~lockfile() {
        unlock();
#ifdef B_OS_WINDOWS
        CloseHandle(this->fileHandle);
#else
        close((size_t)this->fileHandle);
#endif
    }

    bool lockfile::lock(bool return_instead_of_throw) {
        if (!return_instead_of_throw) {
            if (timeout.has_value()) lock_polling();
            else lock_impl(true);
        }
        else {
            try {
                if (timeout.has_value()) lock_polling();
                else lock_impl(true);
            } catch (...) {
                return false;
            }
        }
        return true;
    }

    bool lockfile::try_lock(bool return_instead_of_throw) {
        if (!return_instead_of_throw) {
            lock_impl(false);
        }
        else {
            try {
                lock_impl(false);
            } catch (...) {
                return false;
            }
        }
        return true;
    }

    void lockfile::unlock() {
        std::scoped_lock lock(*this->mutex.get());
        if (!this->locked) {
            return;
        }
#ifdef B_OS_WINDOWS
        OVERLAPPED overlapped = {0};
        UnlockFileEx(fileHandle, 0, 0xFFFFFFFF, 0xFFFFFFFF, &overlapped);
#else
        flock(static_cast<int>((size_t)this->fileHandle), LOCK_UN);
#endif
        locked = false;
    }

    bool lockfile::is_locked() const {
        return locked;
    }

    lockfile::operator bool() const {
        return is_locked();
    }

    void lockfile::lock_impl(bool blocking) {
        std::scoped_lock lock(*this->mutex.get());
        if (this->locked) {
            throw std::runtime_error(b::format("Failed to aquire lockfile '{}': Lock already in use", filename));
        }
#ifdef B_OS_WINDOWS
        OVERLAPPED overlapped = { 0 };
        overlapped.Offset = 0;
        overlapped.OffsetHigh = 0;
        DWORD flags = blocking ? (LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY) : LOCKFILE_EXCLUSIVE_LOCK;
        if (!LockFileEx(this->fileHandle, flags, 0, 0xFFFFFFFF, 0xFFFFFFFF, &overlapped)) {
            throw std::runtime_error(b::format("Failed to aquire lockfile '{}': Lock already in use", filename));
        }
#else
        struct flock fl = {};
        fl.l_type = F_WRLCK;    // Write lock
        fl.l_whence = SEEK_SET; // Start at beginning of file
        fl.l_start = 0;         // Lock entire file
        fl.l_len = 0;

        // Attempt to acquire the lock using F_SETLKW
        int ret = fcntl(static_cast<int>((size_t)this->fileHandle), blocking ? F_SETLKW : F_SETLK, &fl);
        if (ret == -1) {
            throw std::runtime_error(b::format("Failed to aquire lockfile '{}': Lock already in use", filename));
        }
        this->locked = true;
#endif
    }

    void lockfile::lock_polling() {
        auto start = b::time();
        while (b::time() < start + timeout.value()) {
            try {
                if (try_lock()) {
                    return;
                }
            } catch (...) {
                // No action
            }
            b::sleep(poll_interval);
        }
        throw std::runtime_error(b::format("Failed to aquire lockfile '{}': Timeout expired", filename));
    }



    scoped_lockfile::scoped_lockfile(const b::fs::path& filename, lockfile_mode mode, std::optional<double> timeout)
            : _lockfile(filename) {
        _lockfile.timeout = timeout;
        if (mode == lockfile_mode::blocking) {
            _lockfile.lock();
        }
        else {
            _lockfile.try_lock();
        }
    }

    scoped_lockfile_nothrow::scoped_lockfile_nothrow(const b::fs::path& filename, lockfile_mode mode, std::optional<double> timeout)
            : _lockfile(filename) {
        _lockfile.timeout = timeout;
        if (mode == lockfile_mode::blocking) {
            _lockfile.lock(true);
        }
        else {
            _lockfile.try_lock(true);
        }
    }

}
