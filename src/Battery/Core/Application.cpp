
#include "Battery/Core/Application.h"
#include "Battery/Renderer/Renderer2D.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/Core/AllegroContext.h"

namespace Battery {

	// This function is local within this .cpp file
	static ALLEGRO_COLOR ConvertAllegroColor(const glm::vec4& color) {
		return al_map_rgba(color.r, color.g, color.b, color.a);
	}

	// TODO: Allegro version printen

	// TODO: Application name / window title optional

	// Global application pointer
	Application* applicationPointer = nullptr;

	Application& GetApp() {
		if (!applicationPointer) {
			throw Battery::Exception(std::string(__FUNCTION__) + ": The application has not been constructed yet! Use a Battery::Application derived class!");
		}

		return *applicationPointer;
	}

	Window& GetMainWindow() {
		return GetApp().window;
	}


	Application::Application(int width, int height, const std::string applicationFolderName) : window(width, height) {
		this->applicationFolderName = applicationFolderName;

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










	bool Application::GetKey(int allegroKeycode) {
		ALLEGRO_KEYBOARD_STATE keyboard;
		al_get_keyboard_state(&keyboard);
		return al_key_down(&keyboard, allegroKeycode);
	}

	std::string Application::GetKeyName(int allegroKeycode) {
		return al_keycode_to_name(allegroKeycode);
	}

	glm::ivec2 Application::GetPrimaryMonitorSize() {
		ALLEGRO_MONITOR_INFO monitor;
		al_get_monitor_info(0, &monitor);
		return glm::ivec2(monitor.x2 - monitor.x1, monitor.y2 - monitor.y1);
	}











	void Application::run(int argc, const char** argv) {
		try {
			runApplication(argc, argv);
		}
		catch (const Battery::Exception& e) {
			LOG_CORE_CRITICAL(std::string("Unhandled Battery::Exception: '") + e.what() + "'");
			Battery::ShowErrorMessageBox(std::string("Unhandled Battery::Exception: '") + e.what() + "'");
		}
		catch (const std::exception& e) {
			LOG_CORE_CRITICAL(std::string("Unhandled std::exception: '") + e.what() + "'");
			Battery::ShowErrorMessageBox(std::string("Unhandled std::exception: '") + e.what() + "'");
		}
		catch (...) {
			LOG_CORE_CRITICAL("Unhandled exception: Unknown exception type, make sure to catch it correctly!");
			Battery::ShowErrorMessageBox("Unhandled exception: Unknown exception type, make sure to catch it correctly!");
		}
	}

	void Application::runApplication(int argc, const char** argv) {

		// Initialize the Allegro framework
		if (!AllegroContext::GetInstance()->Initialize(applicationFolderName)) {
			LOG_CORE_WARN("The Allegro context failed to initialize, closing application...");
			return;
		}

		// Create Allegro window
		window.Create(windowFlags);
		window.SetEventCallback(std::bind(&Application::_onEvent, this, std::placeholders::_1));

		// Load 2D renderer and draw the default background color
		Renderer2D::Setup();

		// Parse command line arguments
		LOG_CORE_TRACE("Command line arguments:");
		for (int i = 0; i < argc; i++) {
			args.push_back(argv[i]);
			LOG_CORE_TRACE(std::string("[") + std::to_string(i) + "]: " + args[i]);
		}

		// Client startup
		try {
			LOG_CORE_INFO("Application created, loading client OnStartup()");
			if (!OnStartup()) {
				LOG_CORE_WARN("Application::OnStartup() returned false, "
					"skipping main loop and shutting engine down...");
				shouldClose = true;
			}
			else {
				LOG_CORE_INFO("Everything loaded, entering main loop");
			}
		}
		catch (const Battery::Exception& e) {
			LOG_CORE_CRITICAL(std::string("Application::OnStartup() threw Battery::Exception: ") + e.what());
			ShowErrorMessageBox(std::string("Application::OnStartup() threw Battery::Exception: ") + e.what());
			LOG_CORE_ERROR("Shutting engine down...");
			shouldClose = true;
		}

		// Client update
		try {
			_mainLoop();
		}
		catch (const Battery::Exception& e) {
			LOG_CORE_CRITICAL(std::string("Some Layer's OnUpdate() function threw Battery::Exception: ") + e.what());
			ShowErrorMessageBox(std::string("Some Layer's OnUpdate() function threw Battery::Exception: ") + e.what());
			LOG_CORE_ERROR("Shutting engine down...");
		}

		// Client shutdown
		try {
			LOG_CORE_INFO("Shutting down: client OnShutdown()");
			OnShutdown();
		}
		catch (const Battery::Exception& e) {
			LOG_CORE_CRITICAL(std::string("Application::OnShutdown() threw Battery::Exception: ") + e.what());
			ShowErrorMessageBox(std::string("Application::OnShutdown() threw Battery::Exception: ") + e.what());
		}

		// Unload 2D renderer
		LOG_CORE_TRACE("Shutting down 2D Renderer");
		Renderer2D::Shutdown();

		// Destroy Allegro window
		window.Destroy();

		// Clear layer stack
		LOG_CORE_TRACE("Clearing any left over layers from layer stack");
		layers.ClearStack();

		// Shut the allegro framework down
		LOG_CORE_TRACE("Destroying Allegro context");
		AllegroContext::GetInstance()->Destroy();

		LOG_CORE_INFO("Application stopped");
	}

	void Application::_preUpdate() {
		double now = GetRuntime();
		
		frametime = now - oldPreUpdateTime;
		
		if (oldPreUpdateTime == 0) {
			frametime = 0.f;
		}
		
		oldPreUpdateTime = now;
		if (frametime != 0.f)
			framerate = 1.0 / frametime;

		frameDiscarded = false;

		// Handle events
		window.HandleEvents();
		PROFILE_TIMESTAMP(__FUNCTION__"() (and Handled events)");
	}

	void Application::_postUpdate() {
		framecount++;
		PROFILE_TIMESTAMP(__FUNCTION__"()");
	}

	void Application::_preRender() {
		// Paint the background by default
		Renderer2D::DrawBackground(BATTERY_DEFAULT_BACKGROUND_COLOR);
		PROFILE_TIMESTAMP(__FUNCTION__"()");
	}

	void Application::_postRender() {
		Renderer2D::EndUnfinishedScene();
		PROFILE_TIMESTAMP(__FUNCTION__"()");
	}

	void Application::_mainLoop() {

		double nextFrame = GetRuntime();
		double desiredFrametime = 0.0;

		//ConsistentTimer frametimeTimer;
		//frametimeTimer.Update();

		while (!shouldClose) {

			PROFILE_TIMESTAMP_START("Mainloop start");
			LOG_CORE_TRACE("Main loop started");

			// Update everything
			{
				//PROFILE_CORE_SCOPE("Mainloop update routines");
				_preUpdate();
				_updateApp();
				_postUpdate();
			}
			
			// Render everything
			{
				PROFILE_CORE_SCOPE("Mainloop render rountines");
				_preRender();
				_renderApp();
				_postRender();
			}

			{
				PROFILE_CORE_SCOPE("Mainloop sleeping until next frame");
				// Wait for the right time to render
				desiredFrametime = 1.0 / desiredFramerate;
				LOG_CORE_TRACE("Waiting for frametime before flipping screen");
				Sleep(nextFrame - GetRuntime());
				while (GetRuntime() < nextFrame);	// Shouldn't do anything, for safety if sleeping
																// was not successful
				PROFILE_TIMESTAMP("Slept until next frame timepoint");
			}

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
				PROFILE_CORE_SCOPE("Mainloop flipping frame buffers");
				LOG_CORE_TRACE("Flipping displays");
				al_set_current_opengl_context(window.allegroDisplayPointer);
				al_flip_display();
				PROFILE_TIMESTAMP("Flipped display buffers");
			}

			LOG_CORE_TRACE("Main loop finished, applying profiling results");
			PROFILE_TIMESTAMP("Entire frametime");
			//ProfilerStorage::GetInstance().ApplyProfiles(frametimeTimer.Update());
		}
	}

	void Application::_updateApp() {

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
		//PROFILE_TIMESTAMP(__FUNCTION__ "()");
	}

	void Application::_renderApp() {

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

	void Application::_onEvent(Event* e) {

		// Give the event to the base application
		LOG_CORE_TRACE("Application::OnEvent()");
		OnEvent(e);

		if (e->WasHandled()) {
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
		}
	}

	void Application::SetFramerate(double f) {
		desiredFramerate = f;
	}

	void Application::SetWindowFlag(enum WindowFlags flag) {
		this->windowFlags |= (int)flag;
	}

	void Application::ClearWindowFlag(enum WindowFlags flag) {
		this->windowFlags &= ~(int)flag;
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
