
#include "tools.h"

Err cmake_configure(const ProjectData& project) {
    b::process cmake;
    cmake.options.executable = "cmake";
    cmake.options.executable = { ".." };
    cmake.options.working_directory = project.project_root + project.cmake_path;
    cmake.execute_sync();

    if (cmake.exit_code != 0) {
        battery::log::error("The CMake subprocess failed with error code {}: {}", cmake.exit_code, cmake.error_message);
        return { Result::SUBPROCESS_FAILED, "" };
    }
    return { Result::SUCCESS, "" };
}

Err cli_configure(const ProjectData& project) {

    auto err = cmake_configure(project);
    if (err.first != Result::SUCCESS) {
        return err;
    }

    battery::log::info("Project configured. Use 'battery build' or 'battery run' to use it");

    return { Result::SUCCESS, "" };
}
