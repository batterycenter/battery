
#ifdef BATTERY_FEATURES_GRAPHICS

#include "Battery/Core/Config.h"
#include "Battery/Core/Application.h"
#include "Battery/Core/Exception.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/Utils/ImGuiUtils.h"
#include "Battery/Utils/StringUtils.h"
#include "Battery/Platform/Platform.h"

#include "Battery/Fonts/RobotoMedium.h"

#include "imgui-SFML.h"

namespace Battery {

	Application* appPointer = nullptr;

	Application& GetApp() {
		if (!appPointer)
			throw Battery::Exception("Not Initialized");

		return *appPointer;
	}

	bool SetWindowIcon(sf::RenderWindow& window, uint8_t iconSize, void* data, size_t dataSize) {
		sf::Image image;
		if (!image.loadFromMemory(data, dataSize)) {
            Log::Core::Error("Cannot load window icon: Unsupported image format, cannot load sf::Image from memory!");
			return false;
		}
		SetWindowIcon(window, iconSize, image);
		return true;
	}

	void SetWindowIcon(sf::RenderWindow& window, uint8_t iconSize, sf::Image image) {
		window.setIcon(iconSize, iconSize, image.getPixelsPtr());
	}

	bool SetWindowIconBase64(sf::RenderWindow& window, uint8_t iconSize, const std::string& data) {
		auto decoded = String::DecodeBase64(data);
		if (decoded.empty()) {
            Log::Core::Error("Cannot load window icon: Base64 encoding contains no data!");
			return false;
		}
		return SetWindowIcon(window, iconSize, &decoded[0], decoded.size());
	}

	void SetMouseCursor(sf::Cursor::Type type) {
		sf::Cursor cursor;
		if (cursor.loadFromSystem(type));
			GetApp().window.setMouseCursor(cursor);
	}

	void SetWindowTransparent(sf::RenderWindow& window, bool transparent) {
		platform_SetWindowTransparent(window.getSystemHandle(), transparent);
	}

	void SetWindowAlpha(sf::RenderWindow& window, uint8_t alpha) {
		platform_SetWindowAlpha(window.getSystemHandle(), alpha);
	}

	void SetWindowFocus(sf::RenderWindow& window) {
		platform_Focus(window.getSystemHandle());
	}




	Application::Application() {
        Log::Core::Trace("Creating Application");
		appPointer = this;
	}

