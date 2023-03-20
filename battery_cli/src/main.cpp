
#include "app.h"
#include "tools.h"
#include "scripts.h"

Err parse_cli(const std::vector<std::string>& args) {

    std::string version_message = CLI_PRODUCT_NAME + " version ";// + resources::version_txt.str();     // TODO !!!!
    std::string message = version_message + "\n"
        "Your main tool for working with https://github.com/HerrNamenlos123/Battery\n"
        "Supports you with project generation, building, deploying to your cloud and more!\n";

    CLI::App batterycli(message);
    batterycli.set_version_flag("--version", version_message, "Print the version");
    
    auto batterycli_new = batterycli.add_subcommand("new", "Generate a new C++ project using Battery (--help for more)");
    auto batterycli_configure = batterycli.add_subcommand("configure", "Only configure an existing Battery project (--help for more)");
    auto batterycli_build = batterycli.add_subcommand("build", "Build an existing Battery project (--help for more)");
    auto batterycli_run = batterycli.add_subcommand("run", "Build and run an existing Battery project (--help for more)");
    batterycli.require_subcommand(1);  // need exactly 1 subcommand

    try {
        batterycli.parse(args.size(), b::args_to_argv(args));
    } catch(const CLI::ParseError &e) {
        batterycli.exit(e);     // Simply print the error. We are not interested in the error code
        return { Result::CLI_INVALID_ARGUMENTS, "Invalid arguments were given, CLI failed to parse" };
    }

    //auto project_data_opt = fetch_project_data();
    //if (!project_data_opt) return project_data_opt.error();
    //ProjectData project = project_data_opt.value();

    //battery::log::info("Battery project file found at {}:", project.project_root.to_string());
    //battery::log::info("Project: {}", project.project_name);
    //battery::log::info("Version: {}.{}.{}", project.project_version.major, project.project_version.minor, project.project_version.patch);

    //if (batterycli_new->parsed()) {               // battery new ...
    //    return cli_new(project);
    //}
    //else if(batterycli_configure->parsed()) {     // battery configure ...
    //    return run_script(project, "configure");
    //}
    //else if(batterycli_build->parsed()) {         // battery build ...
    //    return run_script(project, "build");
    //}
    //else if(batterycli_run->parsed()) {           // battery start ...
    //    return run_script(project, "start");
    //}

    return { Result::INTERNAL_ERROR, "Unreachable Code: Somehow the subcommand check got bypassed, this should be impossible..." };
}

#include "resources/version.txt.h"

int b::main(const std::vector<std::string>& args) {

    std::cout << resources::version_txt << std::endl;

    battery::log::pattern("%^>> %v%$");  // Output format of log messages

    auto [errorcode, errormessage] = parse_cli(args);
    if ((int)errorcode > 0) {                               // Only error codes greater than 0 are printed
        battery::log::error("Error {} {}: {}", (int)errorcode, magic_enum::enum_name(errorcode), errormessage);
    }

    return (int)errorcode;
}
