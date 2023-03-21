#pragma once

#define MESSAGES_TOML_NOT_FOUND \
    "{} not found in any of the parent directories. \n" \
    "   Make sure you are on the correct path, otherwise use 'b new' to generate a new project."

#define MESSAGES_CANNOT_READ_VERSION_FILE "Cannot read version file '{}': No such file or directory."

#define MESSAGES_INVALID_VERSION "'{}' does not conform to semantic versioning. Reason: {}"
