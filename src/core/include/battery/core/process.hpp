#pragma once

#include "battery/core/fs.hpp"
#include "battery/core/async_worker.hpp"

#include "reproc++/reproc.hpp"

namespace b {

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
            std::function<void(const std::string& str)> stdout_callback;
            std::function<void(const std::string& str)> stderr_callback;
            reproc::options reproc_options;
            options_t() = default;
            options_t(const options_t& other) {
                this->operator=(other);
            }
            options_t& operator=(const options_t& other) {
                this->executable = other.executable;
                this->arguments = other.arguments;
                this->working_directory = other.working_directory;
                this->silent = other.silent;
                this->passthrough_to_parent = other.passthrough_to_parent;
                this->suppress_carriage_return = other.suppress_carriage_return;
                this->strip_trailing_whitespace_after_join = other.strip_trailing_whitespace_after_join;
                this->wait_ms_before_every_stdin_write = other.wait_ms_before_every_stdin_write;
                this->stdout_callback = other.stdout_callback;
                this->stderr_callback = other.stderr_callback;
                this->reproc_options = reproc::options::clone(other.reproc_options);
                return *this;
            }
        };

        options_t options;

        int exit_code = 0;
        std::string error_message;
        std::string output_stdout;
        std::string output_stderr;
        std::string output_combined;

        process();
        ~process();

        process(const process& other);
        process& operator=(const process& other);
        process(process&& other) = default;
        process& operator=(process&& other) = default;

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

        b::async_worker<void> worker;
        reproc::process _process;
    };

    process execute(const std::string& command, process::options_t options = process::options_t());

}