
#include "battery/core/file_watcher.hpp"

namespace b {

    BasicFileWatcher::BasicFileWatcher(const b::fs::path &filePath) : m_filePath(filePath) {}

    bool BasicFileWatcher::update() {
        if (m_filePath.empty()) return false;
        if (!b::fs::exists(m_filePath)) return false;

        auto current_modification_time = b::fs::last_write_time(m_filePath);
        if (current_modification_time != m_lastModificationTime) {
            m_lastModificationTime = current_modification_time;
            return true;
        }
        return false;
    }

    FileWatcher::FileWatcher(const b::fs::path &file_path, std::function<void()> callback, double pollInterval)
        : BasicFileWatcher(file_path), m_callback(std::move(callback)), m_pollInterval(pollInterval) {
        m_thread = b::thread([this](std::stop_token stop_token) { this->watch(stop_token); });
    }

    void FileWatcher::watch(std::stop_token& stop_token) {
        while (!stop_token.stop_requested()) {
            if (this->update()) {
                this->m_callback();
            }
            b::sleep(m_pollInterval);
        }
    }

} // namespace b