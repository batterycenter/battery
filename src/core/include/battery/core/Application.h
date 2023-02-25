
#ifndef BATTERY_CORE_APPLICATION_H_
#define BATTERY_CORE_APPLICATION_H_

#include <string>
#include <vector>

namespace battery {

class Application {
public:

    // These are your command line arguments your program was called with. Guaranteed to be cross-platform and UTF-8
    std::vector<std::string> cli_args;

    // [Windows-only] If true, battery will automatically set the caller's terminal codepage to UTF-8 when calling .run()
    bool win_adjust_codepage = true;

    Application();
    virtual ~Application();

    void run(int argc, const char** argv, const std::string& app_name);

private:
    void SetupWindowsConsole();
    std::vector<std::string> ParseCLI(int argc, const char** argv);

private:
    std::string app_name;

};

}   // namespace battery

#endif // BATTERY_CORE_APPLICATION_H_
