
#include "tools.h"

Err cmake_run(const ProjectData& project) {
    b::process cmake;
    cmake.options.executable = "cmake";
    cmake.options.executable = { "--build", "." };
    cmake.options.working_directory = project.project_root + project.cmake_path;
    cmake.execute_sync();

    if (cmake.exit_code != 0) {
        battery::log::error("The CMake subprocess failed with error code {}: {}", cmake.exit_code, cmake.error_message);
        return { Result::SUBPROCESS_FAILED, "" };
    }
    return { Result::SUCCESS, "" };
}

Err cli_run(const ProjectData& project) {     // TODO: Add a cache entry and run configure if it hasn't been run yet

    auto err = cmake_run(project);
    if (err.first != Result::SUCCESS) {
        return err;
    }

    battery::log::info("Project built successfully. Use 'battery run' to execute it");

    return { Result::SUCCESS, "" };
}
