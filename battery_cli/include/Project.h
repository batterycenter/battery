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

    nlohmann::json data;
    std::unordered_map<std::string, std::string> scripts;
    b::cachefile projectCache;

    Project();

    bool isProjectConfigured();

    [[nodiscard]] b::expected<std::nullopt_t, Error> init(const std::string& cmake_flags, const std::string& args);

    [[nodiscard]] b::expected<std::nullopt_t, Error> findProjectRoot();
    [[nodiscard]] b::expected<std::nullopt_t, Error> fetchProjectData();

    void printScriptLabel(const std::string& script);
    [[nodiscard]] b::expected<std::nullopt_t, Error> runScript(std::string script);

};
