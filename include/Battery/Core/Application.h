#pragma once

#include "Battery/common.h"
#include "Battery/Core/Config.h"
#include "Battery/Core/log.hpp"
#include "Battery/Core/Layer.h"
#include "Battery/Core/LayerStack.h"
#include "Battery/Core/Config.h"
#include "Battery/Utils/TimeUtils.h"

bool SetWindowIcon(sf::RenderWindow& window, uint8_t iconSize, void* data, size_t dataSize);
void SetWindowIcon(sf::RenderWindow& window, uint8_t iconSize, sf::Image image);
bool SetWindowIconBase64(sf::RenderWindow& window, uint8_t iconSize, const std::string& data);
void SetMouseCursor(sf::Cursor::Type type);
void SetWindowTransparent(sf::RenderWindow& window, bool transparent);
void SetWindowAlpha(sf::RenderWindow& window, uint8_t alpha);
void SetWindowFocus(sf::RenderWindow& window);

virtual void OnEvent(sf::Event event, bool& handled);
void SetWindowStandby(bool standby);
glm::ivec2 GetPrimaryMonitorSize();

namespace Battery {

	class Application;
	Application& GetApp();

	class Application {
	public:

		void SetFramerate(uint32_t fps);
		void PushLayer(std::shared_ptr<Layer> layer);
		void PushOverlay(std::shared_ptr<Layer> overlay);
		void ClearLayerStack();
		void CloseApplication();

        template<typename T>
        T* GetUI() {
            return (T*)ui;
        }

		void Run(int width, int height, int argc, const char** argv, int windowStyle = sf::Style::Default);

		float frametime = 0.f;
		float framerate = 0.f;
		size_t framecount = 0;
		std::vector<std::string> args;
		sf::RenderWindow window;
		std::string applicationName = BATTERY_DEFAULT_APPLICATION_NAME;

	private:
		void RunMainloop();

		void PreUpdate();
		void UpdateApp();
		void PostUpdate();

		void PreRender();
		void RenderApp();
		void PostRender();
		
		void HandleEvents();

        std::unique_ptr<BatteryUI::RootUI> ui;
		LayerStack layers;
    	sf::Clock window_dt;
    	sf::Clock frametime_clock;
        uint32_t framerateLimit = 0;
		ImFont* defaultFont = nullptr;
		bool shutdownRequested = false;
		bool windowStandby = false;
	};

}
