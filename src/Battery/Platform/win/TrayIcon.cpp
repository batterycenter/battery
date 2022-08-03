
#ifdef _WIN32

#include "Battery/Platform/TrayIcon.h"

#include "Battery/Core/Log.h"
#include "Battery/Core/Exception.h"
#include <queue>

#include "windows.h"
#include "shellapi.h"

namespace Battery {

    struct TrayMessage {
        WPARAM wParam = 0;
        LPARAM lParam = 0;
        size_t id = 0;
    };

    std::mutex messageStackMutex;
    std::queue<TrayMessage> messageStack;

	LRESULT CALLBACK TrayWndMessage(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
		if (iMsg > WM_USER) {
            TrayMessage msg;
            msg.wParam = wParam;
            msg.lParam = lParam;
            msg.id = WM_USER - 1;

            std::lock_guard<std::mutex> lock(messageStackMutex);
            messageStack.push(msg);
		}
		return DefWindowProc(hWnd,iMsg,wParam,lParam);
	}

	struct __trayIconData__ {
    	MSG msg;
		NOTIFYICONDATAA nid;
		HWND hwnd = NULL;
		HICON icon = NULL;
	};

	HICON CreateWin32IconFromImage(const sf::Image& image) {
		std::vector<uint8_t> png;
		if (!image.saveToMemory(png, "png")) return nullptr;
		return CreateIconFromResourceEx(&png[0], png.size(), true, 0x00030000, image.getSize().x, image.getSize().y, LR_DEFAULTCOLOR);
	}

	TrayIcon::TrayIcon(const sf::Image& icon, const std::string& tip) : data(new __trayIconData__()) {
		using namespace std::placeholders;

		data->icon = CreateWin32IconFromImage(icon);
		__id__++;
		id = __id__;
		std::string classname = "TRAY" + std::to_string(id);
		
		// Create the hidden window responsible for the message stack
		WNDCLASSEXA wc;
		memset(&wc, 0, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.lpfnWndProc = TrayWndMessage;
		wc.hInstance = GetModuleHandleA(NULL);
		wc.lpszClassName = classname.c_str();
		RegisterClassExA(&wc);
		data->hwnd = CreateWindowExA(0, (LPCSTR)classname.c_str(), NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		if (data->hwnd == NULL) {
			throw Battery::Exception("Failed to create the hidden window for the tray icon message stack");
		}
		UpdateWindow(data->hwnd);

		// Now create the tray icon
		memset(&data->nid, 0, sizeof(data->nid));
		data->nid.cbSize = sizeof(data->nid);
		data->nid.hWnd = data->hwnd;
		data->nid.uID = 0;
		data->nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		data->nid.uCallbackMessage = WM_USER + 1 + id;
		data->nid.hIcon = data->icon;
		strcpy(data->nid.szTip, tip.c_str());

		Shell_NotifyIconA(NIM_ADD, &data->nid);
		Shell_NotifyIconA(NIM_SETVERSION, &data->nid);
	}

	TrayIcon::~TrayIcon() {
    	Shell_NotifyIconA(NIM_DELETE, &data->nid);
		DestroyWindow(data->hwnd);
		DestroyIcon(data->icon);
	}

	void TrayIcon::update() {
        std::lock_guard<std::mutex> lock(messageStackMutex);
        while (!messageStack.empty()) {
            handleMessage(messageStack.front());
            messageStack.pop();
        }
	}

	void TrayIcon::attachLeftClickCallback(std::function<void(void)> callback) {
		leftClickCallback = callback;
	}

	void TrayIcon::attachRightClickCallback(std::function<void(void)> callback) {
		rightClickCallback = callback;
	}

    void TrayIcon::handleMessage(const TrayMessage& msg) {
        switch (msg.lParam) {

			case WM_LBUTTONDOWN:
				if (leftClickCallback) {
					leftClickCallback();
				}
				break;

			case WM_RBUTTONDOWN:
				if (rightClickCallback) {
					rightClickCallback();
				}
				break;
		}
    }

}

#endif // _WIN32
