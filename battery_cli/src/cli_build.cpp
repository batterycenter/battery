
#include "battery_cli.h"

Err cli_build() {
    Log::Info("battery build command");
    return { Result::SUCCESS, "" };
}
