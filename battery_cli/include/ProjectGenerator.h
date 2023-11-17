#pragma once

#include "battery/battery.hpp"
#include "Error.h"

class ProjectGenerator {
public:
    // Properties of the project
    bool library = false;
    b::string project_name;
    b::semver::version project_version;
    b::fs::path project_dir;

    // File contents, loaded before writing to disk
    b::string gitignore;
    b::string cmakelists;
    b::string cmake_cpm_cmake;
    b::string battery_toml;
    b::string version_txt;
    b::string main_cpp;
    b::string pch_hpp;

    ProjectGenerator() {}

    [[nodiscard]] b::expected<std::nullopt_t, Error> run(const b::string& new_dir);

    [[nodiscard]] b::expected<b::string, Error> generateSourceFile(const b::string& name, b::string content);
    [[nodiscard]] b::expected<std::nullopt_t, Error> generate();

    [[nodiscard]] b::expected<std::nullopt_t, Error> writeSourceFile(const b::fs::path& filename, const b::string& data);
    [[nodiscard]] b::expected<std::nullopt_t, Error> writeToDisk();
};
