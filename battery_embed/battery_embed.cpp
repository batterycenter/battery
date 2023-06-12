
#include "battery/core.hpp"
#include <regex>
#include <utility>

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
size_t WriteHexComma(char* buf, uint8_t num) {
    static b::string lut =
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

    // NOLINTBEGIN
    buf[0] = '0';
    buf[1] = 'x';
    buf[2] = lut[(num & 0xFF) * 2];
    buf[3] = lut[(num & 0xFF) * 2 + 1];
    buf[4] = ',';
    // NOLINTEND
    return 5;
}

b::string SanitizeVariableName(b::string symbol) {
    symbol = b::string::replace(symbol, "Ä", "ae");
    symbol = b::string::replace(symbol, "ä", "ae");
    symbol = b::string::replace(symbol, "Ö", "oe");
    symbol = b::string::replace(symbol, "ö", "oe");
    symbol = b::string::replace(symbol, "Ü", "ue");
    symbol = b::string::replace(symbol, "ü", "ue");
    symbol = b::string::replace(symbol, "ß", "ss");
    return std::regex_replace(symbol, std::regex("[^a-zA-Z0-9_]"), "_");
}

class BatteryEmbed {
public:
    b::fs::path m_resourcePath;
    b::fs::path m_targetPath;

    b::string m_packedSymbolName;       // Raw symbol name including subfolders
    b::string m_sanitizedSymbolName;    // Entire symbol name for use as a variable name
    b::string m_strippedSymbolName;     // Symbol name without subfolders
    b::string m_packedNamespace;

    BatteryEmbed(
            const b::fs::path& resourcePath,
            const b::fs::path& targetPath,
            const b::string& symbolName
        )
        : m_resourcePath(resourcePath),
          m_targetPath(targetPath),
          m_packedSymbolName(symbolName)
    {
        auto packedSymbol = b::fs::path(m_packedSymbolName);
        m_strippedSymbolName = SanitizeVariableName(packedSymbol.filename().u8string());
        m_sanitizedSymbolName = SanitizeVariableName(packedSymbol.u8string());

        auto parts = b::string::split(packedSymbol, "/");
        parts.pop_back();
        auto nameSpace = b::string::join(parts, "::");
        if (!nameSpace.empty()) {
            m_packedNamespace = "::" + nameSpace;
        }
    }

