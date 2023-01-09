#pragma once

#include "battery/core/fs.h"
#include "battery/core/thread.h"

namespace battery {

    class process {
    public:

        fs::path executable;
        std::vector<std::string> args;
        fs::path working_directory;

        process();
        ~process();

        [[nodiscard]] void execute();
        void execute_async();
        [[nodiscard]] void join();

    private:
        std::pair<int, std::string> error;

        void run_process();

        battery::thread thread;
        std::mutex func_mutex;

    };

}