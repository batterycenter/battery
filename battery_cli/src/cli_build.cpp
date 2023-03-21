//
//b::expected<std::nullopt_t,Error> cmake_build(const ProjectData& project) {
//    b::process cmake;
//    cmake.options.executable = "cmake";
//    cmake.options.executable = { "--build", "." };
//    cmake.options.working_directory = project.project_root + project.cmake_path;
//    cmake.execute_sync();
//
//    if (cmake.exit_code != 0) {
//        battery::log::error("The CMake subprocess failed with error code {}: {}", cmake.exit_code, cmake.error_message);
//        return b::unexpected(Error::SUBPROCESS_FAILED);
//    }
//
//    return std::nullopt;
//}
//
//b::expected<std::nullopt_t,Error> cli_build(const ProjectData& project) {     // TODO: Add a cache entry and run configure if it hasn't been run yet
//
//    auto success = cmake_build(project);
//    if (!success) {
//        return b::unexpected(success.error());
//    }
//
//    battery::log::info("Project built successfully. Use 'battery run' to execute it");
//
//    return std::nullopt;
//}
