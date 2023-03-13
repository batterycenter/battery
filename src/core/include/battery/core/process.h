#pragma once

#include "battery/core/fs.h"
#include "battery/core/detachable_thread.h"

#include "reproc++/reproc.hpp"

namespace reproc {
    class process;
}

namespace battery {

    class process {
    public:

        struct options_t {
            std::string executable;
            std::vector<std::string> arguments;
            std::optional<fs::path> working_directory;
            bool silent = false;
            bool passthrough_to_parent = false;
            bool suppress_carriage_return = true;
            bool strip_trailing_whitespace_after_join = true;
            uint64_t wait_ms_before_every_stdin_write = 100;
            reproc::options reproc_options;
        };

        options_t options;
        std::function<void(const std::string& str)> stdout_callback;
        std::function<void(const std::string& str)> stderr_callback;

        int exit_code = 0;
        std::string error_message;
        std::string output_stdout;
        std::string output_stderr;
        std::string output_combined;

        process() = default;

        void execute_sync();
        void execute_async();
        void join();

        void stdin_write(const std::string_view& str);
        void stdin_write(const char* str);
        void stdin_write(const char* buffer, size_t length);

    private:
        static std::string remove_trailing_whitespace(std::string buffer);
        std::error_code stdout_sink(const uint8_t* _buffer, size_t length);
        std::error_code stderr_sink(const uint8_t* _buffer, size_t length);
        void run_process();

        battery::detachable_thread worker;
        reproc::process _process;
    };

}