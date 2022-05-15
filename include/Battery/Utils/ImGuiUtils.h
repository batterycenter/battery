#pragma once

#include "Battery/pch.h"

#define ADD_FONT(symbolName, size) Battery::AddEmbeddedFont(symbolName ## _compressed_data, \
	symbolName ## _compressed_size, size);

#define ADD_FONT_UNCOMPRESSED(symbolName, size) Battery::AddEmbeddedFontUncompressed( \
	symbolName ## _compressed_data, symbolName ## _compressed_size, size);

namespace Battery {

	// This is a container for the ImGui ImGuiCol_ enum. It combines all entries with the values
	// and the enum name as a description. The std::vector index is the enum index.
	struct ImGuiColors {

		struct ImGuiColor {
			std::string name;
			ImVec4 color;
		};

		std::vector<ImGuiColor> colors;

		ImGuiColor& operator[](size_t index) {
			return colors[index];
		}

		size_t size() const {
			return colors.size();
		}
	};

	ImGuiColors GetColorScheme();
	void LoadColorScheme(const ImGuiColors& scheme);
	void LoadBatteryColorScheme();
	void LoadDefaultColorScheme();
	
	/// <summary>
	/// INTERNAL: Do not use this function manually.
	/// </summary>
	void CaptureCurrentColorSchemeAsDefault();

	/// <summary>
	/// Prevent ImGui from creating an 'imgui.ini' file. Disabling means that UI changes are not saved and every
	/// session starts at the default, not remembering previous changes.
	/// </summary>
	void DisableIniFile();
	
	/// <summary>
	/// Load a .ttf file and generate an embedded header file. The content of the header file is returned
	/// as a string. By including the header and loading the font with the ADD_FONT() or ADD_FONT_UNCOMPRESSED() macro,
	/// the font can be embedded in the source code.
	/// </summary>
	/// <param name="ttfBytes"></param>
	/// <param name="symbolName"> -> the name of the font, specify this in ADD_FONT(<symbolname>, size)</param>
	/// <param name="compress"></param>
	/// <returns>The content of the header file</returns>
	std::string EmbedTTFAsHeaderFileDirect(const std::string& ttfBytes, const std::string& symbolName, bool compress = true);
	
	/// <summary>
	/// Generates an embedded header file from a .ttf file, it loads, converts and writes the file.
	/// See EmbedTTFAsHeaderFileDirect for more info on what it does.
	/// </summary>
	/// <param name="inputFile"></param>
	/// <param name="outputFile"></param>
	/// <param name="symbolName"></param>
	/// <param name="compress"></param>
	/// <returns>Success</returns>
	bool EmbedTTFAsHeaderFile(const std::string& inputFile, const std::string& outputFile, const std::string& symbolName, bool compress = true);
	
	/// <summary>
	/// Load an embedded font from a compressed header, generated with ImGuiUtils::EmbedTTFAsHeaderFile().
	/// Use the ADD_FONT() macro for convenience.
	/// </summary>
	/// <param name="data"></param>
	/// <param name="length"></param>
	/// <param name="pixels"></param>
	/// <returns></returns>
	ImFont* AddEmbeddedFont(const unsigned int* data, unsigned int length, float pixels);

	/// <summary>
	/// Load an embedded font from a compressed header, generated with ImGuiUtils::EmbedTTFAsHeaderFile().
	/// Use the ADD_FONT_UNCOMPRESSED() macro for convenience.
	/// </summary>
	/// <param name="data"></param>
	/// <param name="length"></param>
	/// <param name="pixels"></param>
	/// <returns></returns>
	ImFont* AddEmbeddedFontUncompressed(const unsigned int* data, unsigned int length, float pixels);


	class DropdownMenu {
	public:

		std::vector<std::string> items;
		std::string name;

		DropdownMenu() { LOG_WARN("ID: {}", specificID); }
		DropdownMenu(const std::string& name) : name(name) { LOG_WARN("ID: {}", specificID); }

		/// <summary>
		/// Render the dropdown menu. Returns the array index of the currently selected item,
		/// or -1 if the array is empty.
		/// </summary>
		size_t Draw();

	private:
		size_t selected = 0;
		size_t specificID = objectID++;
		inline static size_t objectID = 0;		// special ID for every class
	};
}
