
# Usage of this module:
# battery_add_module(cli11)
# target_link_libraries(... PUBLIC battery::cli11)

# Download the CLI11 library from Github
set(CLI11_VERSION "v2.3.2")
set(CLI11_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CLI11_VERSION}/include")
set(CLI11_HEADER_FILE "${CLI11_INCLUDE_DIR}/battery/cli11.hpp")

if (NOT EXISTS "${CLI11_HEADER_FILE}")
    message(STATUS "Downloading CLI11 ${CLI11_VERSION} from Github")
    file(DOWNLOAD
            "https://github.com/CLIUtils/CLI11/releases/download/${CLI11_VERSION}/CLI11.hpp"
            "${CLI11_HEADER_FILE}"
            )
endif ()

add_library(battery_cli11 INTERFACE)
add_library(battery::cli11 ALIAS battery_cli11)
target_include_directories(battery_cli11 INTERFACE ${CLI11_INCLUDE_DIR})