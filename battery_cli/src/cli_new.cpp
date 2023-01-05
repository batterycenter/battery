
#include "battery_cli.h"

Err cli_new() {
    Log::Info("battery new command");
    return { Result::SUCCESS, "" };
}
