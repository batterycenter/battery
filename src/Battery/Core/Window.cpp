
#include "Battery/Core/Window.h"
#include "Battery/Core/Application.h"
#include "Battery/Core/Config.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/Renderer/Bitmap.h"
#include "Battery/Renderer/Renderer2D.h"
#include "Battery/Platform/Platform.h"
#include "clip.h"


// TODO: Handle crashes when allegro window fails, maybe try again

#undef CreateEvent

#define CHECK_ALLEGRO_INIT() \
	if (!Battery::AllegroContext::GetInstance()->IsInitialized()) {	\
		throw Battery::Exception(std::string(__FUNCTION__) + ": Allegro Context is not initialized!");	\
	}

namespace Battery {

	Window::Window(int w, int h) {
		width = w;
		height = h;
	}

	Window::~Window() {
		Destroy();
	}

	void Window::Create(int windowFlags) {
		LOG_CORE_TRACE("Creating Window");
		
		// Auto-fit window size
		if (width <= 0) width = GetApp().GetPrimaryMonitorSize().x;
		if (height <= 0) height = GetApp().GetPrimaryMonitorSize().y;

		// Finally create the window
		window.create(sf::VideoMode({ width, height }), BATTERY_DEFAULT_TITLE);
		if (!window.isOpen())
			throw Battery::Exception("Could not create SFML window! Please check the graphics drivers!");
		
		// The window is clean now, now it can be moved back to the center
		CenterOnPrimaryMonitor();
		
		// Window is created and valid
		LOG_CORE_TRACE("Created Allegro window");
	}

	void Window::Destroy() {
		if (window.isOpen()) {
			LOG_CORE_TRACE("Destroying window");
			window.close();
		}
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
		/*
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

		}*/
	}

	void Window::HandleEvent(Battery::Event* event) {

#ifdef BATTERY_DEBUG // TODO
		/*static const size_t length = 1024;
		char info[length];
		event->GetInfoString(info, length);
		LOG_CORE_TRACE("Handling Event {}", info);*/
#endif
		
		/*if (event->GetType() == Battery::EventType::WindowResize) {
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
		}*/
	}

	glm::ivec2 Window::GetScreenPosition() {
		auto [x, y] = window.getPosition();
		return { x, y };
	}

	void Window::SetScreenPosition(const glm::ivec2& position) {
		window.setPosition({ position.x, position.y });
	}
	
	bool Window::SetWindowSizeConstraints(const glm::ivec2& minimum, const glm::ivec2& maximum) {
		/*
		// Limit window size
		if (!al_set_window_constraints(allegroDisplayPointer, minimum.x, minimum.y, maximum.x, maximum.y)) {
			return false;
		}
		al_apply_window_constraints(allegroDisplayPointer, true);*/
		return true;
	}

	void Window::CenterOnPrimaryMonitor() {
		glm::vec2 monitor = Battery::GetApp().GetPrimaryMonitorSize();
		SetScreenPosition((monitor - glm::vec2(GetSize())) / 2.f + glm::vec2(0, -30));
	}

	int Window::GetWidth() {
		return GetSize().x;
	}

	int Window::GetHeight() {
		return GetSize().y;
	}

	glm::ivec2 Window::GetSize() {
		auto [x, y] = window.getSize();
		return { x, y };
	}

	void Window::SetSize(const glm::ivec2 size) {
		window.setSize({ (uint32_t)size.x, (uint32_t)size.y });
	}

	void Window::SetTitle(const std::string title) {
		window.setTitle(title);
	}

	void Window::Maximize() {
		//al_set_display_flag(allegroDisplayPointer, ALLEGRO_MAXIMIZED, true);
	}

	void Window::Minimize() {
		//al_set_display_flag(allegroDisplayPointer, ALLEGRO_MINIMIZED, true);
	}

	void Window::Restore() {
		//al_set_display_flag(allegroDisplayPointer, ALLEGRO_MAXIMIZED, false);
		//al_set_display_flag(allegroDisplayPointer, ALLEGRO_MINIMIZED, false);
	}

	sf::WindowHandle Window::GetSystemHandle() {
		return window.getSystemHandle();
	}

	bool Window::IsFocused() {
		//return platform_IsFocused(allegroDisplayPointer);
		return false;
	}

	void Window::Focus() {
		//platform_Focus(allegroDisplayPointer);
	}

	void Window::Hide() {
		//platform_Hide(allegroDisplayPointer);
	}

	void Window::Show() {
		//platform_Show(allegroDisplayPointer);
	}

	void Window::HideFromTaskbar() {
		//platform_HideFromTaskbar(allegroDisplayPointer);
	}

	void Window::ShowInTaskbar() {
		//platform_ShowInTaskbar(allegroDisplayPointer);
	}

	void Window::SetFrameless(bool frameless) {
		//al_set_display_flag(allegroDisplayPointer, ALLEGRO_FRAMELESS, frameless);
	}

	void Window::FlipDisplay() {
		//al_set_target_backbuffer(allegroDisplayPointer);
		//al_flip_display();
	}

	ClipboardFormatID Window::RegisterClipboardFormat(const std::string& format) {
		return clip::register_format(format);
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
		//return al_set_system_mouse_cursor(allegroDisplayPointer, (ALLEGRO_SYSTEM_MOUSE_CURSOR)cursorID);
		return false;
	}

#ifdef _WIN32
	bool Window::SetWindowExecutableIcon(int iconID) {
		/*
		// Load the embedded icon to the Allegro window so no external 
		// icon resource is needed
		HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(iconID));

		if (!icon)
			return false;

		HWND winhandle = al_get_win_window_handle(allegroDisplayPointer);
		SetClassLongPtr(winhandle, GCLP_HICON, (LONG_PTR)icon);
		SetClassLongPtr(winhandle, GCLP_HICONSM, (LONG_PTR)icon);

		return true;*/
		return false;
	}
#endif

}
