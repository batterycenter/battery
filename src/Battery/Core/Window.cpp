
#include "Battery/pch.h"
#include "Battery/Core/Window.h"
#include "Battery/Core/Application.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Core/Config.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/Renderer/Bitmap.h"
#include "Battery/Renderer/Renderer2D.h"
#include "Battery/Core/AllegroContext.h"
#include "clip.h"

// TODO: Do something about allegro default font

// TODO: Handle crashes when allegro window fails, maybe try again

#undef CreateEvent

#define CHECK_ALLEGRO_INIT() \
	if (!Battery::AllegroContext::GetInstance()->IsInitialized()) {	\
		throw Battery::Exception(std::string(__FUNCTION__) + ": Allegro Context is not initialized!");	\
	}

namespace Battery {

	// This function is local within this .cpp file
	static ALLEGRO_COLOR ConvertAllegroColor(const glm::vec4& color) {
		return al_map_rgba(color.r, color.g, color.b, color.a);
	}



	Window::Window(int w, int h) {
		width = std::max(w, BATTERY_MIN_WINDOW_WIDTH);
		height = std::max(h, BATTERY_MIN_WINDOW_HEIGHT);
	}

	Window::~Window() {
		if (valid) {
			Destroy();
		}
	}

	void Window::Create(int windowFlags) {
		CHECK_ALLEGRO_INIT();

		if (valid) {
			LOG_CORE_WARN("Can't create window: Already exists!");
			return;
		}

		if (!al_is_system_installed())
			throw Battery::Exception("Failed to create Allegro window: Allegro is not initialized!");

		// Window flags are Battery flags, need to be converted now
		int allegroFlags = ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_OPENGL | ALLEGRO_RESIZABLE;
		bool frameless = false;
		bool noTaskbar = false;
		bool hidden = false;

		if (windowFlags & (int)WindowFlags::NON_RESIZABLE) {
			allegroFlags &= ~ALLEGRO_RESIZABLE;
		}
		if (windowFlags & (int)WindowFlags::FULLSCREEN) {
			allegroFlags |= ALLEGRO_FULLSCREEN;
		}
		if (windowFlags & (int)WindowFlags::WINDOWED_FULLSCREEN) {
			allegroFlags |= ALLEGRO_FULLSCREEN_WINDOW;
		}
		if (windowFlags & (int)WindowFlags::FRAMELESS) {
			allegroFlags |= ALLEGRO_FRAMELESS;
			frameless = true;
		}
		if (windowFlags & (int)WindowFlags::NO_TASKBAR) {
			noTaskbar = true;
		}
		if (windowFlags & (int)WindowFlags::HIDDEN) {
			hidden = true;
		}
												// Move the window very far to the bottom temporarily 
		al_set_new_window_position(0, 3000);	// to prevent visible flickering
		
		al_set_new_display_flags(allegroFlags);
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

		allegroDisplayPointer = al_create_display(width, height);
		if (allegroDisplayPointer == nullptr)
			throw Battery::Exception("Failed to create Allegro window! Please check the graphics drivers!");
		
		// Immediately draw the background color
		if (hidden) {
			//Hide();
			#warning Implement Hiding
		}
		al_clear_to_color(ConvertAllegroColor(BATTERY_DEFAULT_BACKGROUND_COLOR));
		al_flip_display();

		// Now show or hide the window in the taskbar
		if (noTaskbar) {
			//HideFromTaskbar();
			#warning Implement Hiding
		}
		else {
			//ShowInTaskbar();
			#warning Implement Hiding
		}

		// The window is clean now, now it can be moved back to the center
		CenterOnPrimaryMonitor();
		
		//allegroDefaultFont = al_load_ttf_font(defaultFontFile.c_str(), 64, 0);
		//if (allegroDefaultFont == nullptr) {
		//	al_destroy_display(allegroDisplayPointer);
		//	allegroDisplayPointer = nullptr;
		//	throw Battery::Exception("Failed to load Allegro font " + defaultFontFile);
		//}

		allegroEventQueue = al_create_event_queue();
		if (allegroEventQueue == nullptr) {
			al_destroy_display(allegroDisplayPointer);
			//al_destroy_font(allegroDefaultFont);
			allegroDisplayPointer = nullptr;
			//allegroDefaultFont = nullptr;
			throw Battery::Exception("Failed to create Allegro event queue");
		}

		// Attach event sources
		al_register_event_source(allegroEventQueue, al_get_mouse_event_source());
		al_register_event_source(allegroEventQueue, al_get_keyboard_event_source());
		al_register_event_source(allegroEventQueue, al_get_display_event_source(allegroDisplayPointer));

		// Limit window size
		if (!al_set_window_constraints(allegroDisplayPointer, BATTERY_MIN_WINDOW_WIDTH, BATTERY_MIN_WINDOW_HEIGHT, 0, 0)) {
			LOG_CORE_WARN("{}: {}", __FUNCTION__, "Window constraints could not be set!");
		}
		al_apply_window_constraints(allegroDisplayPointer, true);
		
		// Set the window title
		SetTitle(BATTERY_DEFAULT_TITLE);
		
		// Window is created and valid
		LOG_CORE_TRACE("Created Allegro window");
		valid = true;
	}

