
#include "battery/core/all.h"
using namespace battery;

// TODO: Print version (add option)
// TODO: Create cache file besides executable, that remembers the hash of a file and does not overwrite if not necessary
// TODO: Support file encryption while embedding, for executable reverse-engineering security
// TODO: Support nested directories in the resource folder

enum class ErrorCode {
    SUCCESS = 0,
    INPUT_FILE_FAILED = -1,
    OUTPUT_FILE_FAILED = -2,
    OUTPUT_HEADER_FILE_FAILED = -3
};

// Using lookup tables for converting ints to hex strings, for best performance
size_t write_hex_comma(char* buf, uint8_t num) {
    static const char lut[513] =
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f"
        "505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f"
        "707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f"
        "909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
        "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
        "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
        "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";

    buf[0] = '0';
    buf[1] = 'x';
    buf[2] = lut[(num & 0xFF) * 2];
	buf[3] = lut[(num & 0xFF) * 2 + 1];
	buf[4] = ',';
    return 5;
}

// Do the main conversion by reading and writing the files
std::pair<ErrorCode, std::string> convert(
    const std::string& input_file, 
    const std::string& source_dir, 
    const std::string& header_dir,
    std::string symbol_name, 
    bool binary, 
    bool print_header_name_only, 
    bool print_src_name_only
) {
    auto infile_name = fs::path(input_file).filename().string();

	// Replace a few characters to create a valid C++ symbol name
    if (symbol_name.empty()) {
        symbol_name = fs::path(input_file).filename().string();
		symbol_name = string::replace(symbol_name, ".", "_");
		symbol_name = string::replace(symbol_name, ":", "_");
		symbol_name = string::replace(symbol_name, ",", "_");
		symbol_name = string::replace(symbol_name, "-", "_");
		symbol_name = string::replace(symbol_name, " ", "_");
		symbol_name = string::replace(symbol_name, "Ä", "ae");
		symbol_name = string::replace(symbol_name, "ä", "ae");
		symbol_name = string::replace(symbol_name, "Ö", "oe");
		symbol_name = string::replace(symbol_name, "ö", "oe");
		symbol_name = string::replace(symbol_name, "Ü", "ue");
		symbol_name = string::replace(symbol_name, "ü", "ue");
		symbol_name = string::replace(symbol_name, "ß", "ss");
    }

    auto header_file = fs::path(header_dir).append(symbol_name).string() + ".h";
    auto source_file = fs::path(source_dir).append(symbol_name).string() + ".cpp";

    if (print_src_name_only) {
        std::cout << source_file << std::endl;
        return { ErrorCode::SUCCESS, "" };
    }
    if (print_header_name_only) {
        std::cout << header_file << std::endl;
        return { ErrorCode::SUCCESS, "" };
    }

    // Open input file to be embedded
    fs::ifstream infile(input_file, binary ? fs::Mode::BINARY : fs::Mode::TEXT);
    if (!infile.is_open()) return { ErrorCode::INPUT_FILE_FAILED, input_file };
    size_t filesize = infile.file_size();

    // Open output file for writing (.cpp)
    fs::ofstream outfile(source_file, fs::Mode::TEXT);
    if (!outfile.is_open()) return { ErrorCode::OUTPUT_FILE_FAILED, source_file + strerror(errno) };
	
    // Generate the source file
    outfile << "// File generated using battery_embed. https://github.com/HerrNamenlos123/Battery\n";
    outfile << "// Embedded file: " << fs::path(input_file).filename().string() << " | filesize: "
        << filesize << " bytes | was embedded as binary: " << (binary ? "true" : "false") << "\n";
    outfile << "// Header file containing the declaration for retrieving the data: " + infile_name + ".h\n";
    outfile << "// DO NOT EDIT THIS FILE!!!\n";
    outfile << "\n#include <cinttypes>\n\n";
    outfile << "namespace resources::internal {\n\n";
    outfile << "extern const size_t __" << symbol_name << "_size = " << filesize << ";\n";
    outfile << "extern const uint8_t __" << symbol_name << "_data[] = {\n";
    outfile << "    ";
	
	// And now parse all bytes as fast as possible, this part is computationally intensive
    size_t chunk_size = 1024 * 64;  // 64kb chunks
    std::string outbuffer;
    outbuffer.resize(chunk_size * 5 + (chunk_size / 20 * 5) + 2); // Calculate the required buffer size
                                                           // 5 bytes per byte + 5 bytes every 20 bytes + 2 (\0)
	
	infile.read_in_chunks(chunk_size, [&] (std::string_view chunk) {
        // Parse each chunk of the file
        size_t index = 0;
        for (int i = 0; i < chunk.size(); i++) {
			index += write_hex_comma(outbuffer.data() + index, chunk[i]);

            if (i % 20 == 19) {
                strcpy(outbuffer.data() + index, "\n    ");
                index += 5;
            }
        }
        outfile << std::string(outbuffer.data(), index);
    });
	
    outfile << "\n};\n\n} // namespace Battery::Embed\n";


    // Open output file (.h)
    fs::ofstream header(header_file, fs::Mode::TEXT);
    if (!header.is_open()) return { ErrorCode::OUTPUT_HEADER_FILE_FAILED, header_file };

    // Generate the header file
    header << "// File generated using battery_embed. https://github.com/HerrNamenlos123/Battery\n";
    header << "// Embedded file: " << fs::path(input_file).filename().string() << " | filesize: "
           << filesize << " bytes | was embedded as binary: " << (binary ? "true" : "false") << "\n";
    header << "// Source file containing the data: " + infile_name + ".cpp\n";
    header << "// DO NOT EDIT THIS FILE!!!\n";
    header << "\n#ifndef __battery_embed_" + symbol_name + "_\n";
    header << "#define __battery_embed_" + symbol_name + "_\n\n";
    header << "#include <cinttypes>\n";
    header << "#include <iostream>\n";
    header << "#include <string>\n";
    header << "#include <vector>\n\n";
    header << "namespace resources {\n\n";
    header << "    namespace internal {\n\n";
    header << "        extern const size_t __" << symbol_name << "_size;\n";
    header << "        extern const uint8_t __" << symbol_name << "_data[];\n\n";
    header << "        class " + symbol_name + "_t {\n";
    header << "        public:\n";
    header << "            " + symbol_name + "_t() = default;\n";
    header << "            static std::string str() {\n";
    header << "                return { reinterpret_cast<const char*>(__" + symbol_name + "_data), __" + symbol_name + "_size };\n";
    header << "            }\n";
    header << "            operator std::string() { return str(); }\n";
    header << "            \n";
    header << "            static std::vector<uint8_t> vec() {\n";
    header << "                return { __" + symbol_name + "_data, __" + symbol_name + "_data + __" + symbol_name + "_size };\n";
    header << "            }\n";
    header << "            operator std::vector<uint8_t>() { return vec(); }\n\n";
    header << "            size_t size() {\n";
    header << "                return str().size();\n";
    header << "            }\n";
    header << "        };\n";
    header << "        inline std::ostream& operator<<(std::ostream& os, const " + symbol_name + "_t& data) { os << data.str(); return os; }\n";
    header << "    };\n\n";
    header << "    inline internal::" + symbol_name + "_t " + symbol_name + ";\n";
    header << "}\n";
    header << "#endif // __battery_embed_" + symbol_name + "_\n";

    return { ErrorCode::SUCCESS, "" };
}

