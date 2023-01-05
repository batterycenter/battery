
#include "battery_cli.h"

Err cli_run() {
    Log::Info("battery run command");
    return { Result::SUCCESS, "" };
}
