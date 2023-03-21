//
//b::expected<std::nullopt_t,Error> cmake_configure(const ProjectData& project) {
//    b::process cmake;
//    cmake.options.executable = "cmake";
//    cmake.options.arguments = { ".." };
//    cmake.options.working_directory = project.project_root + project.cmake_path;
//    cmake.execute_sync();
//
//    if (cmake.exit_code != 0) {
//        battery::log::error("The CMake subprocess failed with error code {}: {}", cmake.exit_code, cmake.error_message);
//        return b::unexpected<Error>(Error::SUBPROCESS_FAILED);
//    }
//    return std::nullopt;
//}
//
//b::expected<std::nullopt_t,Error> cli_configure(const ProjectData& project) {
//    auto success = cmake_configure(project);
//    if (!success) {
//        return b::unexpected(success.error());
//    }
//
//    battery::log::info("Project configured. Use 'battery build' or 'battery run' to use it");
//
//    return std::nullopt;
//}