	void Window::Destroy() {
		// Destroy everything
		LOG_CORE_TRACE("Destroying Allegro window");
		al_destroy_display(allegroDisplayPointer);
		//al_destroy_font(allegroDefaultFont);
		al_destroy_event_queue(allegroEventQueue);
		allegroDisplayPointer = nullptr;
		//allegroDefaultFont = nullptr;
		allegroEventQueue = nullptr;
		valid = false;
	}

	void Window::SetEventCallback(std::function<void(Battery::Event* event)> eventCallback) {
		if (eventCallback != nullptr) {
			this->eventCallback = eventCallback;
		}
		else {
			LOG_CORE_ERROR("Window: Event Callback was nullptr!");
		}
	}

	void Window::HandleEvents() {
		CHECK_ALLEGRO_INIT();
		//PROFILE_CORE_SCOPE("{}: {}", __FUNCTION__, "");

		if (eventCallback != nullptr && allegroEventQueue != nullptr) {

			while (!al_is_event_queue_empty(allegroEventQueue)) {
				ALLEGRO_EVENT allegroEvent;

				if (al_get_next_event(allegroEventQueue, &allegroEvent)) {

					eventContainer.Load(&allegroEvent);

					if (eventContainer.primaryEventType != Battery::EventType::NONE) {
						HandleEvent(eventContainer.primaryEvent);
					}
					
					if (eventContainer.secondaryEventType != Battery::EventType::NONE) {
						HandleEvent(eventContainer.secondaryEvent);
					}
				}
			}

		}
	}

	void Window::HandleEvent(Battery::Event* event) {

#ifdef BATTERY_DEBUG // TODO
		static const size_t length = 1024;
		char info[length];
		event->GetInfoString(info, length);
		LOG_CORE_TRACE("Handling Event {}", info);
#endif
		
		if (event->GetType() == Battery::EventType::WindowResize) {
			al_acknowledge_resize(allegroDisplayPointer);
		}

		// Clear keyboard buffer when window loses focus
		if (event->GetType() == Battery::EventType::WindowLostFocus) {
			LOG_CORE_TRACE("{}: {}", __FUNCTION__, "Window lost focus: Clearing keyboard state");
			al_clear_keyboard_state(allegroDisplayPointer);
		}

		if (eventCallback != nullptr) {
			eventCallback(event);
		}
		else {
			LOG_CORE_TRACE("{}: {}", __FUNCTION__, "OnEvent callback can't be called: Function pointer is nullptr!");
		}
	}

	glm::ivec2 Window::GetScreenPosition() {
		CHECK_ALLEGRO_INIT();
		glm::ivec2 position;
		al_get_window_position(allegroDisplayPointer, &position.x, &position.y);
		return position;
	}

	void Window::SetScreenPosition(const glm::ivec2& position) {
		CHECK_ALLEGRO_INIT();
		al_set_window_position(allegroDisplayPointer, position.x, position.y);
	}

