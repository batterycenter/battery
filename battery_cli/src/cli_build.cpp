
#include "tools.h"

Err cmake_build(const ProjectData& project) {
    battery::process cmake;
    cmake.in.command = { "cmake", "--build", "." };
    cmake.in.working_directory = project.project_root + project.cmake_path;
    cmake.execute();

    if (cmake.out.status != 0) {
        log::error("The CMake subprocess failed with error code {}: {}", cmake.out.status, cmake.out.error_message);
        return { Result::SUBPROCESS_FAILED, "" };
    }
    return { Result::SUCCESS, "" };
}

Err cli_build(const ProjectData& project) {     // TODO: Add a cache entry and run configure if it hasn't been run yet

    auto err = cmake_build(project);
    if (err.first != Result::SUCCESS) {
        return err;
    }

    log::info("Project built successfully. Use 'battery run' to execute it");

    return { Result::SUCCESS, "" };
}
