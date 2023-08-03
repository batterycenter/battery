#pragma once

#include "battery/core/fs.hpp"
#include "battery/core/async_worker.hpp"

#include "reproc++/reproc.hpp"

// TODO b::execute should have a way to directly retrieve the output of the process -> Overload 'capture_output' parameter

namespace b {

    class process {
    public:

        struct options_t {
            b::string executable;
            std::vector<b::string> arguments;
            std::optional<fs::path> working_directory;
            bool execute_as_shell_command = false;
            bool silent = false;
            bool passthrough_to_parent = false;
            bool suppress_carriage_return = true;
            bool strip_trailing_whitespace_after_join = true;
            std::optional<bool> forward_ctrl_c = std::nullopt;  // Defaults to the value of passthrough_to_parent
            double wait_ms_before_every_stdin_write = 100.0;
            std::function<void(const b::string& str)> stdout_callback;
            std::function<void(const b::string& str)> stderr_callback;
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
        b::string error_message;
        b::string output_stdout;
        b::string output_stderr;
        b::string output_combined;

        process() = default;

        process(const process& other);
        process& operator=(const process& other);
        process(process&& other) = default;
        process& operator=(process&& other) = default;

        void execute_sync();
        void execute_async();
        void join();

        void stdin_write(const b::string& str);
        void stdin_write(const std::string_view& str);
        void stdin_write(const char* str);
        void stdin_write(const char* buffer, size_t length);

        void kill();
        void terminate();

    private:
        static b::string remove_trailing_whitespace(b::string buffer);
        std::error_code stdout_sink(const b::string& data);
        std::error_code stderr_sink(const b::string& data);
        void run_process();

        b::async_worker<void> worker;
        reproc::process _process;
    };

    process execute(const b::string& command, const process::options_t& options = process::options_t());

}