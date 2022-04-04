#pragma once

#include "Battery/Core/Exception.h"
#include "Battery/Core/ApplicationEvents.h"
#include "Battery/Renderer/Bitmap.h"

// Forward declarations for the Windows handle
struct HWND__;
typedef HWND__* HWND;

struct ALLEGRO_DISPLAY;
struct ALLEGRO_EVENT_QUEUE;
struct ALLEGRO_FONT;

namespace Battery {

	typedef clip::format ClipboardFormatID;

	enum class ClipboardFormat {
		NONE  = 0,
		TEXT  = 1,
		IMAGE = 2
	};

	class Window {
	public:
		Window(int w, int h);
		~Window();

		void Create(int windowFlags = 0);
		void Destroy();
		void SetEventCallback(std::function<void(Battery::Event* event)> eventCallback);
		void HandleEvents();
		void HandleEvent(Battery::Event* event);

		glm::ivec2 GetScreenPosition();
		void SetScreenPosition(const glm::ivec2& position);
		void CenterOnPrimaryMonitor();
		int GetWidth();
		int GetHeight();
		glm::ivec2 GetSize();
		void SetSize(const glm::vec2 size);
		void SetTitle(const std::string title);		// TODO: Get window title
		void Maximize();
		void Minimize();
		void Restore();
		glm::vec2 GetMousePosition();
		bool GetLeftMouseButton();
		bool GetRightMouseButton();
		bool GetMouseWheel();
		
#ifdef _WIN32
		HWND GetWinHandle();
		bool IsFocused();
		bool Focus();
		bool Hide();
		bool Show();
		void HideFromTaskbar();
		void ShowInTaskbar();
#endif

		void SetFrameless(bool frameless);
		void FlipDisplay();

		ClipboardFormatID RegisterClipboardFormat(const std::string& format);
		bool HasClipboardText();
		bool HasClipboardImage();
		bool HasClipboardFormat(ClipboardFormatID format);
		std::pair<std::string, bool> GetClipboardText();
		std::optional<Battery::Bitmap> GetClipboardImage();
		std::optional<std::vector<uint8_t>> GetClipboardCustomFormat(ClipboardFormatID customFormat);
		std::optional<std::string> GetClipboardCustomFormatString(ClipboardFormatID customFormat);
		bool SetClipboardText(const std::string& text);
		bool SetClipboardImage(const Battery::Bitmap& image);
		bool SetClipboardCustomFormat(ClipboardFormatID customFormat, void* data, size_t size);
		bool SetClipboardCustomFormatString(ClipboardFormatID customFormat, const std::string& string);

		/// <summary>
		/// This function sets the cursor icon. It must be called for every mouse event.
		/// Parameter is the ALLEGRO_SYSTEM_MOUSE_CURSOR enum
		/// </summary>
		bool SetMouseCursor(int cursorID);

		/// <summary>
		/// Parameter is the ID of an icon, which is embedded in the executable using a .rc resource file.
		/// </summary>
		/// <param name="iconID">- the id of the embedded icon in the executable.</param>
		/// <returns>bool - if loading the icon was successful</returns>
#ifdef _WIN32
		bool SetWindowExecutableIcon(int iconID);
#endif

		ALLEGRO_DISPLAY* allegroDisplayPointer = nullptr;
		ALLEGRO_EVENT_QUEUE* allegroEventQueue = nullptr;
		ALLEGRO_FONT* allegroDefaultFont = nullptr;

	private:
		std::string defaultFontFile = "C:\\Windows\\Fonts\\consola.ttf";

		// These are only for remembering the size until the window is created
		int width = 0;
		int height = 0;
		bool valid = false;

		ParentEventContainer eventContainer;
		std::function<void(Battery::Event* event)> eventCallback = nullptr;
	};

}
