#pragma once

#include "Battery/pch.h"
#include "Battery/Core/Layer.h"
#include "Battery/Core/Config.h"
#include "Battery/Core/ImGuiPanel.h"
#include "Battery/Core/Application.h"
#include "Battery/Utils/ImGuiUtils.h"

struct ALLEGRO_DISPLAY;

namespace Battery {

	struct Fonts {
		inline static ImFont* defaultFont = nullptr;

		static void loadDefault() {
			LOG_ERROR("Loading default");
			defaultFont = ImGui::GetIO().Fonts->AddFontDefault();
		}
	};

	bool __acknowledgeResize(ALLEGRO_DISPLAY* display);		// TODO: Solve allegro acknowledge problem (linking)

	// This Layer can only be attached once in an application
	template<typename TFont>
	class ImGuiLayer : public Layer {

		ImGuiIO dummyIO;
		inline static bool attached = false;
		std::vector<std::shared_ptr<ImGuiPanel<>>> panels;

	public:
		ImGuiIO& io = dummyIO;

		ImGuiLayer() {

		}

		ImGuiLayer(const std::string& debugName) : Layer(debugName) {

		}

		/// <summary>
		/// Push panels onto the stack, so they're automatically updated and rendered every frame.
		/// Panels are updated/rendered in the order they are pushed. Panels with template parameters must
		/// be updated/rendered manually.
		/// </summary>
		/// <param name="panel"></param>
		void PushPanel(std::shared_ptr<ImGuiPanel<>> panel) {
			if (!panel)
				throw Battery::Exception("ImGuiLayer::PushPanel(): The panel is invalid!");

			panels.push_back(panel);
		}

		void OnAttach() final {		// Prevent from being overridden again

			if (attached)
				throw Battery::Exception("A Battery::ImGuiLayer is already attached! Only one instance is allowed in an application.");

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImPlot::CreateContext();
			io = ImGui::GetIO();
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

			// Save the default ImGui color scheme and then load the Battery color scheme
			ImGui::StyleColorsDark();
			ImGuiUtils::defaultColorScheme = ImGuiUtils::GetColorScheme();
			ImGuiUtils::LoadBatteryColorScheme();

			// Setup Platform/Renderer backends
			ImGui_ImplAllegro5_Init(Battery::GetMainWindow().allegroDisplayPointer);

			io = ImGui::GetIO();
			TFont::loadDefault();
			TFont::load();
			OnImGuiAttach();
			io.Fonts->Build();

			ImGui_ImplAllegro5_CreateDeviceObjects();

			attached = true;
		}

		void OnDetach() final {
			io = ImGui::GetIO();
			OnImGuiDetach();
			ImGui_ImplAllegro5_Shutdown();

			ImPlot::DestroyContext();
			ImGui::DestroyContext();
		}

		void OnUpdate() final {
			io = ImGui::GetIO();

			// First update all panels on the stack
			for (auto panel : panels) {
				panel->Update();
			}

			OnImGuiUpdate();
		}
		
		void OnRender() final {

			// Start ImGui Frame
			ImGui_ImplAllegro5_NewFrame();
			ImGui::NewFrame();
			
			io = ImGui::GetIO();

			// First render all panels on the stack
			for (auto panel : panels) {
				panel->Render();
			}

			OnImGuiRender();
			// Draw the result on the screen
			ImGui::Render();
			ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
		}

		void OnEvent(Battery::Event* event) final {
			ImGui_ImplAllegro5_ProcessEvent(event->GetAllegroEvent());

			if (event->GetType() == EventType::WindowResize) {
				ImGui_ImplAllegro5_InvalidateDeviceObjects();
				__acknowledgeResize(Battery::GetMainWindow().allegroDisplayPointer);
				ImGui_ImplAllegro5_CreateDeviceObjects();
			}

			io = ImGui::GetIO();
			OnImGuiEvent(event);
		}
    
		virtual void OnImGuiRender() = 0;
		virtual void OnImGuiAttach() = 0;
		virtual void OnImGuiDetach() = 0;
		virtual void OnImGuiUpdate() = 0;
		virtual void OnImGuiEvent(Battery::Event* event) = 0;

	};

}
