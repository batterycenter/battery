
#ifdef __linux__

#include "Battery/Platform/TrayIcon.h"

#include "Battery/Core/Log.h"
#include "Battery/Core/Exception.h"
#include <queue>

namespace Battery {

	struct __trayIconData__ {
		int x;
	};

	TrayIcon::TrayIcon(const sf::Image& icon, const std::string& tip) : data(new __trayIconData__()) {
//#warning TODO: TrayIcon not implemented
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}

	TrayIcon::~TrayIcon() {
	}

	void TrayIcon::update() {
//#warning TODO: TrayIcon not implemented
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}

	void TrayIcon::attachLeftClickCallback(std::function<void(void)> callback) {
//#warning TODO: TrayIcon not implemented
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}

	void TrayIcon::attachRightClickCallback(std::function<void(void)> callback) {
//#warning TODO: TrayIcon not implemented
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}

    void TrayIcon::setIcon(const sf::Image& icon) {
//#warning TODO: TrayIcon not implemented
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
    }

    void TrayIcon::handleMessage(const TrayMessage& msg) {
//#warning TODO: TrayIcon not implemented
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
    }

}

#endif // _WIN32
