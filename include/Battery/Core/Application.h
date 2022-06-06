#pragma once

#include "Battery/pch.h"
#include "Battery/Core/Log.h"
#include "Battery/Core/Layer.h"
#include "Battery/Core/LayerStack.h"
//#include "Battery/Core/ApplicationEvents.h"
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
	sf::Window& GetMainWindow();

	class Application {
	public:

		double frametime = 0;
		double framerate = 0;
		uint32_t framecount = 0;
		std::vector<std::string> args;
		sf::Window window;

		Application();
		virtual ~Application();

		virtual void OnStartup() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnShutdown() = 0;
		virtual void OnEvent(Battery::Event* e) = 0;

		bool GetKey(int allegroKeycode);
		std::string GetKeyName(int allegroKeycode);
		glm::ivec2 GetPrimaryMonitorSize();
		// TODO: Add keyboard leds cuz why not

		void SetFramerate(double f);
		void SetWindowFlag(enum WindowFlags flag);
		void ClearWindowFlag(enum WindowFlags flag);
		void PushLayer(std::shared_ptr<Layer> layer);
		void PushOverlay(std::shared_ptr<Layer> overlay);
		void ClearLayerStack();
		void CloseApplication();
		void DiscardFrame();

	private:
		void Run(int width, int height, int argc, const char** argv);
		void RunApplication(int width, int height, int argc, const char** argv);
		friend int ::main(int argc, const char** argv);

		void RunMainloop();
		void PreUpdate();
		void PostUpdate();
		void PreRender();
		void PostRender();
		void UpdateApp();
		void RenderApp();
		void RunEvents(Battery::Event* e);

		LayerStack layers;
		bool shouldClose = false;
		bool frameDiscarded = false;
		double desiredFramerate = 60;

	protected:
		friend sf::Window& GetMainWindow();
	};

}
