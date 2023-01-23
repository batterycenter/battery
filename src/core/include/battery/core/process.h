#pragma once

#include "battery/core/fs.h"
#include "battery/core/thread.h"

namespace battery {

    class process {
    public:

        enum class redirect {
            PARENT,
            MEMORY
        };

        struct {
            std::vector<std::string> command;
            std::optional<fs::path> working_directory;
            enum redirect redirect = redirect::PARENT;
            int timeout_ms = 0;
        } in;

        struct {
            int status = 0;
            std::string error_message;
            std::string output;
        } out;

        process();
        ~process();

        bool execute();
        void execute_async();
        bool join();

    private:
        void run_process();

        battery::thread thread;
        std::mutex func_mutex;

    };

}