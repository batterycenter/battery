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

#include <string>
#include "battery/core/fs.hpp"

///
/// \brief Lockfile utilities
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
    /// \see b::scoped_lockfile
    ///
    class lockfile {
    public:

        ///
        /// \brief b::lockfile constructor
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
        /// \return True if the lock was acquired, and false otherwise, given that `return_instead_of_throw` is true
        /// \throw std::runtime_error if the lockfile could not be acquired and `return_instead_of_throw` is false
        ///
        bool lock(bool return_instead_of_throw = false);

        ///
        /// \brief Lock the lockfile and fail immediately if the lock is already occupied
        /// \param[in] return_instead_of_throw Set to true to return false instead of throwing an exception [default: false]
        /// \return True if the lock was acquired, and false otherwise, given that `return_instead_of_throw` is true
        /// \throw std::runtime_error if the lockfile could not be acquired and `return_instead_of_throw` is false
        ///
        bool try_lock(bool return_instead_of_throw = false);

        ///
        /// \brief Unlock the lockfile
        ///
        void unlock();

        ///
        /// \brief Check if the lockfile is locked
        /// \return True if the lockfile is locked, and false otherwise
        ///
        bool is_locked() const;

        ///
        /// \brief Check if the lockfile is locked
        /// \return True if the lockfile is locked, and false otherwise
        ///
        operator bool() const;

    private:
        void lock_blocking();
        void lock_immediate();
        void lock_polling();

        const b::fs::path filename;
        void* fileHandle = nullptr;
        bool locked = false;
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
        /// \brief b::scoped_lockfile constructor
        /// \param[in] filename The path at which the lockfile should be created
        /// \param[in] blocking If the lock should wait until it gets freed, or fail immediately [default: true]
        /// \param[in] timeout If `blocking` is true, this is the maximum time in seconds to wait for the lock to be freed [default: no timeout]
        ///
        scoped_lockfile(const b::fs::path& filename, bool blocking = true, std::optional<double> timeout = std::nullopt);

    private:
        bool blocking = false;
        lockfile lockfile;
    };

    ///
    /// \brief RAII convenience wrapper for `b::lockfile` that does not throw
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
        /// \brief b::scoped_lockfile_nothrow constructor
        /// \param[in] filename The path at which the lockfile should be created
        /// \param[in] blocking If the lock should wait until it gets freed, or fail immediately [default: true]
        /// \param[in] timeout If `blocking` is true, this is the maximum time in seconds to wait for the lock to be freed [default: no timeout]
        ///
        scoped_lockfile_nothrow(const b::fs::path& filename, bool blocking = true, std::optional<double> timeout = std::nullopt);

        ///
        /// \brief Check if the lock has been acquired successfully
        /// \return `true` if the lock has been acquired successfully, `false` otherwise
        ///
        operator bool() {
            return lockfile;
        }

    private:
        bool blocking = false;
        lockfile lockfile;
    };

}