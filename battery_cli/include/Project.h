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

    Project() = default;

    bool isProjectConfigured();

    [[nodiscard]] b::expected<std::nullopt_t, Error> init(const std::string& cmake_flags, const std::string& root, const std::string& args);

    [[nodiscard]] b::expected<std::nullopt_t, Error> findProjectRoot(const std::string& root);
    [[nodiscard]] b::expected<std::nullopt_t, Error> fetchProjectData(const std::string& root);

    void printScriptLabel(const std::string& script);
    [[nodiscard]] b::expected<std::string, Error> renderString(std::string string);
    [[nodiscard]] b::expected<std::nullopt_t, Error> runScript(std::string script);

    [[nodiscard]] b::expected<std::nullopt_t, Error> generateFile(const b::fs::path& input_path, const b::fs::path& output_path);

};
