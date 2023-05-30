
#include "Error.h"
#include "Project.h"
#include "ProjectGenerator.h"
#include "resources/battery_cli/version.hpp"

[[nodiscard]] static b::expected<std::nullopt_t,Error> executeProgram(const b::string& executable, const b::string& args) {
    auto exe = b::fs::path(executable).make_preferred();

    if (!b::fs::is_regular_file(exe)) {
        b::print("\n");
        b::log::warn("Could not find executable '{}': Script failed", exe);
        return b::unexpected(Error::EXECUTABLE_NOT_FOUND);
    }

    b::print("\n{}\n", exe);
    auto result = b::execute(exe + " " + args);
    b::print("\n>> {} terminated with exit code {}\n", b::fs::path(executable).filename(), result.exit_code);
    return std::nullopt;
}

b::expected<std::nullopt_t,Error> parse_cli(const std::vector<b::string>& args) {

    b::string version_message = CLI_PRODUCT_NAME + " version " + resources::battery_cli::version.string();
    b::string message = version_message + "\n"
        "Your main tool for working with https://github.com/batterycenter/battery\n"
        "Supports you with project generation, building, deploying to your cloud and more!\n";

    CLI::App batterycli(message);
    batterycli.set_version_flag("--version", version_message, "Print the version");
    
    auto batterycli_new = batterycli.add_subcommand("new", "Generate a new C++ project using Battery (b new --help for more)");
    auto batterycli_configure = batterycli.add_subcommand("configure", "Only configure an existing Battery project (b configure --help for more)");
    auto batterycli_build = batterycli.add_subcommand("build", "Build an existing Battery project (b build --help for more)");
    auto batterycli_start = batterycli.add_subcommand("start", "Build and run an existing Battery project (b start --help for more)");
    auto batterycli_run = batterycli.add_subcommand("run", "Run a (custom) script (b run --help for more)");
    auto batterycli_generate = batterycli.add_subcommand("generate", "Generate a file and replace keywords using the template engine (b generate --help for more)");
    auto batterycli_docs = batterycli.add_subcommand("docs", "Build the doxygen documentation for your project (b docs --help for more)");
    auto batterycli_execute = batterycli.add_subcommand("execute", "Execute a program specified by path (b execute --help for more)");
    batterycli.require_subcommand(1);  // need exactly 1 subcommand

    b::string script;
    batterycli_run->add_option("script", script);

    std::vector<b::string> additional_args;
    batterycli_run->add_option("--args", additional_args, "Any arguments to pass to the executable when running");
    batterycli_start->add_option("--args", additional_args, "Any arguments to pass to the executable when running");
    batterycli_execute->add_option("--args", additional_args, "Any arguments to pass to the executable");

    b::string input_file;
    batterycli_generate->add_option("input_file", input_file, "Which file to process")->required();
    b::string output_file;
    batterycli_generate->add_option("output_file", output_file, "Where to write the processed file")->required();

    // TODO: Add scripts --list option
    bool configure_cache = false;
    batterycli_configure->add_flag("--cache", configure_cache, "Only configure if the project hasn't been configured yet");

    b::string new_dir;
    batterycli_new->add_option("root_directory", new_dir, "Where to generate the new project (optional)");

    b::string cmake_flags;
    batterycli_new->add_flag("--cmake-flags", cmake_flags, "Any parameters to pass to CMake directly");
    batterycli_configure->add_flag("--cmake-flags", cmake_flags, "Any parameters to pass to CMake directly");
    batterycli_build->add_flag("--cmake-flags", cmake_flags, "Any parameters to pass to CMake directly");
    batterycli_start->add_flag("--cmake-flags", cmake_flags, "Any parameters to pass to CMake directly");
    batterycli_run->add_flag("--cmake-flags", cmake_flags, "Any parameters to pass to CMake directly");

    b::string root_dir;
    batterycli_configure->add_option("--root", root_dir, "The root directory to look for battery.toml in");
    batterycli_build->add_option("--root", root_dir, "The root directory to look for battery.toml in");
    batterycli_start->add_option("--root", root_dir, "The root directory to look for battery.toml in");
    batterycli_run->add_option("--root", root_dir, "The root directory to look for battery.toml in");

    b::string executable;
    batterycli_execute->add_option("executable", executable, "The full path of the executable to run");

    try {
        batterycli.parse(static_cast<int>(args.size()), b::args_to_argv(args));
    } catch(const CLI::ParseError &e) {
        batterycli.exit(e);
        return b::unexpected(Error::CLI_FAILED_TO_PARSE);
    }

    if (batterycli_execute->parsed()) {           // b execute ...
        return executeProgram(executable, b::string::join(additional_args, " "));
    }
    else if (batterycli_new->parsed()) {          // b new ...
        ProjectGenerator generator;
        return generator.run(new_dir);
    }

    Project project;
    auto result = project.init(cmake_flags, root_dir, b::string::join(additional_args, " "));
    if (!result) {
        return b::unexpected(result.error());
    }

    if(batterycli_configure->parsed()) {           // b configure ...
        if (configure_cache) {
            if (!project.isProjectConfigured()) {
                return project.runScript("configure");
            }
            else {
                b::print("\n");
                project.printScriptLabel("configure --cache");
                b::print(b::colors::fg::yellow, ">> Skipping configure stage, as the project has already been configured once\n");
                return std::nullopt;
            }
        }
        else {
            return project.runScript("configure");
        }
    }
    else if(batterycli_build->parsed()) {          // b build ...
        return project.runScript("build");
    }
    else if(batterycli_start->parsed()) {          // b start ...
        return project.runScript("start");
    }
    else if(batterycli_run->parsed()) {            // b run ...
        return project.runScript(script);
    }
    else if(batterycli_generate->parsed()) {       // b generate ...
        return project.generateFile(input_file, output_file);
    }
    else if(batterycli_docs->parsed()) {           // b docs ...
        return project.runScript("build_docs");
    }

    b::log::error("Unreachable Code: Somehow the subcommand switch got bypassed, this should be impossible...");
    return b::unexpected(Error::INTERNAL_ERROR);
}

int b::main(const std::vector<b::string>& args) {

    b::push_ctrl_c_handler([]{});   // Ignore Ctrl+C, we push another handler later

    auto result = parse_cli(args);
    if (!result) {
        if (result.error() == Error::TERMINATED_BY_USER) {  // Silent error pass-on
            return (int)result.error();
        }
        b::log::error("battery terminated with exit code {}: {}", (int)result.error(), magic_enum::enum_name(result.error()));
        return (int)result.error();
    }

    return 0;
}
