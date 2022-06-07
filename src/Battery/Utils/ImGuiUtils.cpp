
#include "Battery/Core/Log.h"
#include "Battery/Core/Exception.h"
#include "Battery/Utils/ImGuiUtils.h"
#include "Battery/Utils/FileUtils.h"

#include "Battery/Extern/magic_enum.hpp"

// Defined in Battery/Utils/ImGuiCompressor.cpp
extern std::string binary_to_compressed_c(const std::string& file, const std::string& symbol, bool use_base85_encoding, bool use_compression);

namespace Battery {

	Battery::ImGuiColors defaultColorScheme;

	ImGuiColors GetColorScheme() {
		ImGuiColors scheme;

		for (size_t i = 0; i < ImGuiCol_COUNT; i++) {
			std::string name = magic_enum::enum_name((ImGuiCol_)i).data();
			scheme.colors.push_back({ name, ImGui::GetStyle().Colors[i] });
		}

		return scheme;
	}

	void LoadColorScheme(const ImGuiColors& scheme) {
		ImVec4* colors = ImGui::GetStyle().Colors;

		for (size_t i = 0; i < scheme.size() && i < ImGuiCol_COUNT; i++) {
			colors[i] = scheme.colors[i].color;
		}
	}

	void LoadDefaultColorScheme() {
		LoadColorScheme(defaultColorScheme);
	}

	void CaptureCurrentColorSchemeAsDefault() {
		defaultColorScheme = GetColorScheme();
	}

	void LoadBatteryColorScheme() {
        auto colors = ImGui::GetStyle().Colors;

        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.53f, 0.53f, 0.53f, 0.48f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.80f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.63f, 0.63f, 0.63f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 1.00f, 0.00f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.55f, 0.55f, 0.55f, 0.38f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.80f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.63f, 0.63f, 0.63f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.55f, 0.55f, 0.55f, 0.38f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.63f, 0.63f, 0.63f, 1.00f);
        colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
        colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
        colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
        colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.63f, 0.63f, 0.63f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	void DisableIniFile() {
        ImGui::GetIO().IniFilename = NULL;
	}

    std::string EmbedTTFAsHeaderFileDirect(const std::string& ttfBytes, const std::string& symbolName, bool compress) {
        return binary_to_compressed_c(ttfBytes, symbolName, false, compress);
    }

    bool EmbedTTFAsHeaderFile(const std::string& inputFile, const std::string& outputFile, const std::string& symbolName, bool compress) {
        
        File ttfBytes = ReadFile(inputFile, true);
        if (ttfBytes.fail())
            return false;

        return WriteFile(outputFile, EmbedTTFAsHeaderFileDirect(ttfBytes.content(), symbolName, compress));
    }

    ImFont* AddEmbeddedFont(const unsigned int* data, unsigned int length, float pixels, const ImWchar* glyphRange) {
        return ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(data, length, pixels, nullptr, glyphRange);
    }

    ImFont* AddEmbeddedFontUncompressed(const unsigned int* data, unsigned int length, float pixels, const ImWchar* glyphRange) {
        void* temp = new uint8_t[length];   // This buffer is tranferred to ImGui and then deleted by ImGui
        memcpy(temp, data, length);         // cfg.FontDataOwnedByAtlas is not used to avoid data lifetime issues
        return ImGui::GetIO().Fonts->AddFontFromMemoryTTF(temp, length, pixels, nullptr, glyphRange);
    }







    size_t DropdownMenu::Draw() {

		ImGui::PushID("BatteryDropdownMenu");

		if (selected >= items.size())
			selected = items.size() - 1;

        if (name.length() == 0)
            name += "##";

		ImGui::PushID(specificID);
        const char* label = (items.size() == 0) ? "" : items[selected].c_str();
		if (ImGui::BeginCombo(name.c_str(), label)) {
			if (items.size() != 0) {
			    for (size_t i = 0; i < items.size(); i++) {
					ImGui::PushID(i);
			        bool is_selected = (i == selected);
			        if (ImGui::Selectable(items[i].c_str(), is_selected))
			            selected = i;
			        if (is_selected)
			            ImGui::SetItemDefaultFocus();
					ImGui::PopID();
			    }
			}
			ImGui::EndCombo();
		}
		ImGui::PopID();
		ImGui::PopID();

		return selected;
	}
}
