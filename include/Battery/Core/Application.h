#pragma once

#include "Battery/pch.h"
#include "Battery/Core/Log.h"
#include "Battery/Core/Layer.h"
#include "Battery/Core/LayerStack.h"
#include "Battery/Core/Config.h"
#include "Battery/Utils/TimeUtils.h"

#include "SFML/Window.hpp"

namespace Battery {

	class Application;
	Application& GetApp();

	bool SetWindowIcon(sf::RenderWindow& window, uint8_t iconSize, void* data, size_t dataSize);
	bool SetWindowIconBase64(sf::RenderWindow& window, uint8_t iconSize, const std::string& data);
	void SetMouseCursor(sf::Cursor::Type type);

	class Application {
	public:

		Application();
		virtual ~Application();

		virtual void OnStartup();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnShutdown();
		virtual void OnEvent(sf::Event event, bool& handled);

		void SetFramerate(float fps);
		void PushLayer(std::shared_ptr<Layer> layer);
		void PushOverlay(std::shared_ptr<Layer> overlay);
		void ClearLayerStack();
		void CloseApplication();
		glm::ivec2 GetPrimaryMonitorSize();

		void Run(int width, int height, int argc, const char** argv);

		float frametime = 0.f;
		float framerate = 0.f;
		size_t framecount = 0;
		std::vector<std::string> args;
		sf::RenderWindow window;

	private:
		void RunMainloop();

		void PreUpdate();
		void UpdateApp();
		void PostUpdate();

		void PreRender();
		void RenderApp();
		void PostRender();
		
		void HandleEvents();

		LayerStack layers;
    	sf::Clock window_dt;
    	sf::Clock frametime_clock;
		ImFont* defaultFont = nullptr;
		bool shutdownRequested = false;
	};

}
