
#include "Error.h"
#include "Project.h"
#include "ProjectGenerator.h"
#include "resources/version.txt.h"

static void executeProgram(const std::string& executable, const std::string& args) {
    auto exe = b::fs::path(executable).make_preferred();
    auto result = b::execute(exe.to_string() + " " + args);
    std::cout << std::endl;
    b::print("{} terminated with exit code {}", b::fs::path(executable).filename(), result.exit_code);
}

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
    auto batterycli_execute = batterycli.add_subcommand("execute", "Execute a program specified by path (b execute --help for more)");
    batterycli.require_subcommand(1);  // need exactly 1 subcommand

    std::string script;
    batterycli_run->add_option("script", script);

    std::vector<std::string> additional_args;
    batterycli_run->add_option("--args", additional_args, "Any arguments to pass to the executable when running");
    batterycli_start->add_option("--args", additional_args, "Any arguments to pass to the executable when running");
    batterycli_execute->add_option("--args", additional_args, "Any arguments to pass to the executable");

    bool configure_cache = false;
    batterycli_configure->add_flag("--cache", configure_cache, "Only configure if the project hasn't been configured yet");

    std::string cmake_flags;
    batterycli_new->add_flag("--cmake-flags", cmake_flags, "Any parameters to pass to CMake directly");
    batterycli_configure->add_flag("--cmake-flags", cmake_flags, "Any parameters to pass to CMake directly");
    batterycli_build->add_flag("--cmake-flags", cmake_flags, "Any parameters to pass to CMake directly");
    batterycli_start->add_flag("--cmake-flags", cmake_flags, "Any parameters to pass to CMake directly");
    batterycli_run->add_flag("--cmake-flags", cmake_flags, "Any parameters to pass to CMake directly");

    std::string root_dir;
    batterycli_configure->add_option("--root", root_dir, "The root directory to look for battery.toml in");
    batterycli_build->add_option("--root", root_dir, "The root directory to look for battery.toml in");
    batterycli_start->add_option("--root", root_dir, "The root directory to look for battery.toml in");
    batterycli_run->add_option("--root", root_dir, "The root directory to look for battery.toml in");

    std::string executable;
    batterycli_execute->add_option("executable", executable, "The full path of the executable to run");

    try {
        batterycli.parse(static_cast<int>(args.size()), b::args_to_argv(args));
    } catch(const CLI::ParseError &e) {
        batterycli.exit(e);
        return b::unexpected(Error::CLI_FAILED_TO_PARSE);
    }

    if (batterycli_execute->parsed()) {           // b execute ...
        executeProgram(executable, b::join(additional_args, " "));
        return std::nullopt;
    }
    else if (batterycli_new->parsed()) {          // b new ...
        ProjectGenerator generator;
        return generator.run();
    }

    Project project;
    auto result = project.init(cmake_flags, root_dir, b::join(additional_args, " "));
    if (!result) {
        return b::unexpected(result.error());
    }

    if(batterycli_configure->parsed()) {           // b configure ...
        if (configure_cache) {
            if (!project.isProjectConfigured()) {
                return project.runScript("configure");
            }
            else {
                project.printScriptLabel("configure --cache");
                b::print(b::print_color::YELLOW, "Skipping configure stage, as the project has already been configured once\n");
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

    b::log::error("Unreachable Code: Somehow the subcommand switch got bypassed, this should be impossible...");
    return b::unexpected(Error::INTERNAL_ERROR);
}

class foo {
public:
    foo(const std::string& str) {
        b::print(b::print_color::GREEN, "foo constructor {}", str);
    }
    ~foo() {
        b::print(b::print_color::GREEN, "foo destructor");
    }
};

int b::main(const std::vector<std::string>& args) {

    b::set_ctrl_c_handler([]() {
        Project::terminateApplication = true;
        if (Project::terminateCallback) {
            Project::terminateCallback();
        }
        std::cout << "Ctrl-C\n";
    });

    b::print_pattern("%^>> %v%$");  // Output format of log messages

    auto result = parse_cli(args);
    if (!result) {                               // Only error codes greater than 0 are printed
        b::log::error("battery terminated with exit code {}: {}", (int)result.error(), magic_enum::enum_name(result.error()));
        return (int)result.error();
    }

    return 0;
}
