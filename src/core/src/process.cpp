
#include "battery/core/process.h"
#include "battery/core/exception.h"
#include "battery/core/time.h"

#include "reproc++/drain.hpp"

#include <iostream>

namespace battery {

    void process::execute_sync() {
        worker.execute_sync_thread(&process::run_process, this);
    }

    void process::execute_async() {
        worker.execute_async_thread(&process::run_process, this);
    }

    void process::join() {
        worker.join();
    }

    std::string process::remove_trailing_whitespace(std::string buffer) {
        if (buffer.empty()) return buffer;
        for (int i = buffer.length() - 1; i >= 0; i--) {
            switch (buffer.back()) {
                case '\r':
                case '\n':
                case '\t':
                case ' ':
                    buffer.pop_back();
                    break;
                default:
                    return buffer;
            }
        }
        return buffer;
    }

    void process::stdin_write(const std::string_view& str) {
        stdin_write(str.data(), str.length());
    }

    void process::stdin_write(const char* str) {
        stdin_write(str, std::strlen(str));
    }

    void process::stdin_write(const char* buffer, size_t length) {
        battery::sleep_ms(options.wait_ms_before_every_stdin_write);
        _process.write(std::bit_cast<const uint8_t *>(buffer), length);
    }

    std::error_code process::stdout_sink(const uint8_t* _buffer, size_t length) {
        if (length == 0) return {};
        auto buffer = std::string(std::bit_cast<const char*>(_buffer), length);
        if (options.suppress_carriage_return) {
            buffer = battery::string::replace(buffer, "\r", "");
        }
        output_stdout += buffer;
        output_combined += buffer;
        if (stdout_callback) { stdout_callback(buffer); }
        if (!options.silent) { std::cout << buffer; }
        return {};
    }

    std::error_code process::stderr_sink(const uint8_t* _buffer, size_t length) {
        if (length == 0) return {};
        auto buffer = std::string(std::bit_cast<const char*>(_buffer), length);
        if (options.suppress_carriage_return) {
            buffer = battery::string::replace(buffer, "\r", "");
        }
        output_stderr += buffer;
        output_combined += buffer;
        if (stderr_callback) { stderr_callback(buffer); }
        if (!options.silent) { std::cout << buffer; }
        return {};
    }

    void process::run_process() {   // TODO !!!!! : Process stdout and stdin has no unicode support!!!

        // Convert commands from STL vector to raw C-style string pointers
        std::vector<const char*> cmd_cstr;

        cmd_cstr.emplace_back(options.executable.c_str());

        for (const auto& c : options.arguments) {
            cmd_cstr.emplace_back(c.c_str());
        }
        cmd_cstr.emplace_back(nullptr);

        options.reproc_options.redirect.parent = options.passthrough_to_parent;
        std::string workdir = options.working_directory.has_value() ? options.working_directory->to_string() : "";
        options.reproc_options.working_directory = !workdir.empty() ? workdir.c_str() : nullptr;

        if (options.working_directory.has_value()) {
            auto dir = options.working_directory.value();
            if (!fs::is_directory(dir)) {
                throw battery::exception(
                        std::string("Cannot run process in working directory ") + workdir +
                        ": Directory does not exist");
            }
        }

        std::error_code ec = _process.start(cmd_cstr.data(), options.reproc_options);
        if (ec) {
            exit_code = ec.value();
            error_message = ec.message();
            return;
        }

        ec = reproc::drain(
                _process,
                [this] (auto, const auto* buffer, auto length) {
                    return this->stdout_sink(buffer, length);
                },
                [this] (auto, const auto* buffer, auto length) {
                    return this->stderr_sink(buffer, length);
                });

        if (ec) {
            exit_code = ec.value();
            error_message = ec.message();
            return;
        }

        auto [status, _ec] = _process.wait(reproc::infinite);
        if (_ec) {
            exit_code = status;
            error_message = _ec.message();
            return;
        }

        if (options.strip_trailing_whitespace_after_join) {
            output_stdout = remove_trailing_whitespace(output_stdout);
            output_stderr = remove_trailing_whitespace(output_stderr);
            output_combined = remove_trailing_whitespace(output_combined);
        }

        exit_code = status;
        error_message = _ec.message();
    }

}
