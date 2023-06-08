
#include "battery/core/all.hpp"
#include <gtest/gtest.h>

TEST(BatteryCore_Process, ProcessRunsSuccessfully) {
    b::process proc;
    if (b::Constants::Platform() == b::Platform::Windows) {
        proc.options.executable = "cmd.exe";
        proc.options.arguments = { "/c", "exit", "100" };
    }
    else {
        proc.options.executable = "bash";
        proc.options.arguments = { "-c", "exit 100" };
    }
    proc.execute_sync();
    EXPECT_EQ(proc.exit_code, 100);
}

TEST(BatteryCore_Process, ProcessRunsSuccessfullyAsync) {
    b::process proc;
#ifdef B_OS_WINDOWS
    proc.options.executable = "cmd.exe";
    proc.options.arguments = { "/c", "exit", "100" };
#else
    proc.options.executable = "bash";
    proc.options.arguments = { "-c", "exit 100" };
#endif
    proc.execute_async();
    proc.join();
    EXPECT_EQ(proc.exit_code, 100);
}

TEST(BatteryCore_Process, ProcessRedirectStdoutUtf8) {
    b::process proc;
    if (b::Constants::Platform() == b::Platform::Windows) {
        proc.options.executable = "cmd.exe";
        proc.options.arguments = { "/c", "echo unit test works: Süßölgefäß 国 分 高" };
    }
    else {
        proc.options.executable = "bash";
        proc.options.arguments = { "-c", "echo unit test works: Süßölgefäß 国 分 高" };
    }
    proc.options.suppress_carriage_return = true;
    proc.options.silent = true;
    proc.options.strip_trailing_whitespace_after_join = true;
    b::string output;
    proc.options.stdout_callback = [&output] (const auto& str) {
        output += str;
    };
    proc.execute_sync();
    EXPECT_EQ(proc.output_combined, "unit test works: Süßölgefäß 国 分 高");
    EXPECT_EQ(output, "unit test works: Süßölgefäß 国 分 高\n");
    EXPECT_EQ(proc.exit_code, 0);
}

TEST(BatteryCore_Process, ProcessRedirectStdin) {
    b::process proc;
    if (b::Constants::Platform() == b::Platform::Windows) {
        auto filename = "battery-core-unit-test-process-redirect.bat";
        b::fs::ofstream file(filename);
        file <<
            "@echo off\n"
            "set /p test=\n"
            "echo %test%\n"
            "exit 100";
        file.close();

        proc.options.executable = "cmd.exe";
        proc.options.arguments.emplace_back("/c");
        proc.options.arguments.emplace_back(filename);
    }
    else {
        auto filename = "battery-core-unit-test-process-redirect.sh";
        b::fs::ofstream file(filename);
        file <<
            "#!/bin/bash\n"
            "read -p \"Input: \" test\n"
            "echo $test\n"
            "exit 100";
        file.close();

        proc.options.executable = "bash";
        proc.options.arguments.emplace_back(filename);
    }
    proc.options.silent = true;
    proc.execute_async();
    proc.stdin_write("unit test works\n");
    proc.join();
    EXPECT_EQ(proc.output_combined, "unit test works");
    EXPECT_EQ(proc.exit_code, 100);
    b::fs::remove(filename);
}

TEST(BatteryCore_Process, ProcessExecuteDirect) {
    auto proc = b::execute("echo unit test works && exit 187");
    EXPECT_EQ(proc.exit_code, 187);
}
