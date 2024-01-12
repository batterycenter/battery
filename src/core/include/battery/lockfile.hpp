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

#pragma once

#ifndef B_OS_WEB

#include <string>
#include "fs.hpp"

///
/// \brief Lockfile utilities
/// \details Lockfiles are a way to synchronize processes by the use of atomic filesystem operations.
///          The most common use case is to create a lockfile and prevent other processes from doing file system
///          operations in a given directory. Because battery's lockfiles are based on operating system locks, rather than
///          primitive file operations like create and delete, they are never undefined even if the process crashes.
///
///          A crashing process also unlocks the lockfile, which prevents infinite blocking.
///
///          As a result, these lockfiles only work on a local filesystem, they cannot be used on network drives.
///          In this case, a soft-lockfile must be used, which does the locking by creating and deleting a file, instead
///          of locking it on the OS level. The disadvantage is that the file still exists after a crash, resulting in
///          possible infinite blocking.
///
///          If you are in need of such a soft-lockfile, please file a feature request on GitHub.
///
///          Example for a blocking lockfile:
///          \code{.cpp}
///          void critical() {
///              b::scoped_lockfile lockfile(b::folders::get_cache() + "file.lock");
///              b::print("File is locked, performing critical operations...\n");
///          }
///          \endcode
///          If the example above is run from multiple processes at a time, only one will get the lock and the others
///          wait until the lock is released. One process after the other can do critical operations.
///
///          Example for a non-blocking lockfile:
///          \code{.cpp}
///          void critical() {
///              try {
///                  b::scoped_lockfile lockfile(b::folders::get_cache() + "file.lock", b::lockfile_mode::non_blocking);
///                  b::print("File is locked, performing critical operations...\n");
///              }
///              catch (...) {
///                  b::print("ERROR: Failed to acquire lock, it is already in use by another process\n");
///              }
///          }
///          \endcode
///          One instance of the program will get the lock and the others will throw an exception.
///
///          Example for a non-blocking lockfile that does not throw:
///          \code{.cpp}
///          void critical() {
///              b::scoped_lockfile_nothrow lockfile(b::folders::get_cache() + "file.lock", b::lockfile_mode::non_blocking);
///              if (lockfile) {
///                  b::print("File is locked, performing critical operations...\n");
///              }
///              else {
///                  b::print("ERROR: Failed to acquire lock, it is already in use by another process\n");
///              }
///          }
///          \endcode
///
///          Example for a blocking lockfile with a timeout:
///          \code{.cpp}
///          void critical() {
///              try {
///                  b::scoped_lockfile lockfile(b::folders::get_cache() + "file.lock", b::lockfile_mode::blocking, 5.0);
///                  b::print("File is locked, performing critical operations...\n");
///              }
///              catch (...) {
///                  b::print("ERROR: Failed to acquire lock, timeout expired\n");
///              }
///          }
///          \endcode
///          This example will wait for up to 5 seconds for the lock to become available, after that it throws.
///
/// \addtogroup Lockfile
/// @{
///
namespace b {

    ///
    /// \brief A lockfile class that can be used to synchronize processes by the use of atomic filesystem operations.
    /// \details The most common use case is to create a lockfile and prevent other processes from doing file system
    ///          operations in a given directory. The locking process must unlock it or crash, to allow other processes
    ///          to lock it and thereby signalize exclusive access. It is more or less like a mutex,
    ///          but instead of locking across threads, it also locks across processes.
    ///          The lockfile and any parent directories are automatically created, but they are not deleted afterwards.
    ///          It is automatically unlocked on destruction.
    ///
    ///          This class contains the base functionality with explicit calls to lock(), try_lock() and unlock().
    ///          See the `b::scoped_lockfile` class for a more convenient way to use this class.
    /// \warning When the lock is in blocking mode and a timeout is set, the lock is polled in a default interval of 10ms.
    ///          This might lead to slighly higher CPU and filesystem usage. When no timeout is specified,
    ///          the operating system is taking care of the waiting (thus practically no CPU or filesystem usage)
    /// \headerfile battery/core/lockfile.hpp
    /// \see b::scoped_lockfile
    ///
    class lockfile {
    public:

        ///
        /// \brief Constructor
        /// \param[in] filename The path at which the lockfile should be created
        ///
        lockfile(const b::fs::path& filename);

        ///
        /// \brief b::lockfile destructor
        ///
        ~lockfile();

        ///
        /// \brief The timeout in seconds for blocking operations [default: no timeout]
        ///
        std::optional<double> timeout = std::nullopt;

        ///
        /// \brief The polling interval in seconds for blocking operations [default: 0.01 (10ms)]
        ///
        double poll_interval = 0.01;

