
#include "battery/core/file_watcher.hpp"

namespace b {

    basic_file_watcher::basic_file_watcher(const b::fs::path &file_path) : file_path(file_path) {}

    bool basic_file_watcher::update() {
        if (file_path.empty()) return false;
        if (!b::fs::exists(file_path)) return false;

        auto current_modification_time = b::fs::last_write_time(file_path);
        if (current_modification_time != last_modification_time) {
            last_modification_time = current_modification_time;
            return true;
        }
        return false;
    }

    file_watcher::file_watcher(const b::fs::path &file_path, const std::function<void()> &callback, float poll_interval)
        : basic_file_watcher(file_path), callback(callback), poll_interval(poll_interval) {
        thread = b::thread([this](std::stop_token stop_token) { this->watch(stop_token); });
    }

    void file_watcher::watch(std::stop_token& stop_token) {
        while (!stop_token.stop_requested()) {
            if (this->update()) {
                this->callback();
            }
            b::sleep(poll_interval);
        }
    }

}