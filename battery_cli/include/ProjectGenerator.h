#pragma once

#include "battery/core/all.hpp"
#include "Error.h"

class ProjectGenerator {
public:
    // Properties of the project
    bool library = false;
    std::string project_name;
    b::semver::version project_version;
    b::fs::path project_dir;

    // File contents, loaded before writing to disk
    std::string gitignore;
    std::string cmakelists;
    std::string cmake_cpm_cmake;
    std::string battery_toml;
    std::string version_txt;
    std::string main_cpp;
    std::string pch_hpp;

    ProjectGenerator() {}

    [[nodiscard]] b::expected<std::nullopt_t, Error> run(const std::string& new_dir);

    [[nodiscard]] b::expected<std::string, Error> generateSourceFile(const std::string& name, std::string content);
    [[nodiscard]] b::expected<std::nullopt_t, Error> generate();

    [[nodiscard]] b::expected<std::nullopt_t, Error> writeSourceFile(const b::fs::path& filename, const std::string& data);
    [[nodiscard]] b::expected<std::nullopt_t, Error> writeToDisk();
};
