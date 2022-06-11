
#include "Battery/Core/Config.h"
#include "Battery/Core/Application.h"
#include "Battery/Core/Exception.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/Utils/ImGuiUtils.h"

#include "Battery/Fonts/RobotoMedium.h"

#include "imgui-SFML.h"

namespace Battery {

	Application* appPointer = nullptr;

	Application& GetApp() {
		if (!appPointer)
			throw Battery::Exception("Not Initialized");

		return *appPointer;
	}

	Application::Application() {
		LOG_CORE_TRACE("Creating Application");
		appPointer = this;
	}

	Application::~Application() {
		LOG_CORE_TRACE("Application stopped, destroying");
	}

	void Application::OnStartup() {}
	void Application::OnUpdate() {}
	void Application::OnRender() {}
	void Application::OnShutdown() {}

	void Application::OnEvent(sf::Event event, bool& handled) {
		if (event.type == sf::Event::EventType::Closed) {
			CloseApplication();
		}
	}

	void Application::SetFramerate(float fps) {
		window.setFramerateLimit(fps);
	}

	void Application::PushLayer(std::shared_ptr<Layer> layer) {
		layers.PushLayer(layer);
	}

	void Application::PushOverlay(std::shared_ptr<Layer> overlay) {
		layers.PushOverlay(overlay);
	}

	void Application::ClearLayerStack() {
		layers.ClearStack();
	}

	void Application::CloseApplication() {
		shutdownRequested = true;
	}

	glm::ivec2 Application::GetPrimaryMonitorSize() {
		return glm::ivec2(sf::VideoMode::getDesktopMode().size.x, sf::VideoMode::getDesktopMode().size.y);
	}









	void Application::Run(int width, int height, int argc, const char** argv) {
		try {
			LOG_CORE_INFO("Loading Application");

			// Auto-fit window size
			if (width <= 0) width = GetPrimaryMonitorSize().x;
			if (height <= 0) height = GetPrimaryMonitorSize().y;

			// Create window
			window.create(sf::VideoMode({ (uint32_t)width, (uint32_t)height }), BATTERY_DEFAULT_TITLE);
			if (!window.isOpen())
				throw Battery::Exception("Could not create SFML window! Please check the graphics drivers!");

			// Parse command line arguments
			LOG_CORE_TRACE("Command line arguments:");
			for (int i = 0; i < argc; i++) {
				args.push_back(argv[i]);
				LOG_CORE_TRACE(std::string("[") + std::to_string(i) + "]: " + args[i]);
			}

			// Load ImGui
			IMGUI_CHECKVERSION();
			ImGui::SFML::Init(window);
			ImGui::StyleColorsDark();
			CaptureCurrentColorSchemeAsDefault();
			LoadBatteryColorScheme();

			ImPlot::CreateContext();
			window.clear(BATTERY_DEFAULT_BACKGROUND_COLOR);
			window.display();

			OnStartup();

			defaultFont = ADD_FONT(RobotoMedium, DEFAULT_FONT_SIZE);
			ImGui::SFML::UpdateFontTexture();

			LOG_CORE_INFO("Application running");
			RunMainloop();
			LOG_CORE_INFO("Stopping Application");
			window.setVisible(false);
			OnShutdown();

			ImPlot::DestroyContext();
			ImGui::SFML::Shutdown();

			layers.ClearStack();
			window.close();

			LOG_CORE_INFO("Application stopped");
		}
		catch (const Battery::Exception& e) {
			LOG_CORE_CRITICAL(std::string("Unhandled Battery::Exception: '") + e.what() + "'");
		}
		catch (const std::exception& e) {
			LOG_CORE_CRITICAL(std::string("Unhandled std::exception: '") + e.what() + "'");
		}
		catch (...) {
			LOG_CORE_CRITICAL("Unhandled exception: Unknown exception type, make sure to catch it correctly!");
		}
	}

	void Application::RunMainloop() {

		while (!shutdownRequested) {

			// Update everything
			PreUpdate();
			UpdateApp();
			PostUpdate();

			// Render everything
			PreRender();
			RenderApp();
			PostRender();

			// Show rendered image (and sleep)
			window.display();
		}
	}

	void Application::PreUpdate() {

		sf::Time dt = frametime_clock.restart();
		frametime = dt.asSeconds();
				
		if (frametime != 0.f) {
			framerate = 1.f / frametime;
		}

		HandleEvents();
        ImGui::SFML::Update(window, dt);
	}

	void Application::UpdateApp() {

		// First update the base application
		LOG_CORE_TRACE("Application::OnUpdate()");
		OnUpdate();

		// Then propagate through the stack and update all layers sequentially
		for (auto& layer : layers.GetLayers()) {
			LOG_CORE_TRACE("Layer OnUpdate()");
			layer->OnUpdate();
		}
	}

	void Application::PostUpdate() {
		framecount++;
	}

	void Application::PreRender() {
		window.clear(BATTERY_DEFAULT_BACKGROUND_COLOR);
		ImGui::PushFont(defaultFont);
	}

	void Application::RenderApp() {

		// First render the base application
		LOG_CORE_TRACE("Application::OnRender()");
		OnRender();

		// Then propagate through the stack and render all layers sequentially
		for (auto& layer : layers.GetLayers()) {
			LOG_CORE_TRACE("Layer OnRender()");
			layer->OnRender();
		}
	}

	void Application::PostRender() {
		ImGui::PopFont();
        ImGui::SFML::Render(window);
	}

	void Application::HandleEvents() {

        sf::Event event;
		bool handled = false;

		while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

			// Give the event to the base application
			LOG_CORE_TRACE("Application::OnEvent()");
			OnEvent(event, handled);

			if (handled) {
				LOG_CORE_TRACE("Event was handled by the base application");
				continue;
			}

			// Propagate through the layer stack in reverse order
			for (size_t i = 0; i < layers.GetLayers().size(); i++) {
				auto& layer = layers.GetLayers()[layers.GetLayers().size() - i - 1];
				LOG_CORE_TRACE("Layer OnEvent()");
				handled = false;
				layer->OnEvent(event, handled);

				if (handled) {
					LOG_CORE_TRACE("Event was handled by Layer #{}", i);
					break;
				}
			}
        }
	}
}