	void Window::CenterOnPrimaryMonitor() {
		CHECK_ALLEGRO_INIT(); 
		glm::vec2 monitor = Battery::GetApp().GetPrimaryMonitorSize();
		SetScreenPosition((monitor - glm::vec2(GetSize())) / 2.f + glm::vec2(0, -30));
	}

	int Window::GetWidth() {
		CHECK_ALLEGRO_INIT();
		return al_get_display_width(allegroDisplayPointer);
	}

	int Window::GetHeight() {
		CHECK_ALLEGRO_INIT();
		return al_get_display_height(allegroDisplayPointer);
	}

	glm::ivec2 Window::GetSize() {
		CHECK_ALLEGRO_INIT();
		return glm::ivec2(GetWidth(), GetHeight());
	}

	void Window::SetSize(const glm::vec2 size) {
		CHECK_ALLEGRO_INIT();
		al_resize_display(allegroDisplayPointer, 
			std::max((int)size.x, BATTERY_MIN_WINDOW_WIDTH), std::max((int)size.y, BATTERY_MIN_WINDOW_HEIGHT));
	}

	void Window::SetTitle(const std::string title) {
		CHECK_ALLEGRO_INIT();
		al_set_window_title(allegroDisplayPointer, title.c_str());
	}

	void Window::Maximize() {
		CHECK_ALLEGRO_INIT();
		al_set_display_flag(allegroDisplayPointer, ALLEGRO_MAXIMIZED, true);
	}

	void Window::Minimize() {
		CHECK_ALLEGRO_INIT();
		al_set_display_flag(allegroDisplayPointer, ALLEGRO_MINIMIZED, true);
	}

	void Window::Restore() {
		CHECK_ALLEGRO_INIT();
		al_set_display_flag(allegroDisplayPointer, ALLEGRO_MAXIMIZED, false);
		al_set_display_flag(allegroDisplayPointer, ALLEGRO_MINIMIZED, false);
	}

	glm::vec2 Window::GetMousePosition() {
		CHECK_ALLEGRO_INIT();
		ALLEGRO_MOUSE_STATE mouse;
		al_get_mouse_state(&mouse);
		return glm::vec2(mouse.x, mouse.y);
	}

	bool Window::GetLeftMouseButton() {
		CHECK_ALLEGRO_INIT();
		ALLEGRO_MOUSE_STATE mouse;
		al_get_mouse_state(&mouse);
		return mouse.buttons & 0x01;
	}

	bool Window::GetRightMouseButton() {
		CHECK_ALLEGRO_INIT();
		ALLEGRO_MOUSE_STATE mouse;
		al_get_mouse_state(&mouse);
		return mouse.buttons & 0x02;
	}

	bool Window::GetMouseWheel() {
		CHECK_ALLEGRO_INIT();
		ALLEGRO_MOUSE_STATE mouse;
		al_get_mouse_state(&mouse);
		return mouse.buttons & 0x04;
	}

#ifdef _WIN32
	HWND Window::GetWinHandle() {
		CHECK_ALLEGRO_INIT();
		return al_get_win_window_handle(allegroDisplayPointer);
	}

	bool Window::IsFocused() {
		CHECK_ALLEGRO_INIT();
		return GetForegroundWindow() == GetWinHandle();
	}

	bool Window::Focus() {
		CHECK_ALLEGRO_INIT();
		return SetForegroundWindow(GetWinHandle());
	}

	bool Window::Hide() {
		CHECK_ALLEGRO_INIT();
		return ShowWindow(GetWinHandle(), SW_HIDE);
	}

	bool Window::Show() {
		CHECK_ALLEGRO_INIT();
		return ShowWindow(GetWinHandle(), SW_SHOW);
	}

	void Window::HideFromTaskbar() {
		long style = GetWindowLong(GetWinHandle(), GWL_STYLE);
		style |= WS_VISIBLE;
		ShowWindow(GetWinHandle(), SW_HIDE);
		SetWindowLong(GetWinHandle(), GWL_STYLE, style);
		ShowWindow(GetWinHandle(), SW_SHOW);
		Focus();
	}

