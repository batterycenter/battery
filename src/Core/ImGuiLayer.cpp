
#include "Battery/pch.h"
#include "Battery/Core/ImGuiLayer.h"
#include "Battery/AllegroDeps.h"

namespace Battery {

	// TODO: Fix this weird pointer, make a singleton?

	Battery::ImGuiLayer<Battery::FontContainer>* __userInterface = nullptr;

	Battery::ImGuiLayer<Battery::FontContainer>* __getUserInterface() {
		return __userInterface;
	}

	void __setUserInterface(Battery::ImGuiLayer<Battery::FontContainer>* ui) {
		__userInterface = ui;
	}

	bool __acknowledgeResize(ALLEGRO_DISPLAY* display) {
		return al_acknowledge_resize(display);
	}

}
