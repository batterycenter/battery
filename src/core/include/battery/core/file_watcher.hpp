#pragma once

#include "battery/core/fs.hpp"
#include "battery/core/thread.hpp"

namespace b {

    class basic_file_watcher {
    public:
        basic_file_watcher() = default;
        explicit basic_file_watcher(const b::fs::path& file_path);

        bool update();

    private:
        b::fs::path file_path {};
        std::chrono::time_point<std::filesystem::_File_time_clock> last_modification_time {};
    };

    class file_watcher : basic_file_watcher {
    public:
        file_watcher() = default;
        file_watcher(const b::fs::path& file_path, std::function<void()> callback, float poll_interval = 0.1f);

        file_watcher(const file_watcher&) = delete;
        file_watcher& operator=(const file_watcher&) = delete;
        file_watcher(file_watcher&& other) = delete;
        file_watcher& operator=(file_watcher&& other) = delete;

    private:
        void watch(std::stop_token& stop_token);

    private:
        std::function<void()> callback;
        float poll_interval {};
        b::thread thread;
    };

}