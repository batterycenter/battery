#pragma once

#include <string>
#include "battery/core/fs.hpp"

namespace b {

    class lockfile {
    public:
        lockfile(const b::fs::path& filename);
        ~lockfile();

        // If set, try_lock() will wait for this many seconds, while polling the lock every 10ms
        std::optional<double> timeout = std::nullopt;
        double poll_interval = 0.01;

        bool lock(bool return_instead_of_throw = false);
        bool try_lock(bool return_instead_of_throw = false);
        void unlock();

        bool is_locked() const;
        operator bool() const;

    private:
        void lock_blocking();
        void lock_immediate();
        void lock_polling();

        const b::fs::path filename;
        void* fileHandle = nullptr;
        bool locked = false;
    };

    class scoped_lockfile {
    public:
        scoped_lockfile(const b::fs::path& filename, bool blocking = true, std::optional<double> timeout = std::nullopt);

    private:
        bool blocking = false;
        lockfile lockfile;
    };

    class scoped_lockfile_nothrow {
    public:
        scoped_lockfile_nothrow(const b::fs::path& filename, bool blocking = true, std::optional<double> timeout = std::nullopt);

        operator bool() {
            return lockfile;
        }

    private:
        bool blocking = false;
        lockfile lockfile;
    };

}