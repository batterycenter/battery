
#include "Battery/pch.h"
#include "Battery/Core/ImGuiLayer.h"
#include "Battery/Core/AllegroContext.h"

namespace Battery {

	bool __acknowledgeResize(ALLEGRO_DISPLAY* display) {
		return al_acknowledge_resize(display);
	}

}