        ///
        /// \brief Lock the lockfile in blocking mode
        /// \param[in] return_instead_of_throw Set to true to return false instead of throwing an exception [default: false]
        /// \return True if the lock was acquired, false otherwise, given that `return_instead_of_throw=true`
        /// \throw std::runtime_error if the lockfile could not be acquired and `return_instead_of_throw=false`
        ///
        bool lock(bool return_instead_of_throw = false);

        ///
        /// \brief Lock the lockfile and fail immediately if the lock is already occupied
        /// \param[in] return_instead_of_throw Set to true to return false instead of throwing an exception [default: false]
        /// \return True if the lock was acquired, false otherwise, given that `return_instead_of_throw=true`
        /// \throw std::runtime_error if the lockfile could not be acquired and `return_instead_of_throw=false`
        ///
        bool try_lock(bool return_instead_of_throw = false);

        ///
        /// \brief Unlock the lockfile
        ///
        void unlock();

        ///
        /// \brief Check if the lockfile is locked
        /// \return True if the lockfile is locked, false otherwise
        ///
        bool is_locked() const;

        ///
        /// \brief Check if the lockfile is locked
        /// \return True if the lockfile is locked, false otherwise
        ///
        operator bool() const;

    private:
        void lock_impl(bool blocking);
        void lock_polling();

        const b::fs::path filename;
        void* fileHandle = nullptr;
        std::unique_ptr<std::mutex> mutex;
        bool locked = false;
    };

    ///
    /// \brief Lockfile mode for `b::scoped_lockfile` and `b::scoped_lockfile_nothrow`
    /// \see b::scoped_lockfile
    /// \see b::scoped_lockfile_nothrow
    ///
    enum class lockfile_mode {
        blocking,       ///< Blocking mode, will wait until the lock is released
        non_blocking    ///< Non-blocking mode, will fail immediately if the lock is already occupied
    };

    ///
    /// \brief RAII convenience wrapper for `b::lockfile`
    /// \details This wrapper implements `b::lockfile` and automatically locks on construction, and unlocks on destruction.
    ///          It throws an exception on construction if the lockfile cannot be acquired. Most common use is if you want
    ///          to do operations in a filesystem folder and prevent doing other instances of the same program from doing
    ///          the same thing. See `b::scoped_lockfile_nothrow` for a version that does not throw,
    ///          but instead can be queried if the lock was acquired successful.
    ///          See `b::lockfile` for more information on how it works.
    /// \warning When the lock is in blocking mode and a timeout is set, the lock is polled in a fixed interval of 10ms.
    ///          This might lead to slighly higher CPU and filesystem usage. When no timeout is specified,
    ///          the operating system is taking care of the waiting (thus practically no CPU or filesystem usage)
    /// \see b::lockfile
    /// \see b::scoped_lockfile_nothrow
    ///
    class scoped_lockfile {
    public:
        ///
        /// \brief Constructor
        /// \param[in] filename The path at which the lockfile should be created
        /// \param[in] blocking If the lock should wait until it gets available, or fail immediately [default: true]
        /// \param[in] timeout If `blocking` is true, this is the maximum time in seconds to wait for the lock to become available [default: no timeout]
        ///
        scoped_lockfile(const b::fs::path& filename, lockfile_mode mode = lockfile_mode::blocking, std::optional<double> timeout = std::nullopt);

    private:
        b::lockfile _lockfile;
    };

    ///
    /// \brief `b::scoped_lockfile`, but it returns false instead of throwing
    /// \details This wrapper is equivalent to `b::scoped_lockfile`, the only difference is that it does not throw
    ///          if the lockfile cannot be acquired. Instead, the boolean operator can be used to check if the lockfile
    ///          was acquired successfully. See `b::scoped_lockfile` for more information.
    /// \warning When the lock is in blocking mode and a timeout is set, the lock is polled in a fixed interval of 10ms.
    ///          This might lead to slighly higher CPU and filesystem usage. When no timeout is specified,
    ///          the operating system is taking care of the waiting (thus practically no CPU or filesystem usage)
    /// \see b::lockfile
    /// \see b::scoped_lockfile
    ///
    class scoped_lockfile_nothrow {
    public:
        ///
        /// \brief Constructor
        /// \param[in] filename The path at which the lockfile should be created
        /// \param[in] blocking If the lock should wait until it gets available, or fail immediately [default: true]
        /// \param[in] timeout If `blocking` is true, this is the maximum time in seconds to wait for the lock to be become available [default: no timeout]
        ///
        scoped_lockfile_nothrow(const b::fs::path& filename, lockfile_mode mode = lockfile_mode::blocking, std::optional<double> timeout = std::nullopt);

        ///
        /// \brief Check if the lock has been acquired successfully
        /// \return `true` if the lock has been acquired successfully, `false` otherwise
        ///
        operator bool() {
            return _lockfile;
        }

    private:
        b::lockfile _lockfile;
    };

} // !B_OS_WEB

#endif // !B_OS_WEB
