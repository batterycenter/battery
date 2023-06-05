#pragma once

#include "battery/core/fs.hpp"
#include "battery/core/thread.hpp"

namespace b {

    class BasicFileWatcher {
    public:
        BasicFileWatcher() = default;
        explicit BasicFileWatcher(const b::fs::path& filePath);

        bool update();

    private:
        b::fs::path m_filePath {};
        std::chrono::time_point<std::filesystem::_File_time_clock> m_lastModificationTime {};
    };

    class FileWatcher : BasicFileWatcher {
    public:
        FileWatcher() = default;
        FileWatcher(const b::fs::path& file_path, std::function<void()> callback, float pollInterval = 0.1f);

        FileWatcher(const FileWatcher&) = delete;
        FileWatcher& operator=(const FileWatcher&) = delete;
        FileWatcher(FileWatcher&& other) = delete;
        FileWatcher& operator=(FileWatcher&& other) = delete;

    private:
        void watch(std::stop_token& stop_token);

    private:
        std::function<void()> m_callback;
        float m_pollInterval {};
        b::thread m_thread {};
    };

}