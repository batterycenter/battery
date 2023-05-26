
#include "battery/core/process.hpp"
#include "battery/core/time.hpp"
#include "battery/core/signal.hpp"

#include "reproc++/drain.hpp"

#include <iostream>

namespace b {

    process::process(const process& other) :
         options(other.options),
         exit_code(other.exit_code),
         error_message(other.error_message),
         output_stdout(other.output_stdout),
         output_stderr(other.output_stderr),
         output_combined(other.output_combined) {}

    process& process::operator=(const process& other) {
        this->options = other.options;
        this->exit_code = other.exit_code;
        this->error_message = other.error_message;
        this->output_stdout = other.output_stdout;
        this->output_stderr = other.output_stderr;
        this->output_combined = other.output_combined;
        return *this;
    }

    void process::execute_sync() {
        worker.execute_sync([this] { process::run_process(); });
    }

    void process::execute_async() {
        worker.execute_async([this] { process::run_process(); });
    }

    void process::join() {
        worker.join();
    }

    b::string process::remove_trailing_whitespace(b::string buffer) {
        if (buffer.empty()) return buffer;
        for (int64_t i = static_cast<int>(buffer.length() - 1); i >= 0; i--) {
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

    void process::stdin_write(const b::string& str) {
        stdin_write(str.data(), str.length());
    }

    void process::stdin_write(const std::string_view& str) {
        stdin_write(str.data(), str.length());
    }

    void process::stdin_write(const char* str) {
        stdin_write(str, std::strlen(str));
    }

    void process::stdin_write(const char* buffer, size_t length) {
        b::sleep_ms(options.wait_ms_before_every_stdin_write);
        _process.write(std::bit_cast<const uint8_t *>(buffer), length);
    }

    void process::kill() {
        _process.kill();
    }

    void process::terminate() {
        _process.terminate();
    }

    std::error_code process::stdout_sink(const uint8_t* _buffer, size_t length) {
        if (length == 0) return {};
        auto buffer = b::string(std::string(std::bit_cast<const char*>(_buffer), length));
        if (options.suppress_carriage_return) {
            buffer = b::string::replace(buffer, "\r", "");
        }
        output_stdout += buffer;
        output_combined += buffer;
        if (options.stdout_callback) { options.stdout_callback(buffer); }
        if (!options.silent) { b::print("{}", buffer); }
        return {};
    }

    std::error_code process::stderr_sink(const uint8_t* _buffer, size_t length) {
        if (length == 0) return {};
        auto buffer = b::string(std::string(std::bit_cast<const char*>(_buffer), length));
        if (options.suppress_carriage_return) {
            buffer = b::string::replace(buffer, "\r", "");
        }
        output_stderr += buffer;
        output_combined += buffer;
        if (options.stderr_callback) { options.stderr_callback(buffer); }
        if (!options.silent) { b::print("{}", buffer); }
        return {};
    }

    void process::run_process() {
        std::vector<const char*> cmd_cstr;

#ifdef BATTERY_ARCH_WINDOWS
        if (options.execute_as_shell_command) {
            cmd_cstr.emplace_back("cmd.exe");
            cmd_cstr.emplace_back("/c");
        }
#else
        if (options.execute_as_shell_command) {
            cmd_cstr.emplace_back("bash");
            cmd_cstr.emplace_back("-c");
        }
#endif

        if (!options.executable.empty()) {
            cmd_cstr.emplace_back(options.executable.c_str());
        }
        for (const auto& c : options.arguments) {
            if (c.empty()) continue;
            cmd_cstr.emplace_back(c.c_str());
        }
        cmd_cstr.emplace_back(nullptr);

        options.reproc_options.redirect.parent = options.passthrough_to_parent;
        b::string workdir = options.working_directory.has_value() ? options.working_directory->string() : "";
        options.reproc_options.working_directory = !workdir.empty() ? workdir.c_str() : nullptr;
        options.reproc_options.redirect.err.type = reproc::redirect::type::pipe;

        if (options.working_directory.has_value()) {
            auto dir = options.working_directory.value();
            if (!fs::is_directory(dir)) {
                throw std::invalid_argument(b::format("Cannot run process in working directory '{}': Directory does not exist", workdir));
            }
        }

        bool ctrl_c_handler = options.passthrough_to_parent;
        if (options.forward_ctrl_c.has_value()) {
            ctrl_c_handler = options.forward_ctrl_c.value();
        }

        if (ctrl_c_handler) {
            b::push_ctrl_c_handler([this]() {
                this->was_terminated = true;
                this->terminate();
            });
        }

        std::error_code ec = _process.start(cmd_cstr.data(), options.reproc_options);
        if (ec) {
            exit_code = ec.value();
            error_message = ec.message();
            if (ctrl_c_handler) b::pop_ctrl_c_handler();
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
            if (ctrl_c_handler) b::pop_ctrl_c_handler();
            return;
        }

        auto [status, _ec] = _process.wait(reproc::infinite);
        if (_ec) {
            exit_code = status;
            error_message = _ec.message();
            if (ctrl_c_handler) b::pop_ctrl_c_handler();
            return;
        }

        if (options.strip_trailing_whitespace_after_join) {
            output_stdout = remove_trailing_whitespace(output_stdout);
            output_stderr = remove_trailing_whitespace(output_stderr);
            output_combined = remove_trailing_whitespace(output_combined);
        }

        exit_code = status;
        error_message = _ec.message();
        if (ctrl_c_handler) b::pop_ctrl_c_handler();
    }

    process execute(const b::string& command, const process::options_t& options) {
        b::process process;
        process.options = options;
        process.options.passthrough_to_parent = true;
        process.options.execute_as_shell_command = true;
        process.options.arguments = { command };
        process.execute_sync();
        return process;
    }

}
