#pragma once

#include "fs.hpp"
#include "async_worker.hpp"

#include "reproc++/reproc.hpp"

// TODO b::execute should have a way to directly retrieve the output of the process -> Overload 'capture_output' parameter

namespace b {

    class process {
    public:

        struct options_t {
            std::string executable;
            std::vector<std::string> arguments;
            std::optional<fs::path> working_directory;
            bool execute_as_shell_command = false;
            bool silent = false;
            bool passthrough_to_parent = false;
            bool suppress_carriage_return = true;
            bool strip_trailing_whitespace_after_join = true;
            std::optional<bool> forward_ctrl_c = std::nullopt;  // Defaults to the value of passthrough_to_parent
            double wait_ms_before_every_stdin_write = 100.0;
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
        bool was_terminated = false;
        std::string error_message;
        std::string output_stdout;
        std::string output_stderr;
        std::string output_combined;

        process() = default;

        process(const process& other);
        process& operator=(const process& other);
        process(process&& other) = default;
        process& operator=(process&& other) = default;

        void execute_sync();
        void execute_async();
        void join();

        void stdin_write(const std::string& str);
        void stdin_write(const std::string_view& str);
        void stdin_write(const char* str);
        void stdin_write(const char* buffer, size_t length);

        void kill();
        void terminate();

    private:
        static std::string remove_trailing_whitespace(std::string buffer);
        std::error_code stdout_sink(const std::string& data);
        std::error_code stderr_sink(const std::string& data);
        void run_process();

        b::async_worker<void> worker;
        reproc::process _process;
    };

    process execute(const std::string& command, const process::options_t& options = process::options_t());

}