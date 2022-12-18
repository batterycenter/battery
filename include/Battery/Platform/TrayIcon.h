#pragma once

#include "Battery/common.h"
#include "Battery/Utils/TypeUtils.h"
#include "Battery/Utils/OsString.h"

namespace Battery {
    
    struct __trayIconData__;
    struct TrayMessage;
	class TrayIcon {
	public:
		//TrayIcon(const std::string& icon_path, const OsString& tip = "");
        TrayIcon(const sf::Image& icon, const OsString& tip = "");
		~TrayIcon();

		void update();
		void attachLeftClickCallback(std::function<void(void)> callback);
		void attachRightClickCallback(std::function<void(void)> callback);
        void setIcon(const sf::Image& icon);

	private:
        void handleMessage(const TrayMessage& msg);

		IncompleteTypeWrapper<__trayIconData__> data;
		size_t id;
		inline static size_t __id__ = 0;

		std::function<void(void)> leftClickCallback;
		std::function<void(void)> rightClickCallback;
	};
}
