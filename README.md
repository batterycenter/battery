# Battery

<img src="assets/battery.png" width="30%" align="right">

<a href="https://www.flaticon.com/free-icon/battery_3165660" title="battery icons">Battery icon is provided by Freepik - Flaticon</a>  
Library Documentation WIP: https://batterycenter.github.io/battery/

# Disclaimer

This library is currently under heavy development and is not ready for production use whatsoever. Everything is subject to change.

`include/Battery` and `src/Battery` are subject to be removed entirely.

# The Pitch

Battery is a modern C++ library that aims to provide a simple, yet powerful API for creating cross-platform desktop applications. It tries to hold your hand all the way through your process of developing a new open-source desktop app, from project setup over building, all the way to releasing it on GitHub Releases, including One-Click installers and automatic updates.

It fills a lot of gaps in the standard library like guaranteed UTF-8 support on every supported platform, or useful utilities like cache files, or the ability to embed a resource file directly into your executable at compile time.

The frontend is built on top of Dear ImGui, but it offers a very good default look and many different default themes, as well as the ability to easily create complex custom Widgets using ImGui. The UI loop can either be written in C++ directly, or in Python, which allows live-reloading on your ImGui UI.

Battery aims to be the next big thing in the world of desktop applications after Qt has entered the market. It tries to breath fresh air into the desktop application market, allowing you to have a modern, beautiful, cross-platform application like with Electron.js, less than 10Mb hello world filesize, the native performance of C++, customizability like in Qt, but not gigabytes of dependencies like Qt, the simplicity of one-click installers, and automatic updates like in VS-Code.

Let's make the world a better place by writing software that is fast and just works ❤️

# License

Battery is licensed under the Apache License 2.0.  
You ARE allowed to:
 - Use the library and all provided tools for any purpose personally or commercially ✅
 - Modify them, as long as the license remains unchanged and all modifications are clearly marked ✅
 - Distribute any software that depends upon battery as a dependency ✅
 - Redistribute battery in source or binary form as part of another project, while retaining the license ✅
 - Use battery for developing closed source applications commercially ✅

But you are NOT allowed to:
 - Redistribute the battery library itself or large portions of it, claiming you are the original author ❌
 - Sell the code without major modifications, with the goal of making money off of my work ❌
 - Battery is distributed without any warranty whatsoever ❌

Battery is striving to become a healthy community of developers. Everything is allowed that contributes
to the goal of supporting teams in developing and distributing modern, free software.  
We love open source solutions that just work ❤️ 

# WIP stuff

```
sudo apt install git cmake build-essential libx11-dev
```

# Embedded dependencies

https://github.com/gabime/spdlog -> MIT License
https://github.com/DaanDeMeyer/reproc -> MIT License
https://github.com/nemtrif/utfcpp -> Boost Software License 1.0
https://github.com/jothepro/doxygen-awesome-css -> MIT License

# Other stuff

https://google.github.io/styleguide/cppguide.html

https://github.com/TartanLlama/expected -> CC0 1.0 License

https://github.com/zserge/tray -> modified for use with modern C++

https://github.com/CLIUtils/CLI11 -> BSD-Clause, modified to use Battery::FS::ifstream instead of std::ifstream

https://github.com/agauniyal/rang -> Unlicensed

https://github.com/Neargye/semver -> MIT License

https://github.com/cpm-cmake/CPM.cmake -> MIT License

https://github.com/Soundux/traypp -> MIT License

https://www.reichwein.it/unicode/ -> CC0 1.0 Universal (Like MIT)

https://github.com/JuliaStrings/utf8proc -> MIT "expat"

https://github.com/reunanen/cpp-exclusive-lock-file -> MIT License

TODO: Add unit testing

TODO: Add pipelines and automate testing

TODO: Fix build system and the dependency problem (what includes what and what the examples need)

TODO: Setup examples so that they have their own copy of battery

TODO: Properly implement install scripts for installing the CMake library and the resulting binaries

TODO: Check support for network drives (filesystem)

TODO: Allow battery_embed() CMake command take absolute paths

# Inspiration

![Battery1](assets/Battery1.png)

# Old code

```cpp
bool platform_ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force) {

	if (!FS::exists(file))
		return false;

	FS::create_directories(targetDirectory);

	std::string command = std::string("powershell Expand-Archive ") + file.c_str() + " " + targetDirectory.c_str();

	if (force)
		command += " -Force";

	auto[success, returnCode] = FS::ExecuteShellCommandSilent(command, true);
	return success && returnCode == 0;
}

bool platform_IsFocused(sf::WindowHandle window) {
	return GetForegroundWindow() == window;
}

void platform_Focus(sf::WindowHandle window) {
	SetForegroundWindow(window);
}

void platform_Hide(sf::WindowHandle window) {
	ShowWindow(window, SW_HIDE);
}

void platform_Show(sf::WindowHandle window) {
	ShowWindow(window, SW_SHOW);
}

void platform_HideFromTaskbar(sf::WindowHandle window) {
	long style = GetWindowLongW(window, GWL_STYLE);
	style |= WS_VISIBLE;
	ShowWindow(window, SW_HIDE);
	SetWindowLongW(window, GWL_STYLE, style);
	ShowWindow(window, SW_SHOW);
}

void platform_ShowInTaskbar(sf::WindowHandle window) {
	long style = GetWindowLongW(window, GWL_STYLE);
	style &= ~(WS_VISIBLE);
	SetWindowLongW(window, GWL_STYLE, style);
	ShowWindow(window, SW_SHOW);
}

void platform_SetWindowTransparent(sf::WindowHandle window, bool transparent) {
	if (transparent) {
		SetWindowLongPtrW(window, GWL_EXSTYLE, GetWindowLongPtr(window, GWL_EXSTYLE) | WS_EX_LAYERED);
	}
	else {
		SetWindowLongPtrW(window, GWL_EXSTYLE, GetWindowLongPtr(window, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		RedrawWindow(window, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	}
}

void platform_SetWindowAlpha(sf::WindowHandle window, uint8_t alpha) {
	SetLayeredWindowAttributes(window, 0, alpha, LWA_ALPHA);
}

glm::vec2 GetUsableDesktopArea() {
	RECT xy;
	BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA, 0, &xy, 0);
	return { xy.right - xy.left, xy.bottom - xy.top };
}
```

```cpp

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


#ifdef B_MODULE_LOADED_graphics
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
#endif


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
```