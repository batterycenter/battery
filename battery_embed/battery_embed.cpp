
#include "battery/core/all.hpp"
#include <regex>

// TODO: Print version (add option)
// TODO: Create cache file besides executable, that remembers the hash of a file and does not overwrite if not necessary
// TODO: Support file encryption while embedding, for executable reverse-engineering security
// TODO: Support nested directories in the resource folder
// TODO: Move the generated function definitions from the header to the source files

enum class ErrorCode {
    SUCCESS,
    INPUT_FILE_FAILED,
    OUTPUT_FILE_FAILED,
    OUTPUT_HEADER_FILE_FAILED,
    INVALID_ARGUMENTS
};

// Using lookup tables for converting ints to hex strings, for best performance
size_t write_hex_comma(char* buf, uint8_t num) {
    static std::string lut =
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

std::string sanitizeVariableName(std::u8string symbol) {
    symbol = b::replace(symbol, u8"Ä", u8"ae");
    symbol = b::replace(symbol, u8"ä", u8"ae");
    symbol = b::replace(symbol, u8"Ö", u8"oe");
    symbol = b::replace(symbol, u8"ö", u8"oe");
    symbol = b::replace(symbol, u8"Ü", u8"ue");
    symbol = b::replace(symbol, u8"ü", u8"ue");
    symbol = b::replace(symbol, u8"ß", u8"ss");
    return std::regex_replace(b::u8_as_str(symbol), std::regex("[^a-zA-Z0-9_]"), "_");
}

class BatteryEmbed {
public:
    b::fs::path m_resourcePath;
    b::fs::path m_targetPath;

    std::string m_packedSymbolName;       // Raw symbol name including subfolders
    std::string m_sanitizedSymbolName;    // Entire symbol name for use as a variable name
    std::string m_strippedSymbolName;     // Symbol name without subfolders
    std::string m_packedNamespace;

    BatteryEmbed(
            const b::fs::path& resourcePath,
            const b::fs::path& targetPath,
            const std::string& symbolName
        )
        : m_resourcePath(resourcePath),
          m_targetPath(targetPath),
          m_packedSymbolName(symbolName)
    {
        auto packedSymbol = b::fs::path(m_packedSymbolName);
        m_strippedSymbolName = sanitizeVariableName(packedSymbol.filename().u8string());
        m_sanitizedSymbolName = sanitizeVariableName(packedSymbol.u8string());

        auto parts = b::split(b::u8_as_str(packedSymbol.u8string()), "/");
        parts.pop_back();
        auto ns = b::join(parts, "::");
        if (!ns.empty()) {
            m_packedNamespace = "::" + ns;
        }
    }

    ErrorCode generateCpp(bool binary) {
        b::fs::ifstream infile(m_resourcePath, binary ? b::fs::Mode::BINARY : b::fs::Mode::TEXT);
        if (!infile.is_open()) {
            b::log::error("Failed to open input file for reading (error code {}): {}", (int)ErrorCode::INPUT_FILE_FAILED, strerror(errno));
            return ErrorCode::INPUT_FILE_FAILED;
        }

        b::fs::ofstream outfile(m_targetPath, b::fs::Mode::TEXT);
        if (!outfile.is_open()) {
            b::log::error("Failed to open source output file for writing (error code {}): {}", (int)ErrorCode::OUTPUT_FILE_FAILED, strerror(errno));
            return ErrorCode::OUTPUT_FILE_FAILED;
        }

        size_t filesize = infile.file_size();
        std::string respath = b::u8_as_str(m_resourcePath.u8string());

        // Generate the source file
        outfile << fmt::format("// File generated using battery_embed. https://github.com/batterycenter/battery") << std::endl;
        outfile << fmt::format("// Embedded file: {} | filesize: {} bytes | was embedded as binary: {}", respath, filesize, binary ? "true" : "false") << std::endl;
        outfile << fmt::format("// Header file containing the declaration for retrieving the data: {}.hpp", m_packedSymbolName) << std::endl;
        outfile << fmt::format("// DO NOT EDIT THIS FILE!!!") << std::endl;
        outfile << std::endl;
        outfile << fmt::format("#include <cinttypes>") << std::endl;
        outfile << fmt::format("#include <cstddef>") << std::endl;
        outfile << std::endl;
        outfile << fmt::format("namespace resources_internal {{") << std::endl;
        outfile << std::endl;
        outfile << fmt::format("#ifndef BATTERY_PRODUCTION_MODE") << std::endl;
        outfile << fmt::format("    extern const char* __{}_filepath = \"{}\";", m_sanitizedSymbolName, respath) << std::endl;
        outfile << fmt::format("    extern const bool __{}_is_binary = {};", m_sanitizedSymbolName, binary ? "true" : "false") << std::endl;
        outfile << fmt::format("#endif") << std::endl;
        outfile << std::endl;
        outfile << fmt::format("    extern const size_t __{}_size = {};", m_sanitizedSymbolName, filesize) << std::endl;
        outfile << fmt::format("    extern const uint8_t __{}_data[] = {{", m_sanitizedSymbolName) << std::endl;
        outfile << fmt::format("    ");

        // And now parse all bytes as fast as possible, this part is computationally intensive
        size_t chunk_size = 1024 * 64;  // 64kb chunks
        std::string outbuffer;
        outbuffer.resize(chunk_size * 5 + (chunk_size / 20 * 5) + 2); // Calculate the required buffer size
        // 5 bytes per byte + 5 bytes every 20 bytes + 2 (\0)

        infile.read_in_chunks(chunk_size, [&outbuffer,&outfile] (std::string_view chunk) {
            // Parse each chunk of the file
            size_t index = 0;
            for (size_t i = 0; i < chunk.size(); i++) {
                index += write_hex_comma(outbuffer.data() + index, chunk[i]);

                if (i % 20 == 19) {
                    strcpy(outbuffer.data() + index, "\n    ");
                    index += 5;
                }
            }
            outfile << std::string(outbuffer.data(), index);
        });

        outfile << "\n    };\n\n} // namespace resources_internal\n";
        return ErrorCode::SUCCESS;
    }

    ErrorCode generateHpp(bool binary) {
        b::fs::ifstream infile(m_resourcePath, binary ? b::fs::Mode::BINARY : b::fs::Mode::TEXT);
        if (!infile.is_open()) {
            b::log::error("Failed to open input file for reading (error code {}): {}", (int)ErrorCode::INPUT_FILE_FAILED, strerror(errno));
            return ErrorCode::INPUT_FILE_FAILED;
        }

        b::fs::ofstream file(m_targetPath, b::fs::Mode::TEXT);
        if (!file.is_open()) {
            b::log::error("Failed to open header output file for writing (error code {}): {}", (int)ErrorCode::OUTPUT_HEADER_FILE_FAILED, strerror(errno));
            return ErrorCode::OUTPUT_HEADER_FILE_FAILED;
        }

        size_t filesize = infile.file_size();
        std::string respath = b::u8_as_str(m_resourcePath.filename().u8string());

        // Generate the header file
        file << fmt::format("// File generated using battery_embed. https://github.com/batterycenter/battery") << std::endl;
        file << fmt::format("// Embedded file: {} | filesize: {} bytes | was embedded as binary: {}", respath, filesize, binary ? "true" : "false") << std::endl;
        file << fmt::format("// Source file containing the data: {}.cpp", m_packedSymbolName) << std::endl;
        file << fmt::format("// DO NOT EDIT THIS FILE!!!") << std::endl;
        file << std::endl;
        file << fmt::format("#ifndef __battery_embed_{}_", m_sanitizedSymbolName) << std::endl;
        file << fmt::format("#define __battery_embed_{}_", m_sanitizedSymbolName) << std::endl;
        file << std::endl;
        file << fmt::format("#include <cinttypes>") << std::endl;
        file << fmt::format("#include <iostream>") << std::endl;
        file << fmt::format("#include <string>") << std::endl;
        file << fmt::format("#include <vector>") << std::endl;
        file << fmt::format("#include \"battery/core/resource.hpp\"") << std::endl;
        file << fmt::format("#include \"battery/core/fs.hpp\"") << std::endl;
        file << std::endl;
        file << fmt::format("namespace resources_internal {{", m_packedNamespace) << std::endl;
        file << std::endl;
        file << fmt::format("#ifndef BATTERY_PRODUCTION_MODE") << std::endl;
        file << fmt::format("    extern const char* __{}_filepath;", m_sanitizedSymbolName) << std::endl;
        file << fmt::format("    extern const bool __{}_is_binary;", m_sanitizedSymbolName) << std::endl;
        file << fmt::format("#endif") << std::endl;
        file << std::endl;
        file << fmt::format("    extern const size_t __{}_size;", m_sanitizedSymbolName) << std::endl;
        file << fmt::format("    extern const uint8_t __{}_data[];", m_sanitizedSymbolName) << std::endl;
        file << std::endl;
        file << fmt::format("    class {}_t {{", m_sanitizedSymbolName) << std::endl;
        file << fmt::format("    public:") << std::endl;
        file << fmt::format("        {}_t() = default;", m_sanitizedSymbolName) << std::endl;
        file << std::endl;
        file << fmt::format("        inline static std::string str() {{") << std::endl;
        file << fmt::format("            return {{ reinterpret_cast<const char*>(__{}_data), __{}_size }};", m_sanitizedSymbolName, m_sanitizedSymbolName) << std::endl;
        file << fmt::format("        }}") << std::endl;
        file << std::endl;
        file << fmt::format("        inline static const char* data() {{") << std::endl;
        file << fmt::format("            return reinterpret_cast<const char*>(__{}_data);", m_sanitizedSymbolName) << std::endl;
        file << fmt::format("        }}") << std::endl;
        file << std::endl;
        file << fmt::format("        inline operator std::string() {{") << std::endl;
        file << fmt::format("            return str();") << std::endl;
        file << fmt::format("        }}") << std::endl;
        file << std::endl;
        file << fmt::format("        inline static std::vector<uint8_t> vec() {{") << std::endl;
        file << fmt::format("            return {{ __{}_data, __{}_data + __{}_size }};", m_sanitizedSymbolName, m_sanitizedSymbolName, m_sanitizedSymbolName) << std::endl;
        file << fmt::format("        }}") << std::endl;
        file << std::endl;
        file << fmt::format("        inline operator std::vector<uint8_t>() {{") << std::endl;
        file << fmt::format("            return vec();") << std::endl;
        file << fmt::format("        }}") << std::endl;
        file << std::endl;
        file << fmt::format("#ifndef BATTERY_PRODUCTION_MODE") << std::endl;
        file << fmt::format("        inline static b::fs::path filepath() {{") << std::endl;
        file << fmt::format("            return __{}_filepath;", m_sanitizedSymbolName) << std::endl;
        file << fmt::format("        }}") << std::endl;
        file << std::endl;
        file << fmt::format("        inline static bool is_binary() {{") << std::endl;
        file << fmt::format("            return __{}_is_binary;", m_sanitizedSymbolName) << std::endl;
        file << fmt::format("        }}") << std::endl;
        file << fmt::format("#endif") << std::endl;
        file << std::endl;
        file << fmt::format("        inline operator b::resource() {{") << std::endl;
        file << fmt::format("            return b::resource::from_byte_string(str());") << std::endl;
        file << fmt::format("        }}") << std::endl;
        file << std::endl;
        file << fmt::format("        inline size_t size() {{") << std::endl;
        file << fmt::format("            return str().size();") << std::endl;
        file << fmt::format("        }}") << std::endl;
        file << fmt::format("    }};") << std::endl;
        file << fmt::format("    inline std::ostream& operator<<(std::ostream& os, const {}_t& data) {{ os << data.str(); return os; }}\n", m_sanitizedSymbolName) << std::endl;
        file << fmt::format("}}") << std::endl;
        file << std::endl;
        file << fmt::format("namespace resources{} {{", m_packedNamespace) << std::endl;
        file << fmt::format("    inline resources_internal::{}_t {};", m_sanitizedSymbolName, m_strippedSymbolName) << std::endl;
        file << fmt::format("}}") << std::endl;
        file << std::endl;
        file << fmt::format("#endif // __battery_embed_{}_", m_sanitizedSymbolName) << std::endl;

        return ErrorCode::SUCCESS;
    }
};

int b::main(const std::vector<std::u8string>& args) {             // NOLINT NOSONAR
    
    CLI::App app{"Utility for converting Text and Binary files to C++ source files.\n"
                 "This is part of the battery toolbox: https://github.com/batterycenter/battery\n"};

    std::string resourcePath;
    app.add_option("resource file", resourcePath, "The file to be embedded")->required()->check(CLI::ExistingFile);

    std::string targetPath;
    app.add_option("c++ target file", targetPath, "The C++ target source file or header file (depending on --header and --cpp)")->required();

    std::string symbolName;
    app.add_option("--symbol_name", symbolName, "Override the symbol name for the output files");

    bool binary = false;
    app.add_flag("--binary", binary, "File is in binary format instead of plain text (regarding line endings)");

    bool generate_hpp = false;
    app.add_flag("--header", generate_hpp, "Generate the header file for the embedded resource");

    bool generate_cpp = false;
    app.add_flag("--cpp", generate_cpp, "Generate the cpp file for the embedded resource");

    // Parse the CLI options
    CLI11_PARSE(app, args.size(), b::args_to_argv(args));

    BatteryEmbed embedder(
            resourcePath,
            targetPath,
            symbolName
    );

    if (generate_hpp && !generate_cpp) {
        return (int)embedder.generateHpp(binary);
    } else if (generate_cpp && !generate_hpp) {
        return (int)embedder.generateCpp(binary);
    } else {
        b::log::error("Error {} (INVALID_ARGUMENTS): You must specify exactly one of --header or --cpp!", (int)ErrorCode::INVALID_ARGUMENTS);
        return (int)ErrorCode::INVALID_ARGUMENTS;
    }
}
