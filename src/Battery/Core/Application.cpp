
#include "Battery/Core/Application.h"
#include "Battery/Renderer/Renderer2D.h"
#include "Battery/Utils/TimeUtils.h"
#include "imgui-SFML.h"

namespace Battery {

	Application::Application() {
		LOG_CORE_TRACE("Creating Application");
	}

	Application::~Application() {
		LOG_CORE_TRACE("Application stopped, destroying");
	}

	void Application::OnEvent(sf::Event event, bool& handled) {
		if (event.type == sf::Event::EventType::Closed) {
			CloseApplication();
		}
	}








	

	glm::ivec2 Application::GetPrimaryMonitorSize() {
		return glm::ivec2(sf::VideoMode::getDesktopMode().size.x, sf::VideoMode::getDesktopMode().size.y);
	}











	void Application::Run(int width, int height, int argc, const char** argv) {
		try {

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

			ImGui::SFML::Init(window);
			Renderer2D::Setup();

			OnStartup();
			RunMainloop();
			OnShutdown();

			Renderer2D::Shutdown();
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

		static double old = GetRuntime();
		double now = GetRuntime();
		frametime = now - old;
		old = now;
				
		if (frametime != 0.f) {
			framerate = 1.0 / frametime;
		}

		HandleEvents();
        ImGui::SFML::Update(window, window_dt.restart());
	}

	void Application::PostUpdate() {
		framecount++;
	}

	void Application::PreRender() {
		window.clear(BATTERY_DEFAULT_BACKGROUND_COLOR);
	}

	void Application::PostRender() {
        ImGui::SFML::Render(window);
		Renderer2D::EndUnfinishedScene();
	}

	void Application::UpdateApp() {

		// First update the base application
		LOG_CORE_TRACE("Application::OnUpdate()");
		OnUpdate();

		// Then propagate through the stack and update all layers sequentially
		for (auto& layer : layers.GetLayers()) {
			LOG_CORE_TRACE("Layer '{}' OnUpdate()", layer->GetDebugName().c_str());
			layer->OnUpdate();
		}
	}

	void Application::RenderApp() {

		// First render the base application
		LOG_CORE_TRACE("Application::OnRender()");
		OnRender();

		// Then propagate through the stack and render all layers sequentially
		for (auto& layer : layers.GetLayers()) {
			LOG_CORE_TRACE("Layer '{}' OnRender()", layer->GetDebugName().c_str());
			layer->OnRender();
		}
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
				LOG_CORE_TRACE("Layer '{}' OnEvent()", layer->GetDebugName().c_str());
				handled = false;
				layer->OnEvent(event, handled);

				if (handled) {
					LOG_CORE_TRACE("Event was handled by Layer '{}'", layer->GetDebugName().c_str());
					break;
				}
			}
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
}