    ErrorCode generateCpp(bool binary) {
        b::fs::ofstream outfile(m_targetPath, b::fs::Mode::TEXT);
        if (outfile.fail()) {
            b::log::error("Failed to open source output file '{}' for writing (error code {}): {}", m_targetPath, static_cast<int>(ErrorCode::OUTPUT_FILE_FAILED), b::strerror(errno));
            return ErrorCode::OUTPUT_FILE_FAILED;
        }

        if (!b::fs::exists(m_resourcePath)) {
            b::log::error("Failed to open input file '{}' for reading (error code {}): No such file or directory", m_resourcePath, static_cast<int>(ErrorCode::INPUT_FILE_FAILED));
            return ErrorCode::INPUT_FILE_FAILED;
        }

        size_t filesize = 0;
        try {
            filesize = binary ? b::fs::read_binary_file(m_resourcePath).str().length() : b::fs::read_text_file(m_resourcePath).str().length();
        }
        catch (const std::exception& e) {
            b::log::error("Failed to open input file '{}' for reading (error code {}): {}", m_resourcePath, static_cast<int>(ErrorCode::INPUT_FILE_FAILED), e.what());
            return ErrorCode::INPUT_FILE_FAILED;
        }

        // Generate the source file
        outfile << b::format("// File generated using battery_embed. https://github.com/batterycenter/battery") << std::endl;
        outfile << b::format("// Embedded file: {} | filesize: {} bytes | was embedded as binary: {}", m_resourcePath, filesize, binary ? "true" : "false") << std::endl;
        outfile << b::format("// Header file containing the declaration for retrieving the data: {}.hpp", m_packedSymbolName) << std::endl;
        outfile << b::format("// DO NOT EDIT THIS FILE!!!") << std::endl;
        outfile << std::endl;
        outfile << b::format("#include <cinttypes>") << std::endl;
        outfile << b::format("#include <cstddef>") << std::endl;
        outfile << std::endl;
        outfile << b::format("namespace ResourcesInternal {{") << std::endl;
        outfile << std::endl;
        outfile << b::format("#ifndef B_PRODUCTION_MODE") << std::endl;
        outfile << b::format("    extern const char* const RESOURCE_{}_FILEPATH = \"{}\";", m_sanitizedSymbolName, m_resourcePath) << std::endl;
        outfile << b::format("    extern const bool RESOURCE_{}_IS_BINARY = {};", m_sanitizedSymbolName, binary ? "true" : "false") << std::endl;
        outfile << b::format("#endif") << std::endl;
        outfile << std::endl;
        outfile << b::format("    extern const size_t RESOURCE_{}_SIZE = {};", m_sanitizedSymbolName, filesize) << std::endl;
        outfile << b::format("    extern const uint8_t RESOURCE_{}_DATA[] = {{ // NOLINT", m_sanitizedSymbolName) << std::endl;
        outfile << b::format("    ");

        // And now parse all bytes as fast as possible, this part is computationally intensive
        auto chunkSize = static_cast<size_t>(1024 * 64);  // 64kb chunks
        b::string outbuffer;
        outbuffer.resize(chunkSize * 5 + (chunkSize / 20 * 5) + 2); // Calculate the required buffer size
        // 5 bytes per byte + 5 bytes every 20 bytes + 2 (\0)

        auto callback = [&outbuffer,&outfile] (std::string_view chunk) {
            // Parse each chunk of the file
            size_t index = 0;
            for (size_t i = 0; i < chunk.size(); i++) {
                index += WriteHexComma(outbuffer.data() + index, chunk[i]);   // NOLINT

                if (i % 20 == 19) {
                    strcpy(outbuffer.data() + index, "\n    "); // NOLINT
                    index += 5;
                }
            }
            outfile << std::string(outbuffer.data(), index);
        };

        size_t totalBytes = 0;
        if (binary) {
            totalBytes = b::fs::read_binary_file_in_chunks_nothrow(m_resourcePath, chunkSize, callback);
        }
        else {
            totalBytes = b::fs::read_text_file_in_chunks_nothrow(m_resourcePath, chunkSize, callback);
        }

        if (totalBytes == 0) {  // File is empty: A C array must have at least one element. We add a zero byte to it,
            callback(std::string_view("\0", 1));     // although it will not be read anyway as the size is 0 too.
        }

        if (totalBytes == static_cast<size_t>(-1)) {
            b::log::error("Failed to open input file '{}' for reading (error code {}): {}", m_resourcePath, static_cast<int>(ErrorCode::INPUT_FILE_FAILED), b::strerror(errno));
            return ErrorCode::INPUT_FILE_FAILED;
        }

        outfile << "\n    };\n\n} // namespace ResourcesInternal\n";
        return ErrorCode::SUCCESS;
    }

