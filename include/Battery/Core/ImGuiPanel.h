#pragma once

#define DEFAULT_IMGUI_PANEL_FLAGS \
	ImGuiWindowFlags_NoTitleBar | \
	ImGuiWindowFlags_NoMove | \
	ImGuiWindowFlags_NoResize | \
	ImGuiWindowFlags_NoCollapse | \
	ImGuiWindowFlags_NoBringToFrontOnFocus

namespace Battery {

	class ImGuiPanel {
	public:
		std::string title = "panel";
		glm::vec2 position;
		glm::vec2 size;
		bool mouseOnPanel = false;

		ImGuiPanel(std::function<void(void)> updateCallback, std::function<void(void)> renderCallback, ImGuiWindowFlags flags = DEFAULT_IMGUI_PANEL_FLAGS) {
			this->updateCallback = updateCallback;
			this->renderCallback = renderCallback;
			this->flags = flags;
			id = __id__;
			__id__++;
		}

		void update() {
			_hideWindow = false;
			if (updateCallback)
				updateCallback();
		}

		/// <summary>
		/// Render the ImGui panel.
		/// </summary>
		/// <param name="...args"> - Arbitrary number of parameters specified in the template</param>
		/// <param name="flags"> - Optional flags per render call, when applied the stored flags are ignored</param>
		void render(std::optional<ImGuiWindowFlags_> flags = std::nullopt) {

			if (_hideWindow)
				return;

			ImGui::PushID(id);

			ImGui::SetNextWindowPos({ position.x, position.y });
			ImGui::SetNextWindowSize({ size.x, size.y });
			ImGui::GetStyle().WindowRounding = 0.0f;
			ImGui::Begin(title.c_str(), nullptr, flags.has_value() ? flags.value() : this->flags);

			if (renderCallback)
				renderCallback();

			// Check if the mouse is hovering the panel, for the next frame to be consistent through update/render call
			mouseOnPanel =	ImGui::GetMousePos().x >= ImGui::GetWindowPos().x &&
							ImGui::GetMousePos().x <= ImGui::GetWindowPos().x + ImGui::GetWindowSize().x &&
							ImGui::GetMousePos().y >= ImGui::GetWindowPos().y &&
							ImGui::GetMousePos().y <= ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;

			ImGui::End();

			ImGui::PopID();
		}

		/// <summary>
		/// Don't show this ImGui window. Must be called every frame in OnUpdate()
		/// </summary>
		void hideWindow() {
			_hideWindow = true;
		}

	private:
		ImGuiWindowFlags flags;
		bool _hideWindow = false;

		inline static size_t __id__ = 0;
		size_t id = 0;

		std::function<void(void)> updateCallback;
		std::function<void(void)> renderCallback;
	};

}
