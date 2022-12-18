
#ifdef __linux__

#include "Battery/Platform/Platform.h"
#include "Battery/Core/Exception.h"
#include "Battery/Utils/StringUtils.h"

#include <X11/Xatom.h>

#define _NET_WM_STATE_REMOVE        0    // remove/unset property
#define _NET_WM_STATE_ADD           1    // add/set property
#define _NET_WM_STATE_TOGGLE        2    // toggle property

#include <unistd.h>
#include <sys/file.h>

namespace Battery {

	void* platform_LockFileDescriptor(const std::string& file) {
		int fileDescriptor = open(file.c_str(), O_CREAT | O_RDWR, 0666);
		if (fileDescriptor <= 0) {
			throw Battery::NoSuchFileOrDirectoryException();
		}
		if (flock(fileDescriptor, LOCK_EX | LOCK_NB) != 0) {
			platform_UnlockFileDescriptor((void*)(size_t)fileDescriptor);
			throw Battery::LockfileUnavailableException();
		}
		return (void*)(size_t)fileDescriptor;
	}

	void platform_UnlockFileDescriptor(void* fileDescriptor) {
		flock((size_t)fileDescriptor, LOCK_UN);
		close((size_t)fileDescriptor);
	}

	bool platform_ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force) {
//#warning TODO: Implement unzipping
		return false;
	}

    std::pair<bool, size_t> platform_ExecuteShellCommandSilent(const std::string& command, bool hidden) {
		//system(command.c_str());
        return {};
	}

	#pragma warning ("TODO fix this shitty file")

	std::vector<uint8_t> platform_LoadEmbeddedResource(int id, const char* type) {
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}


#ifdef BATTERY_FEATURES_GRAPHICS
	bool platform_IsFocused(sf::WindowHandle window) {
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);

		/*Display* display = XOpenDisplay(NULL);
  		if (display == NULL)  {
			return false;
  		}

		XID focused;
		int revert_to;
		XGetInputFocus(display, &focused, &revert_to);
  		XCloseDisplay(display);

		return focused == al_get_x_window_id(allegroDisplayPointer);*/
	}

	void platform_Focus(sf::WindowHandle window) {
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
/*
		Display* display = XOpenDisplay(NULL);
  		if (display == NULL)  {
			return;
  		}

		XSetInputFocus(display, al_get_x_window_id(allegroDisplayPointer), RevertToNone, CurrentTime);
		XRaiseWindow(display, al_get_x_window_id(allegroDisplayPointer));
  		XCloseDisplay(display);*/
	}

	void platform_Hide(sf::WindowHandle window) {
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
/*
		Display* display = XOpenDisplay(NULL);
  		if (display == NULL)  {
			return;
  		}

		XUnmapWindow(display, al_get_x_window_id(allegroDisplayPointer));
  		XCloseDisplay(display);*/
	}

	void platform_Show(sf::WindowHandle window) {
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);

		/*Display* display = XOpenDisplay(NULL);
  		if (display == NULL)  {
			return;
  		}

		XMapWindow(display, al_get_x_window_id(allegroDisplayPointer));
  		XCloseDisplay(display);*/
	}

	void platform_HideFromTaskbar(sf::WindowHandle window) {
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);

		/*Display* display = XOpenDisplay(NULL);
  		if (display == NULL)  {
			return;
  		}

		XEvent event;
  		event.xclient.type = ClientMessage;
  		event.xclient.serial = 0;
  		event.xclient.send_event = True;
  		event.xclient.message_type = XInternAtom(display, "_NET_WM_STATE", False);
  		event.xclient.window = al_get_x_window_id(allegroDisplayPointer);
  		event.xclient.format = 32;
  		event.xclient.data.l[0] = _NET_WM_STATE_ADD;
  		event.xclient.data.l[1] = XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", False);

  		long mask = SubstructureRedirectMask | SubstructureNotifyMask;
  		XSendEvent(display, DefaultRootWindow(display), False, mask, &event);

		XCloseDisplay(display);*/
	}

	void platform_ShowInTaskbar(sf::WindowHandle window) {
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);

		/*Display* display = XOpenDisplay(NULL);
  		if (display == NULL)  {
			return;
  		}

		XEvent event;
  		event.xclient.type = ClientMessage;
  		event.xclient.serial = 0;
  		event.xclient.send_event = True;
  		event.xclient.message_type = XInternAtom(display, "_NET_WM_STATE", False);
  		event.xclient.window = al_get_x_window_id(allegroDisplayPointer);
  		event.xclient.format = 32;
  		event.xclient.data.l[0] = _NET_WM_STATE_REMOVE;
  		event.xclient.data.l[1] = XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", False);

  		long mask = SubstructureRedirectMask | SubstructureNotifyMask;
  		XSendEvent(display, DefaultRootWindow(display), False, mask, &event);

		XCloseDisplay(display);*/
	}

	void platform_SetWindowTransparent(sf::WindowHandle window, bool transparent) {
//#warning TODO : platform_SetWindowTransparent not implemented
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}

	void platform_SetWindowAlpha(sf::WindowHandle window, uint8_t alpha) {
//#warning TODO : platform_SetWindowAlpha not implemented
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}
#endif // BATTERY_FEATURES_GRAPHICS


	std::string platform_GetLastWin32ErrorAsString() {
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}










	MB_Status MessageBoxError(const std::string& message, const std::string& title, MB_Buttons buttons, int defaultButton) {
//#warning TODO: MessageBoxError not implemented
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}

	MB_Status MessageBoxWarning(const std::string& message, const std::string& title, MB_Buttons buttons, int defaultButton) {
//#warning TODO: MessageBoxWarning not implemented
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}

	MB_Status MessageBoxInfo(const std::string& message, const std::string& title, MB_Buttons buttons, int defaultButton) {
//#warning TODO: MessageBoxInfo not implemented
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}

	glm::vec2 GetUsableDesktopArea() {
//#warning TODO : GetUsableDesktopArea not implemented
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}
}

#endif // __linux__
