
#include "Battery/pch.h"
#include "Battery/Core/AllegroContext.h"
#include "Battery/Platform/Dialog.h"

#ifdef _WIN32
namespace Battery {

	void ShowErrorMessageBox(const std::string& msg, ALLEGRO_DISPLAY* display) {
		al_show_native_message_box(display, "Error", msg.c_str(), "", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
	}

	bool ShowErrorMessageBoxYesNo(const std::string& msg, ALLEGRO_DISPLAY* display) {
		return 1 == al_show_native_message_box(display, "Error", msg.c_str(), "", nullptr, ALLEGRO_MESSAGEBOX_ERROR | ALLEGRO_MESSAGEBOX_YES_NO);
	}

	bool ShowErrorMessageBoxOkCancel(const std::string& msg, ALLEGRO_DISPLAY* display) {
		return 1 == al_show_native_message_box(display, "Error", msg.c_str(), "", nullptr, ALLEGRO_MESSAGEBOX_ERROR | ALLEGRO_MESSAGEBOX_OK_CANCEL);
	}

	void ShowWarningMessageBox(const std::string& msg, ALLEGRO_DISPLAY* display) {
		al_show_native_message_box(display, "Warning", msg.c_str(), "", nullptr, ALLEGRO_MESSAGEBOX_WARN);
	}

	bool ShowWarningMessageBoxYesNo(const std::string& msg, ALLEGRO_DISPLAY* display) {
		return 1 == al_show_native_message_box(display, "Warning", msg.c_str(), "", nullptr, ALLEGRO_MESSAGEBOX_WARN | ALLEGRO_MESSAGEBOX_YES_NO);
	}

	bool ShowWarningMessageBoxOkCancel(const std::string& msg, ALLEGRO_DISPLAY* display) {
		return 1 == al_show_native_message_box(display, "Warning", msg.c_str(), "", nullptr, ALLEGRO_MESSAGEBOX_WARN | ALLEGRO_MESSAGEBOX_OK_CANCEL);
	}

	void ShowInfoMessageBox(const std::string& msg, ALLEGRO_DISPLAY* display) {
		al_show_native_message_box(display, "Info", msg.c_str(), "", nullptr, ALLEGRO_MESSAGEBOX_QUESTION);
	}

	bool ShowInfoMessageBoxYesNo(const std::string& msg, ALLEGRO_DISPLAY* display) {
		return 1 == al_show_native_message_box(display, "Info", msg.c_str(), "", nullptr, ALLEGRO_MESSAGEBOX_QUESTION | ALLEGRO_MESSAGEBOX_YES_NO);
	}

	bool ShowInfoMessageBoxOkCancel(const std::string& msg, ALLEGRO_DISPLAY* display) {
		return 1 == al_show_native_message_box(display, "Info", msg.c_str(), "", nullptr, ALLEGRO_MESSAGEBOX_QUESTION | ALLEGRO_MESSAGEBOX_OK_CANCEL);
	}

}
#endif