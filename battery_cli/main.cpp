
#include "Battery/Battery.h"
using namespace Battery;

const char* message = "Battery-CLI https://github.com/HerrNamenlos123/Battery";

// TODO: Print version (add option)

int Battery::Main(const Args_t& args) {

    CLI::App app(message);

    std::string input_file;
    app.add_option("input_file", input_file, "The file to be embedded")->required()->check(CLI::ExistingFile);

    std::string source_file_output_directory;
    app.add_option("out_dir", source_file_output_directory,
                   "Output dir for C++ source file")->required();

    std::string header_file_output_directory;
    app.add_option("out_dir_header", header_file_output_directory,
                   "Output dir for C++ header file (If omitted, the same output dir is used)");

    std::string symbol_name;
    app.add_option("--symbol_name", symbol_name, "Override the symbol name for the output files");

    bool binary = false;
    app.add_flag("--binary", binary, "File is in binary format instead of plain text (regarding line endings)");

    bool print_header_name_only = false;
    app.add_flag("--print-header-name-only", print_header_name_only, "Only print the final output name of the generated C++ header file");

    bool print_src_name_only = false;
    app.add_flag("--print-src-name-only", print_src_name_only, "Only print the final output name of the generated C++ source file");

	//for (const auto& arg : args)
	//	std::cout << arg << std::endl;
    try {
        std::vector<const char*> __args;
        for (auto arg : args) {
            __args.emplace_back(arg.c_str());
            std::cout << arg << std::endl;
        }
        (app).parse(args.size(), __args.data());
    }
    catch (const CLI::ParseError& e) {
        return (app).exit(e);
    }
    //CLI11_PARSE(app, args);
	
    return 0;
}
