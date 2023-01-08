
#include "battery_cli.h"

Err parse_cli(const Args_t& args) {

    std::string version_message = CLI_PRODUCT_NAME + " version " + resources::version_txt.str();
    std::string message = version_message + "\n"
        "Your main tool for working with https://github.com/HerrNamenlos123/Battery\n"
        "Supports you with project generation, building, deploying to your cloud and more!\n";

    CLI::App batterycli(message);
    batterycli.set_version_flag("--version", version_message, "Print the version");
    
    auto batterycli_new = batterycli.add_subcommand("new", "Generate a new C++ project using Battery (--help for more)");
    auto batterycli_configure = batterycli.add_subcommand("configure", "Only configure the Battery project (--help for more)");
    auto batterycli_build = batterycli.add_subcommand("build", "Build the Battery project (--help for more)");
    auto batterycli_run = batterycli.add_subcommand("run", "Build and run the Battery project (--help for more)");
    batterycli.require_subcommand(1);  // need exactly 1 subcommand

    try {
        batterycli.parse(args.size(), args_to_cstr(args));
    } catch(const CLI::ParseError &e) {
        batterycli.exit(e);     // Simply print the error. We are not interested in the error code
        return { Result::CLI_INVALID_ARGUMENTS, "Invalid arguments were given, CLI failed to parse" };
    }

    if (batterycli_new->parsed()) {               // battery new ...
        return cli_new();
    }
    else if(batterycli_configure->parsed()) {     // battery configure ...
        return cli_configure();
    }
    else if(batterycli_build->parsed()) {         // battery build ...
        return cli_build();
    }
    else if(batterycli_run->parsed()) {           // battery run ...
        return cli_run();
    }

    return { Result::INTERNAL_ERROR, "Unreachable Code: Somehow the subcommand check got bypassed, this should be impossible..." };
}

int battery::main(const Args_t& args) {

    auto [errorcode, errormessage] = parse_cli(args);
    if ((int)errorcode > 0) {                               // Only error codes greater than 0 are printed
        log::error("Error {} {}: {}", (int)errorcode, magic_enum::enum_name(errorcode), errormessage);
    }
    return (int)errorcode;
}
