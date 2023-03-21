
#include "Error.h"
#include "Project.h"
#include "resources/version.txt.h"

b::expected<std::nullopt_t,Error> parse_cli(const std::vector<std::string>& args) {

    std::string version_message = CLI_PRODUCT_NAME + " version " + resources::version_txt.str();
    std::string message = version_message + "\n"
        "Your main tool for working with https://github.com/HerrNamenlos123/Battery\n"
        "Supports you with project generation, building, deploying to your cloud and more!\n";

    CLI::App batterycli(message);
    batterycli.set_version_flag("--version", version_message, "Print the version");
    
    auto batterycli_new = batterycli.add_subcommand("new", "Generate a new C++ project using Battery (b new --help for more)");
    auto batterycli_configure = batterycli.add_subcommand("configure", "Only configure an existing Battery project (b configure --help for more)");
    auto batterycli_build = batterycli.add_subcommand("build", "Build an existing Battery project (b build --help for more)");
    auto batterycli_start = batterycli.add_subcommand("start", "Build and run an existing Battery project (b start --help for more)");
    auto batterycli_run = batterycli.add_subcommand("run", "Run a (custom) script (b run --help for more)");
    batterycli.require_subcommand(1);  // need exactly 1 subcommand

    try {
        batterycli.parse(static_cast<int>(args.size()), b::args_to_argv(args));
    } catch(const CLI::ParseError &e) {
        batterycli.exit(e);
        return b::unexpected(Error::CLI_FAILED_TO_PARSE);
    }

    Project project;
    auto result = project.fetchProjectData();
    if (!result) {
        return b::unexpected(result.error());
    }

    if (batterycli_new->parsed()) {               // b new ...
        return project.generateNewProject();
    }
    else if(batterycli_configure->parsed()) {     // b configure ...
        return project.runScript("configure");
    }
    else if(batterycli_build->parsed()) {         // b build ...
        return project.runScript("build");
    }
    else if(batterycli_start->parsed()) {          // b start ...
        return project.runScript("start");
    }
    else if(batterycli_run->parsed()) {            // b run ...
        return project.runScript("");
    }

    battery::log::error("Unreachable Code: Somehow the subcommand switch got bypassed, this should be impossible...");
    return b::unexpected(Error::INTERNAL_ERROR);
}

int b::main(const std::vector<std::string>& args) {

    battery::log::pattern("%^>> %v%$");  // Output format of log messages

    auto result = parse_cli(args);
    if (!result) {                               // Only error codes greater than 0 are printed
        battery::log::error("Battery stopped with error code {}: {}", (int)result.error(), magic_enum::enum_name(result.error()));
        return (int)result.error();
    }

    return 0;
}
