
#include "battery/core/environment.h"
#include "battery/core/process.h"
#include "battery/core/exception.h"
#include "battery/core/log.h"

#include "reproc++/reproc.hpp"
#include "reproc++/drain.hpp"

namespace battery {

    process::process() {

    }

    process::~process() {
        if (thread.joinable()) {
            log::core::error("battery::~process(): The process is still running. Detaching the thread now. You must join the process explicitly");
            thread.detach();
        }
    }

    bool process::execute() {
        std::unique_lock<std::mutex> lock(func_mutex);

        if (thread.joinable())
            throw battery::exception("battery::process::execute_sync(): Process is still running. Please join before executing again");

        thread = battery::thread([&] { run_process(); });
        thread.join();
        return out.status == 0;
    }

    void process::execute_async() {
        std::unique_lock<std::mutex> lock(func_mutex);

        if (thread.joinable())
            throw battery::exception("battery::process::execute_async(): Process is still running. Please join before executing again");

        thread = battery::thread([&] { run_process(); });
    }

    bool process::join() {
        std::unique_lock<std::mutex> lock(func_mutex);

        if (!thread.joinable())
            throw battery::exception("battery::process::join(): Cannot join process, there is nothing to join. Did you intend to use .execute_async() instead of .execute()?");

        thread.join();
        return out.status == 0;
    }

    void process::run_process() {   // TODO !!!!! : Process stdout and stdin has no unicode support!!!

        // Convert commands from STL vector to raw C-style string pointers
        std::vector<const char*> cmd_cstr;

#ifdef BATTERY_ARCH_WINDOWS     // TODO: Check if this is cross-platform
        cmd_cstr.push_back("cmd.exe");
        cmd_cstr.push_back("/c");
#endif

        for (auto& c : in.command) cmd_cstr.emplace_back(c.c_str());
        cmd_cstr.emplace_back(nullptr);                 // TODO: Add feature for continuous memory stream for stdout

        // Define how to start the process
        reproc::process process;
        std::error_code ec;
        reproc::options options;
        options.deadline = reproc::milliseconds(in.timeout_ms);
        if (in.redirect == redirect::PARENT) {
            options.redirect.parent = true;
        }
        std::string workdir = in.working_directory.has_value() ? in.working_directory->to_string() : "";
        options.working_directory = !workdir.empty() ? workdir.c_str() : nullptr;

        if (!fs::is_directory(options.working_directory)) {
            throw battery::exception(std::string("Cannot run process in working directory ") + options.working_directory + ": No such file or directory");
        }

        // Now start and detach the process
        ec = process.start(cmd_cstr.data(), options);
        if (ec) {
            out.status = ec.value();
            out.error_message = ec.message();
            out.output = "";
            return;
        }

        // Define how to handle stdout and stderr
        std::string output;
        reproc::sink::string sink(output);
        ec = drain(process, sink, reproc::sink::null);
        if (ec) {
            out.status = ec.value();
            out.error_message = ec.message();
            out.output = "";
            return;
        }

        // And finally join the process and wait until it finishes
        auto [status, _ec] = process.stop(options.stop);
        if (_ec) {
            out.status = _ec.value();
            out.error_message = _ec.message();
            out.output = "";
            return;
        }

        out.output = output;
        out.error_message = _ec.message();
        out.status = status;

        //log::info("Result: {} {} \n{}", status, _ec.message(), output);
    }

}