	void Window::ShowInTaskbar() {
		long style = GetWindowLong(GetWinHandle(), GWL_STYLE);
		style &= ~(WS_VISIBLE);
		SetWindowLong(GetWinHandle(), GWL_STYLE, style);
		ShowWindow(GetWinHandle(), SW_SHOW);
	}
#endif

	void Window::SetFrameless(bool frameless) {
		al_set_display_flag(allegroDisplayPointer, ALLEGRO_FRAMELESS, frameless);
	}

	void Window::FlipDisplay() {
		al_set_target_backbuffer(allegroDisplayPointer);
		al_flip_display();
	}

	ClipboardFormatID Window::RegisterClipboardFormat(const std::string& format) {
		clip::format id = clip::register_format(format);
		return id;
	}

	bool Window::HasClipboardText() {
		return clip::has(clip::text_format());
	}

	bool Window::HasClipboardImage() {
		return clip::has(clip::image_format());
	}

	bool Window::HasClipboardFormat(ClipboardFormatID format) {
		return clip::has(format);
	}

	std::pair<std::string, bool> Window::GetClipboardText() {
		
		if (clip::has(clip::text_format())) {
			std::string str;
			if (clip::get_text(str)) {
				return std::make_pair(str, true);
			}
		}

		return std::make_pair("", false);
	}

	std::optional<Battery::Bitmap> Window::GetClipboardImage() {

		if (clip::has(clip::image_format())) {
			clip::image image;

			if (!clip::get_image(image))
				return std::nullopt;

			if (!image.is_valid())
				return std::nullopt;

			return std::make_optional<Battery::Bitmap>(Battery::Bitmap(image));
		}

		return std::nullopt;
	}

	std::optional<std::vector<uint8_t>> Window::GetClipboardCustomFormat(ClipboardFormatID customFormat) {
		clip::lock lock;

		if (!lock.is_convertible(customFormat))
			return std::nullopt;

		size_t length = lock.get_data_length(customFormat);
		std::vector<uint8_t> data(length, 0);

		if (!lock.get_data(customFormat, (char*)&data[0], length))
			return std::nullopt;

		return std::make_optional<std::vector<uint8_t>>(std::move(data));
	}

	std::optional<std::string> Window::GetClipboardCustomFormatString(ClipboardFormatID customFormat) {
		auto opt = GetClipboardCustomFormat(customFormat);

		if (!opt.has_value())
			return std::nullopt;

		std::string str((char*)&opt.value()[0], opt.value().size());
		return std::make_optional<std::string>(std::move(str));
	}

	bool Window::SetClipboardText(const std::string& text) {
		return clip::set_text(text);
	}

	bool Window::SetClipboardImage(const Battery::Bitmap& image) {
		auto img = image.GetClipImage();

		if (!img.has_value())
			return false;

		return clip::set_image(clip::image(&img.value().first[0], img.value().second));
	}

	bool Window::SetClipboardCustomFormat(ClipboardFormatID customFormat, void* data, size_t size) {
		clip::lock lock; 
		lock.clear();
		return lock.set_data(customFormat, (char*)data, size);
	}

	bool Window::SetClipboardCustomFormatString(ClipboardFormatID customFormat, const std::string& string) {
		return SetClipboardCustomFormat(customFormat, (void*)string.c_str(), string.size());
	}

	bool Window::SetMouseCursor(int cursorID) {
		return al_set_system_mouse_cursor(allegroDisplayPointer, (ALLEGRO_SYSTEM_MOUSE_CURSOR)cursorID);
	}

#ifdef _WIN32
	bool Window::SetWindowExecutableIcon(int iconID) {
		CHECK_ALLEGRO_INIT();

		// Load the embedded icon to the Allegro window so no external 
		// icon resource is needed
		HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(iconID));

		if (!icon)
			return false;

		HWND winhandle = al_get_win_window_handle(allegroDisplayPointer);
		SetClassLongPtr(winhandle, GCLP_HICON, (LONG_PTR)icon);
		SetClassLongPtr(winhandle, GCLP_HICONSM, (LONG_PTR)icon);

		return true;
	}
#endif

}