	Application::~Application() {
        Log::Core::Trace("Application stopped, destroying");
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

	void Application::SetWindowStandby(bool standby) {
		windowStandby = standby;
	}

	void Application::SetFramerate(uint32_t fps) {  // TODO: Function not guaranteed to be used
		window.setFramerateLimit(fps);
        framerateLimit = fps;
	}

	void Application::PushLayer(std::shared_ptr<Layer> layer) {
		layers.PushLayer(std::move(layer));
	}

	void Application::PushOverlay(std::shared_ptr<Layer> overlay) {
		layers.PushOverlay(std::move(overlay));
	}

	void Application::ClearLayerStack() {
		layers.ClearStack();
	}

	void Application::CloseApplication() {
		shutdownRequested = true;
	}

	glm::ivec2 Application::GetPrimaryMonitorSize() {
		return {sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height};
	}









	void Application::Run(int width, int height, int argc, const char** argv, int windowStyle) {
		try {
            Log::Core::Info("Loading Application");
			
			// Auto-fit window size
			if (width < 0) width = GetPrimaryMonitorSize().x;
			if (height < 0) height = GetPrimaryMonitorSize().y;

			// Create window
			window.create(sf::VideoMode({ (uint32_t)width, (uint32_t)height }), BATTERY_DEFAULT_TITLE, windowStyle);
			if (!window.isOpen())
				throw Battery::Exception("Could not create SFML window! Please check the graphics drivers!");

			// Parse command line arguments
            Log::Core::Trace("Command line arguments:");
			for (int i = 0; i < argc; i++) {
				args.emplace_back(argv[i]);
                Log::Core::Trace("[{}]: {}", i, args[i]);
			}

			// Load the default window icon
			SetWindowIconBase64(window, (uint8_t)BATTERY_DEFAULT_WINDOW_ICON_SIZE, BATTERY_DEFAULT_WINDOW_ICON_BASE64);

			// Load ImGui
			IMGUI_CHECKVERSION();
			ImGui::SFML::Init(window);

            SetFramerate(60);

			ImGui::StyleColorsDark();
			CaptureCurrentColorSchemeAsDefault();
			LoadBatteryColorScheme();

			ImPlot::CreateContext();
			window.clear(BATTERY_DEFAULT_BACKGROUND_COLOR);
			window.display();

			ClearRuntime();

            //ui = BatteryUI::Setup<UI>([&] {
            //    glfwPostEmptyEvent();
            //});
            //BatteryUI::theme = BatteryUI::Theme_Modern();

            OnStartup();    // This is the user's startup function

			defaultFont = ADD_FONT(RobotoMedium, DEFAULT_FONT_SIZE);
			ImGui::SFML::UpdateFontTexture();

			if (!shutdownRequested) {
                Log::Core::Info("Application running");
				RunMainloop();
			}
			else {
                Log::Core::Warn("OnStartup requested shutdown, skipping main loop entirely and shutting down immediately");
			}

            Log::Core::Info("Stopping Application");
			OnShutdown();
            layers.ClearStack();

            window.setVisible(false);

			ImPlot::DestroyContext();
			ImGui::SFML::Shutdown();
			window.close();

            Log::Core::Info("Application stopped");
		}
		catch (const Battery::Exception& e) {
            Log::Core::Critical("Unhandled Battery::Exception: '{}'", e.what());
		}
		catch (const std::exception& e) {
            Log::Core::Critical("Unhandled std::exception: '{}'", e.what());
		}
		catch (...) {
            Log::Core::Critical("Unhandled exception: Unknown exception type, make sure to catch it correctly!");
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
			if (!windowStandby) {
				window.display();
			}
			else {
				Sleep(1.f / (float)framerateLimit);    // Standby
			}
		}
	}

	void Application::PreUpdate() {

		sf::Time dt = frametime_clock.restart();
		frametime = dt.asSeconds();
				
		if (frametime != 0.f) {
			framerate = 1.f / frametime;
		}

		HandleEvents();
		if (!windowStandby) {
			ImGui::SFML::Update(window, dt);
		}
	}

	void Application::UpdateApp() {

		// First update the base application
        Log::Core::Trace("Application::OnUpdate()");
		OnUpdate();

		// Then propagate through the stack and update all layers sequentially
		for (auto& layer : layers.GetLayers()) {
            Log::Core::Trace("Layer OnUpdate()");
			layer->OnUpdate();
		}
	}

	void Application::PostUpdate() {
		framecount++;
	}

	void Application::PreRender() {
		if (!windowStandby) {
			window.clear(BATTERY_DEFAULT_BACKGROUND_COLOR);
			ImGui::PushFont(defaultFont);
		}
	}

	void Application::RenderApp() {

		// First render the base application
        Log::Core::Trace("Application::OnRender()");
		OnRender();

		// Then propagate through the stack and render all layers sequentially
		for (auto& layer : layers.GetLayers()) {
            Log::Core::Trace("Layer OnRender()");
			layer->OnRender();
		}
	}

	void Application::PostRender() {
		if (!windowStandby) {
			ImGui::PopFont();
			ImGui::SFML::Render(window);
		}
	}

	void Application::HandleEvents() {

        sf::Event event{};
		bool handled = false;

		while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

			// Propagate through the layer stack in reverse order
			for (size_t i = 0; i < layers.GetLayers().size(); i++) {
				auto& layer = layers.GetLayers()[layers.GetLayers().size() - i - 1];
                Log::Core::Trace("Layer OnEvent()");
				handled = false;
				layer->OnEvent(event, handled);

				if (handled) {
                    Log::Core::Trace("Event was handled by Layer #{}", i);
					break;
				}
			}

			// Give the event to the base application
            Log::Core::Trace("Application::OnEvent()");
			OnEvent(event, handled);

			if (handled) {
                Log::Core::Trace("Event was handled by the base application");
				continue;
			}
        }
	}
}

#endif // BATTERY_FEATURES_GRAPHICS
