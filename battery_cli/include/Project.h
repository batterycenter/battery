#pragma once

#include <string>
#include "battery/core/all.hpp"

#include "Error.h"

inline const std::string CLI_PRODUCT_NAME = "Battery";
inline const std::string BATTERY_PROJECT_FILE_NAME = "battery.toml";

class Project {
public:
    std::string projectName;
    b::fs::path projectRoot;
    semver::version projectVersion;
    std::string buildDirectory = "build";
    std::string sourceDirectory = ".";
    std::string cmakeFlags = "";

    std::unordered_map<std::string, std::string> scripts;

    Project();

    b::expected<std::nullopt_t, Error> findProjectRoot();

    b::expected<std::nullopt_t, Error> fetchProjectData();

    b::expected<std::nullopt_t, Error> generateNewProject();

    b::expected<std::nullopt_t, Error> runScript(std::string script);

};
