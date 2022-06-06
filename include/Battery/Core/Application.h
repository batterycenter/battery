#pragma once

#include "Battery/pch.h"
#include "Battery/Core/Log.h"
#include "Battery/Core/Layer.h"
#include "Battery/Core/LayerStack.h"
#include "Battery/Core/Window.h"
#include "Battery/Core/Config.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/Platform/Dialog.h"

#include "SFML/Window.hpp"

// Forward declaring, so that main can be a friend function
// and gets access to the private Run() function
int main(int argc, const char** argv);

namespace Battery {

	Application& GetApp();
	sf::RenderWindow& GetMainWindow();

	class Application {
	public:

		double frametime = 0;
		double framerate = 0;
		uint32_t framecount = 0;
		std::vector<std::string> args;
		sf::RenderWindow window;

		Application();
		virtual ~Application();

		virtual void OnStartup() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnShutdown() {}
		virtual void OnEvent(sf::Event event, bool& handled);

		bool GetKey(int allegroKeycode);
		std::string GetKeyName(int allegroKeycode);
		glm::ivec2 GetPrimaryMonitorSize();
		// TODO: Add keyboard leds cuz why not

		void SetFramerate(float fps);
		void SetWindowFlag(enum WindowFlags flag);
		void ClearWindowFlag(enum WindowFlags flag);
		void PushLayer(std::shared_ptr<Layer> layer);
		void PushOverlay(std::shared_ptr<Layer> overlay);
		void ClearLayerStack();
		void CloseApplication();
		void DiscardFrame();

	private:
		void Run(int width, int height, int argc, const char** argv);
		friend int ::main(int argc, const char** argv);

		void RunMainloop();
		void PreUpdate();
		void PostUpdate();
		void PreRender();
		void PostRender();
		void UpdateApp();
		void RenderApp();
		void HandleEvents();

		LayerStack layers;
    	sf::Clock window_dt;
		bool shutdownRequested = false;
	};

}
