#pragma

#include "Battery/Utils/TypeUtils.h"

namespace Battery {
    
    struct __trayIconData__;
    struct TrayMessage;
	class TrayIcon {
	public:
		TrayIcon(const sf::Image& icon, const std::string& tip = "");
		~TrayIcon();

		void update();
		void attachLeftClickCallback(std::function<void(void)> callback);
		void attachRightClickCallback(std::function<void(void)> callback);

	private:
        void handleMessage(const TrayMessage& msg);

		IncompleteTypeWrapper<__trayIconData__> data;
		size_t id;
		inline static size_t __id__ = 0;

		std::function<void(void)> leftClickCallback;
		std::function<void(void)> rightClickCallback;
	};
}
