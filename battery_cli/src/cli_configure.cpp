
#include "tools.h"

Err cmake_configure(const ProjectData& project) {
    battery::process cmake;
    cmake.in.command = { "cmake", ".." };
    cmake.in.working_directory = project.project_root + project.cmake_path;
    cmake.execute();

    if (cmake.out.status != 0) {
        log::error("The CMake subprocess failed with error code {}: {}", cmake.out.status, cmake.out.error_message);
        return { Result::SUBPROCESS_FAILED, "" };
    }
    return { Result::SUCCESS, "" };
}

Err cli_configure(const ProjectData& project) {

    auto err = cmake_configure(project);
    if (err.first != Result::SUCCESS) {
        return err;
    }

    log::info("Project configured. Use 'battery build' or 'battery run' to use it");

    return { Result::SUCCESS, "" };
}
