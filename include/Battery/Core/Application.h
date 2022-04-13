#pragma once

#include "Battery/pch.h"
#include "Battery/Core/Layer.h"
#include "Battery/Core/LayerStack.h"
#include "Battery/Core/ApplicationEvents.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Core/Window.h"
#include "Battery/Core/Config.h"
#include "Battery/Core/Event.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/Platform/Dialog.h"
#include "Battery/Log/Log.h"

enum class WindowFlags {
	NONE					= 1 << 0,
	NON_RESIZABLE			= 1 << 1,
	FRAMELESS				= 1 << 2,
	NO_TASKBAR				= 1 << 3,
	FULLSCREEN				= 1 << 4,
	WINDOWED_FULLSCREEN		= 1 << 5,
	HIDDEN					= 1 << 6		// TODO: Add flag for no window at all
};

// Forward declaring, so that main can be a friend function
// and gets access to the private Run() function
int main(int argc, const char** argv);

namespace Battery {

	class Application;
	extern Application* CreateApplication();	// This will be defined in the client project

	Application& GetApp();
	Window& GetMainWindow();

	class Application {
	public:

		Application(int width, int height, const std::string applicationFolderName = BATTERY_DEFAULT_FOLDER_NAME);
		virtual ~Application();

		virtual bool OnStartup() = 0;
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
		void run(int argc, const char** argv);
		void runApplication(int argc, const char** argv);
		friend int ::main(int argc, const char** argv);

		void _preUpdate();
		void _postUpdate();
		void _preRender();
		void _postRender();
		void _mainLoop();
		void _updateApp();
		void _renderApp();
		void _onEvent(Battery::Event* e);

	// All layers should have access through the application pointer
	public:
		LayerStack layers;			// TODO: Make layers private
		bool shouldClose = false;

		double frametime = 0;
		double framerate = 0;
		uint32_t framecount = 0;

		std::vector<std::string> args;
		double desiredFramerate = 60;
		double oldPreUpdateTime = 0;

	private:
		std::string applicationFolderName;
		int windowFlags = (int)WindowFlags::NONE;
		bool frameDiscarded = false;

	protected:
		friend Window& GetMainWindow();
		Window window;
	};

}
