
#include "battery/core/process.hpp"
#include "battery/core/signal.hpp"
#include "battery/core/time.hpp"

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
        if (buffer.empty()) {
            return buffer;
        }
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
        stdin_write(str.to_utf8().data(), str.length());
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

    std::error_code process::stdout_sink(const b::string& data) {
        auto buffer = data;
        if (data.empty()) {
            return {};
        }
        if (options.suppress_carriage_return) {
            buffer.replace("\r"_b, ""_b);
        }
        output_stdout += buffer;
        output_combined += buffer;
        if (options.stdout_callback) {
            options.stdout_callback(buffer);
        }
        if (!options.silent) {
            std::string s;
            b::print("{}", "");
        }
        return {};
    }

    std::error_code process::stderr_sink(const b::string& data) {
        auto buffer = data;
        if (data.empty()) {
            return {};
        }
        if (options.suppress_carriage_return) {
            buffer.replace("\r"_b, ""_b);
        }
        output_stderr += buffer;
        output_combined += buffer;
        if (options.stderr_callback) { options.stderr_callback(buffer); }
        if (!options.silent) { b::print("{}", buffer); }
        return {};
    }

    void process::run_process() {
        std::vector<std::string> cmd;   // Here all the strings are stored in UTF-8 format,
                                        // the const char* will point to here, later
        if (options.execute_as_shell_command) {
            if (b::Constants::Platform() == b::Platform::Windows) {
                cmd.emplace_back("cmd.exe");
                cmd.emplace_back("/c");
            } 
            else {
                cmd.emplace_back("bash");
                cmd.emplace_back("-c");
            }
        }

        if (!options.executable.empty()) {
            cmd.emplace_back(options.executable.to_utf8());
        }
        for (const auto& command : options.arguments) {
            if (command.empty()) {
                continue;
            }
            cmd.emplace_back(command.to_utf8());
        }

        options.reproc_options.redirect.parent = options.passthrough_to_parent;
        std::string workdir = options.working_directory.has_value() ? options.working_directory->string().to_utf8() : "";
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

        std::vector<const char*> cmdCstr;
        for (const auto& str : cmd) {
            cmdCstr.emplace_back(str.c_str());
        }
        cmdCstr.emplace_back(nullptr);

        std::error_code errorCode = _process.start(cmdCstr.data(), options.reproc_options);
        if (errorCode) {
            exit_code = errorCode.value();
            error_message = b::string::from_utf8(errorCode.message());
            if (ctrl_c_handler) {
                b::pop_ctrl_c_handler();
            }
            return;
        }

        errorCode = reproc::drain(
                _process,
                [this] (auto, const uint8_t* buffer, size_t length) {
                    return this->stdout_sink(b::string::from_utf8(std::string(std::bit_cast<char*>(buffer), length)));
                },
                [this] (auto, const uint8_t* buffer, size_t length) {
                    return this->stderr_sink(b::string::from_utf8(std::string(std::bit_cast<char*>(buffer), length)));
                });

        if (errorCode) {
            exit_code = errorCode.value();
            error_message = b::string::from_utf8(errorCode.message());
            if (ctrl_c_handler) {
                b::pop_ctrl_c_handler();
            }
            return;
        }

        auto [status, _ec] = _process.wait(reproc::infinite);
        if (_ec) {
            exit_code = status;
            error_message = b::string::from_utf8(_ec.message());
            if (ctrl_c_handler) {
                b::pop_ctrl_c_handler();
            }
            return;
        }

        if (options.strip_trailing_whitespace_after_join) {
            output_stdout = remove_trailing_whitespace(output_stdout);
            output_stderr = remove_trailing_whitespace(output_stderr);
            output_combined = remove_trailing_whitespace(output_combined);
        }

        exit_code = status;
        error_message = b::string::from_utf8(_ec.message());
        if (ctrl_c_handler) {
            b::pop_ctrl_c_handler();
        }
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

} // namespace b