    ErrorCode generateHpp(bool binary) {
        if (!b::fs::exists(m_resourcePath)) {
            b::log::error("Failed to open input file '{}' for reading (error code {}): No such file or directory", m_resourcePath, static_cast<int>(ErrorCode::INPUT_FILE_FAILED));
            return ErrorCode::INPUT_FILE_FAILED;
        }

        b::fs::ofstream file(m_targetPath, b::fs::Mode::TEXT);
        if (!file.is_open()) {
            b::log::error("Failed to open header output file '{}' for writing (error code {}): {}", m_targetPath, static_cast<int>(ErrorCode::OUTPUT_HEADER_FILE_FAILED), b::strerror(errno));
            return ErrorCode::OUTPUT_HEADER_FILE_FAILED;
        }

        size_t filesize = 0;
        try {
            filesize = binary ? b::fs::read_binary_file(m_resourcePath).str().length() : b::fs::read_text_file(m_resourcePath).str().length();
        }
        catch (const std::exception& e) {
            b::log::error("Failed to open input file '{}' for reading (error code {}): {}", m_resourcePath, static_cast<int>(ErrorCode::INPUT_FILE_FAILED), e.what());
            return ErrorCode::INPUT_FILE_FAILED;
        }

        // Generate the header file
        file << b::format("// File generated using battery_embed. https://github.com/batterycenter/battery") << std::endl;
        file << b::format("// Embedded file: {} | filesize: {} bytes | was embedded as binary: {}", m_resourcePath, filesize, binary ? "true" : "false") << std::endl;
        file << b::format("// Source file containing the data: {}.cpp", m_packedSymbolName) << std::endl;
        file << b::format("// DO NOT EDIT THIS FILE!!!") << std::endl;
        file << std::endl;
        file << b::format("#ifndef __battery_embed_{}_", m_sanitizedSymbolName) << std::endl;
        file << b::format("#define __battery_embed_{}_", m_sanitizedSymbolName) << std::endl;
        file << std::endl;
        file << b::format("#include <cinttypes>") << std::endl;
        file << b::format("#include <iostream>") << std::endl;
        file << b::format("#include <vector>") << std::endl;
        file << b::format("#include \"battery/core/resource.hpp\"") << std::endl;
        file << b::format("#include \"battery/core/string.hpp\"") << std::endl;
        file << b::format("#include \"battery/core/fs.hpp\"") << std::endl;
        file << std::endl;
        file << b::format("namespace ResourcesInternal {{", m_packedNamespace) << std::endl;
        file << std::endl;
        file << b::format("#ifndef B_PRODUCTION_MODE") << std::endl;
        file << b::format("    extern const char* const RESOURCE_{}_FILEPATH;", m_sanitizedSymbolName) << std::endl;
        file << b::format("    extern const bool RESOURCE_{}_IS_BINARY;", m_sanitizedSymbolName) << std::endl;
        file << b::format("#endif") << std::endl;
        file << std::endl;
        file << b::format("    extern const size_t RESOURCE_{}_SIZE;", m_sanitizedSymbolName) << std::endl;
        file << b::format("    extern const uint8_t RESOURCE_{}_DATA[];", m_sanitizedSymbolName) << std::endl;
        file << std::endl;
        file << b::format("    class {}_t {{", m_sanitizedSymbolName) << std::endl;
        file << b::format("    public:") << std::endl;
        file << b::format("        {}_t() = default;", m_sanitizedSymbolName) << std::endl;
        file << std::endl;
        file << b::format("        inline b::string string() const {{") << std::endl;
        file << b::format("            return std::string({{ reinterpret_cast<const char*>(RESOURCE_{}_DATA), RESOURCE_{}_SIZE }});", m_sanitizedSymbolName, m_sanitizedSymbolName) << std::endl;
        file << b::format("        }}") << std::endl;
        file << std::endl;
        file << b::format("        inline const char* data() const {{") << std::endl;
        file << b::format("            return reinterpret_cast<const char*>(RESOURCE_{}_DATA);", m_sanitizedSymbolName) << std::endl;
        file << b::format("        }}") << std::endl;
        file << std::endl;
        file << b::format("        inline operator b::string() {{") << std::endl;
        file << b::format("            return string();") << std::endl;
        file << b::format("        }}") << std::endl;
        file << std::endl;
        file << b::format("        inline std::vector<uint8_t> vec() const {{") << std::endl;
        file << b::format("            return {{ RESOURCE_{}_DATA, RESOURCE_{}_DATA + RESOURCE_{}_SIZE }};", m_sanitizedSymbolName, m_sanitizedSymbolName, m_sanitizedSymbolName) << std::endl;
        file << b::format("        }}") << std::endl;
        file << std::endl;
        file << b::format("        inline operator std::vector<uint8_t>() {{") << std::endl;
        file << b::format("            return vec();") << std::endl;
        file << b::format("        }}") << std::endl;
        file << std::endl;
        file << b::format("#ifndef B_PRODUCTION_MODE") << std::endl;
        file << b::format("        inline b::fs::path filepath() const {{") << std::endl;
        file << b::format("            return RESOURCE_{}_FILEPATH;", m_sanitizedSymbolName) << std::endl;
        file << b::format("        }}") << std::endl;
        file << std::endl;
        file << b::format("        inline bool isBinary() const {{") << std::endl;
        file << b::format("            return RESOURCE_{}_IS_BINARY;", m_sanitizedSymbolName) << std::endl;
        file << b::format("        }}") << std::endl;
        file << b::format("#endif") << std::endl;
        file << std::endl;
        file << b::format("        inline operator b::Resource() {{") << std::endl;
        file << b::format("            return b::Resource::FromByteString(string());") << std::endl;
        file << b::format("        }}") << std::endl;
        file << std::endl;
        file << b::format("        inline size_t size() const {{") << std::endl;
        file << b::format("            return string().size();") << std::endl;
        file << b::format("        }}") << std::endl;
        file << b::format("    }};") << std::endl;
        file << b::format("    inline std::ostream& operator<<(std::ostream& os, const {}_t& data) {{ os << data.string(); return os; }}\n", m_sanitizedSymbolName) << std::endl;
        file << b::format("}} // namespace ResourcesInternal") << std::endl;
        file << std::endl;
        file << b::format("namespace Resources{} {{", m_packedNamespace) << std::endl;
        file << b::format("    inline ResourcesInternal::{}_t {};", m_sanitizedSymbolName, m_strippedSymbolName) << std::endl;
        file << b::format("}}") << std::endl;
        file << std::endl;
        file << b::format("#endif // __battery_embed_{}_", m_sanitizedSymbolName) << std::endl;

        return ErrorCode::SUCCESS;
    }
};

