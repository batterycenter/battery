
#include "battery/core/all.h"
#include <gtest/gtest.h>

TEST(BatteryCore_Process, ProcessRunsSuccessfully) {
    battery::process proc;
#ifdef BATTERY_ARCH_WINDOWS
    proc.options.executable = "cmd.exe";
    proc.options.arguments = { "/c", "exit", "100" };
#else
    proc.options.executable = "bash";
    proc.options.arguments = { "-c", "exit 100" };
#endif
    proc.execute_sync();
    EXPECT_EQ(proc.exit_code, 100);
}

TEST(BatteryCore_Process, ProcessRunsSuccessfullyAsync) {
    battery::process proc;
#ifdef BATTERY_ARCH_WINDOWS
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

TEST(BatteryCore_Process, ProcessRedirectStdout) {
    battery::process proc;
#ifdef BATTERY_ARCH_WINDOWS
    proc.options.executable = "cmd.exe";
    proc.options.arguments = { "/c", "echo unit test works" };
#else
    proc.options.executable = "bash";
    proc.options.arguments = { "-c", "echo unit test works" };
#endif
    proc.options.suppress_carriage_return = true;
    proc.options.strip_trailing_whitespace_after_join = true;
    std::string output;
    proc.stdout_callback = [&output] (const auto& str) {
        output += str;
    };
    proc.execute_sync();
    EXPECT_EQ(proc.output_combined, "unit test works");
    EXPECT_EQ(output, "unit test works\n");
    EXPECT_EQ(proc.exit_code, 0);
}

TEST(BatteryCore_Process, ProcessRedirectStdin) {
    battery::process proc;
#ifdef BATTERY_ARCH_WINDOWS
    auto filename = "battery-core-unit-test-process-redirect.bat";
    battery::fs::ofstream file(filename);
    file <<
        "@echo off\n"
        "set /p test=\n"
        "echo %test%\n"
        "exit 100";
    file.close();

    proc.options.executable = "cmd.exe";
    proc.options.arguments.emplace_back("/c");
    proc.options.arguments.emplace_back(filename);
#else
    auto filename = "battery-core-unit-test-process-redirect.bat";
    battery::fs::ofstream file(filename);
    file <<
        "#!/bin/bash\n"
        "read -p \"Input: \" test\n"
        "echo $test\n"
        "exit 100";
    file.close();

    proc.options.executable = "bash";
    proc.options.arguments.emplace_back(filename);
#endif
    proc.execute_async();
    proc.stdin_write("unit test works\n");
    proc.join();
    EXPECT_EQ(proc.output_combined, "unit test works");
    EXPECT_EQ(proc.exit_code, 100);
    battery::fs::remove(filename);
}