int battery::main(const Args_t& args) {
    
    CLI::App app{"Utility for converting Text and Binary files to C++ source files.\n"
                 "This is part of the Battery framework: https://github.com/HerrNamenlos123/Battery\n"};

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
	
    // Parse the CLI options
    CLI11_PARSE(app, args.size(), args_to_cstr(args));

    if (header_file_output_directory.empty()) {
        header_file_output_directory = source_file_output_directory;
    }

    auto [status, error] = convert(input_file, source_file_output_directory, header_file_output_directory, 
                                symbol_name, binary, print_header_name_only, print_src_name_only);
    switch (status) {

        case ErrorCode::SUCCESS: 
            break;  // Success

        case ErrorCode::INPUT_FILE_FAILED:
            log::error("Failed to open input file for reading (error code {}): {}", (int)status, error);
            break;

        case ErrorCode::OUTPUT_HEADER_FILE_FAILED:
            log::error("Failed to open header output file for writing (error code {}): {}", (int)status, error);
            break;

        case ErrorCode::OUTPUT_FILE_FAILED:
            log::error("Failed to open source output file for writing (error code {}): {}", (int)status, error);
            break;

        default:
            log::error("Unknown error (error code {}): {}", (int)status, error);
            break;
    }
    return (int)status;
}

int main(int argc, const char** argv) {
    //battery::main();
}