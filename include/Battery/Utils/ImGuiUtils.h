#pragma once

#include "Battery/pch.h"

namespace Battery {
	namespace ImGuiUtils {

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
		void LoadDefaultColorScheme();
		void LoadBatteryColorScheme();

		void DisableIniFile();
		
		std::string EmbedTTFAsHeaderFileDirect(const std::string& ttfBytes, const std::string& symbolName, bool compress = true);
		bool EmbedTTFAsHeaderFile(const std::string& inputFile, const std::string& outputFile, const std::string& symbolName, bool compress = true);
		ImFont* AddEmbeddedFont(const unsigned int* data, unsigned int length, float pixels);
		ImFont* AddEmbeddedFontUncompressed(const unsigned int* data, unsigned int length, float pixels);

		extern ImGuiColors defaultColorScheme;		// Global variable

	}
}
