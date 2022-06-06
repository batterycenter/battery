
#include "Battery/Core/Application.h"
#include "Battery/Renderer/Renderer2D.h"
#include "Battery/Utils/TimeUtils.h"

namespace Battery {

	// Global application pointer
	Application* applicationPointer = nullptr;

	Application& GetApp() {
		if (!applicationPointer) {
			throw Battery::Exception(std::string(__FUNCTION__) + ": The application has not been constructed yet! Use a Battery::Application derived class!");
		}

		return *applicationPointer;
	}

	sf::Window& GetMainWindow() {
		return GetApp().window;
	}

	Application::Application() {

		if (applicationPointer != nullptr) {
			throw Battery::Exception("Cannot construct Battery::Application: Another application has already been constructed!");
		}

		applicationPointer = this;
		LOG_CORE_TRACE("Creating Application");
	}

	Application::~Application() {
		applicationPointer = nullptr;
		LOG_CORE_TRACE("Application stopped, destroying");
	}








	

	glm::ivec2 Application::GetPrimaryMonitorSize() {
		return glm::ivec2(sf::VideoMode::getDesktopMode().size.x, sf::VideoMode::getDesktopMode().size.y);
	}











	void Application::Run(int width, int height, int argc, const char** argv) {
		try {
			RunApplication(width, height, argc, argv);
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

	void Application::RunApplication(int width, int height, int argc, const char** argv) {

		// Auto-fit window size
		if (width <= 0) width = GetPrimaryMonitorSize().x;
		if (height <= 0) height = GetPrimaryMonitorSize().y;

		window.create(sf::VideoMode({ (uint32_t)width, (uint32_t)height }), BATTERY_DEFAULT_TITLE);
		if (!window.isOpen())
			throw Battery::Exception("Could not create SFML window! Please check the graphics drivers!");

		// Load 2D renderer and draw the default background color
		//Renderer2D::Setup();

		// Parse command line arguments
		LOG_CORE_TRACE("Command line arguments:");
		for (int i = 0; i < argc; i++) {
			args.push_back(argv[i]);
			LOG_CORE_TRACE(std::string("[") + std::to_string(i) + "]: " + args[i]);
		}

		// Client startup
		try {
			LOG_CORE_INFO("Application created, loading client OnStartup()");
			OnStartup();
			LOG_CORE_INFO("Application loaded, entering main loop");
		}
		catch (const Battery::Exception& e) {
			LOG_CORE_CRITICAL(std::string("Application::OnStartup() threw Battery::Exception: ") + e.what());
			LOG_CORE_ERROR("Shutting engine down...");
			shouldClose = true;
		}

		// Client update
		try {
			RunMainloop();
		}
		catch (const Battery::Exception& e) {
			LOG_CORE_CRITICAL(std::string("Some Layer's OnUpdate() function threw Battery::Exception: ") + e.what());
			LOG_CORE_ERROR("Shutting engine down...");
		}

		// Client shutdown
		try {
			LOG_CORE_INFO("Shutting down: client OnShutdown()");
			OnShutdown();
		}
		catch (const Battery::Exception& e) {
			LOG_CORE_CRITICAL(std::string("Application::OnShutdown() threw Battery::Exception: ") + e.what());
		}

		// Unload 2D renderer
		LOG_CORE_TRACE("Shutting down 2D Renderer");
		Renderer2D::Shutdown();

		// Destroy Allegro window
		window.close();

		// Clear layer stack
		LOG_CORE_TRACE("Clearing any left over layers from layer stack");
		layers.ClearStack();

		LOG_CORE_INFO("Application stopped");
	}

	void Application::RunMainloop() {

		double nextFrame = GetRuntime();
		double desiredFrametime = 0.0;

		while (!shouldClose) {

			// Update everything
			PreUpdate();
			UpdateApp();
			PostUpdate();

			// Render everything
			PreRender();
			RenderApp();
			PostRender();

			// Wait for the right time to render
			desiredFrametime = 1.0 / desiredFramerate;
			LOG_CORE_TRACE("Waiting for frametime before flipping screen");
			Sleep(nextFrame - GetRuntime());

			// Set time for next frame
			double now = GetRuntime();
			if (now <= nextFrame + desiredFrametime) {	// Everything is on time, just increment for
				nextFrame += desiredFrametime;			// maximum accuracy in framerate
			}
			else {	// Too slow, a frame was missed, increment from timepoint now, but framerate will
				nextFrame = now + desiredFrametime;					// not be 100% accurate
			}

			// Show rendered image
			if (!frameDiscarded) {
				//al_set_current_opengl_context(window.allegroDisplayPointer);
				//al_flip_display();
			}

			//ProfilerStorage::GetInstance().ApplyProfiles(frametimeTimer.Update());
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

		frameDiscarded = false;
	}

	void Application::PostUpdate() {
		framecount++;
	}

	void Application::PreRender() {
		// Paint the background by default
		Renderer2D::DrawBackground(BATTERY_DEFAULT_BACKGROUND_COLOR);
	}

	void Application::PostRender() {
		Renderer2D::EndUnfinishedScene();
	}

	void Application::UpdateApp() {

		// First update the base application
		LOG_CORE_TRACE("Application::OnUpdate()");
		OnUpdate();

		if (frameDiscarded) {
			LOG_CORE_TRACE("{}: {}", __FUNCTION__, "Skipping further update routines, frame was discarded");
			return;
		}

		// Then propagate through the stack and update all layers sequentially
		for (auto& layer : layers.GetLayers()) {
			LOG_CORE_TRACE("Layer '{}' OnUpdate()", layer->GetDebugName().c_str());
			layer->OnUpdate();
		}
	}

	void Application::RenderApp() {

		if (frameDiscarded) {
			LOG_CORE_TRACE("{}: {}", __FUNCTION__, "Skipping main render routine, frame was discarded");
			return;
		}

		// First render the base application
		LOG_CORE_TRACE("Application::OnRender()");
		OnRender();

		if (frameDiscarded) {
			LOG_CORE_TRACE("{}: {}", __FUNCTION__, "Skipping further render routines, frame was discarded");
			return;
		}

		// Then propagate through the stack and render all layers sequentially
		for (auto& layer : layers.GetLayers()) {
			LOG_CORE_TRACE("Layer '{}' OnRender()", layer->GetDebugName().c_str());
			layer->OnRender();
		}
		PROFILE_TIMESTAMP(__FUNCTION__"()");
	}

	void Application::RunEvents(Event* e) {

		// Give the event to the base application
		LOG_CORE_TRACE("Application::OnEvent()");
		OnEvent(e);

		/*if (e->WasHandled()) {
			LOG_CORE_TRACE("Event was handled by the base application");
			return;
		}

		// Propagate through the layer stack in reverse order
		for (size_t i = 0; i < layers.GetLayers().size(); i++) {
			auto& layer = layers.GetLayers()[layers.GetLayers().size() - i - 1];
			LOG_CORE_TRACE("Layer '{}' OnEvent()", layer->GetDebugName().c_str());
			layer->OnEvent(e);

			if (e->WasHandled()) {
				LOG_CORE_TRACE("Event was handled by Layer '{}'", layer->GetDebugName().c_str());
				break;
			}
		}*/
	}

	void Application::SetFramerate(double f) {
		desiredFramerate = f;
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
		shouldClose = true;
	}

	void Application::DiscardFrame() {
		frameDiscarded = true;
	}
}