int b::main(const std::vector<b::string>& args) {             // NOLINT NOSONAR
    
    CLI::App app{"Utility for converting Text and Binary files to C++ source files.\n"
                 "This is part of the battery toolbox: https://github.com/batterycenter/battery\n"};

    b::string resourcePath;
    app.add_option("resource file", resourcePath, "The file to be embedded")->required()->check(CLI::ExistingFile);

    b::string targetPath;
    app.add_option("c++ target file", targetPath, "The C++ target source file or header file (depending on --header and --cpp)")->required();

    b::string symbolName;
    app.add_option("--symbol-name", symbolName, "Override the symbol name for the output files")->required();

    bool binary = false;
    app.add_flag("--binary", binary, "File is in binary format instead of plain text (regarding line endings)");

    bool generateHpp = false;
    app.add_flag("--header", generateHpp, "Generate the header file for the embedded resource");

    bool generateCpp = false;
    app.add_flag("--cpp", generateCpp, "Generate the cpp file for the embedded resource");

    // Parse the CLI options
    CLI11_PARSE(app, args.size(), b::args_to_argv(args));

    BatteryEmbed embedder(
            resourcePath,
            targetPath,
            symbolName
    );

    if (generateHpp && !generateCpp) {
        return static_cast<int>(embedder.generateHpp(binary));
    }
    if (generateCpp && !generateHpp) {
        return static_cast<int>(embedder.generateCpp(binary));
    }

    b::log::error("Error {} (INVALID_ARGUMENTS): You must specify exactly one of --header or --cpp!", static_cast<int>(ErrorCode::INVALID_ARGUMENTS));
    return static_cast<int>(ErrorCode::INVALID_ARGUMENTS);
}
