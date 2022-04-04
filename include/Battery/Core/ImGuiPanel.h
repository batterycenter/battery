#pragma once

#include "Battery/pch.h"

#define DEFAULT_IMGUI_PANEL_FLAGS \
	ImGuiWindowFlags_NoTitleBar | \
	ImGuiWindowFlags_NoMove | \
	ImGuiWindowFlags_NoResize | \
	ImGuiWindowFlags_NoCollapse | \
	ImGuiWindowFlags_NoBringToFrontOnFocus

namespace Battery {

	template <typename TFontContainer>
	class ImGuiLayer;
	struct FontContainer;

	Battery::ImGuiLayer<Battery::FontContainer>* __getUserInterface();

	template<typename... Targs>
	class ImGuiPanel {

		ImGuiWindowFlags flags;
		bool hideWindow = false;

	public:
		std::string title;
		glm::vec2 position;
		glm::vec2 size;
		bool mouseOnPanel = false;

		ImGuiPanel(const std::string& title, glm::vec2 position, glm::vec2 size, ImGuiWindowFlags flags = DEFAULT_IMGUI_PANEL_FLAGS) {
			this->title = title;
			this->position = position;
			this->size = size;
			this->flags = flags;
		}

		void Update(Targs... args) {
			hideWindow = false;
			OnUpdate(args...);
		}

		/// <summary>
		/// Render the 
		/// ImGui panel.
		/// </summary>
		/// <param name="...args"> - Arbitrary number of parameters specified in the template</param>
		/// <param name="flags"> - Optional flags per render call, when applied the stored flags are ignored</param>
		void Render(Targs... args, std::optional<ImGuiWindowFlags_> flags = std::nullopt) {

			if (hideWindow)
				return;

			ImGui::SetNextWindowPos(ImVec2(position.x, position.y));
			ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
			ImGui::GetStyle().WindowRounding = 0.0f;
			ImGui::Begin(title.c_str(), nullptr, flags.has_value() ? flags.value() : this->flags);

			OnRender(args...);

			// Check if the mouse is hovering the panel, for the next frame to be consistent through update/render call
			mouseOnPanel =	ImGui::GetMousePos().x >= ImGui::GetWindowPos().x &&
							ImGui::GetMousePos().x <= ImGui::GetWindowPos().x + ImGui::GetWindowSize().x &&
							ImGui::GetMousePos().y >= ImGui::GetWindowPos().y &&
							ImGui::GetMousePos().y <= ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;

			ImGui::End();
		}

		//template <typename T>
		//T* GetFontContainer() {
		//	return __getUserInterface()->GetFontContainer<T>();
		//}

		/// <summary>
		/// Don't show this ImGui window. Must be called every frame in OnUpdate()
		/// </summary>
		void HideWindow() {
			hideWindow = true;
		}

		virtual void OnUpdate(Targs... args) = 0;
		virtual void OnRender(Targs... args) = 0;
	};

}
