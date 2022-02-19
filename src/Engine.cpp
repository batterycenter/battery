
#include "Battery/pch.h"
#include "Battery/Core.h"
#include "Battery/Core/Exception.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/StringUtils.h"

// ImGui library
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui.h"
#include "imgui_impl_allegro5.h"

/*

	bool Engine::SetWindowIcon(const std::string& file) {

		std::string e = StringUtils::ToLowerCase(FileUtils::GetExtensionFromPath(file));

		if (e == ".bmp" ||
			e == ".dds" ||
			e == ".pcx" ||
			e == ".tga" ||
			e == ".jpg" ||
			e == ".jpeg" ||
			e == ".png")
		{
			if (!FileUtils::FileExists(file))
				return false;

			ALLEGRO_BITMAP* bitmap = al_load_bitmap(file.c_str());

			if (bitmap == nullptr)
				return false;

			al_set_display_icon(display, bitmap);
			al_destroy_bitmap(bitmap);

			return true;
		}

		throw Battery::Exception("Engine::SetWindowIcon(): The specified icon file has an unsupported file format!");
	}

	void Engine::PreUpdate() {
		width = al_get_display_width(display);
		height = al_get_display_height(display);

		framecount++;
		frametime = (TimeUtils::GetMicroseconds() - previousUpdate) / 1000000.f;
		previousUpdate = TimeUtils::GetMicroseconds();
		framerate = 1.f / frametime;

		ALLEGRO_MOUSE_STATE state;
		al_get_mouse_state(&state);
		pmouse = mouse;
		mouse = { state.x, state.y };
		glm::ivec2 scrolled = mouseScroll - glm::ivec2(state.w, state.z);
		mouseScroll = glm::ivec2(state.w, state.z);
		//if (scrolled.x != 0 || scrolled.y != 0)
		//	mouseScrolled(scrolled.x, scrolled.y);


		Graphics::DrawBackground(BATTERY_DEFAULT_BACKGROUND_COLOR);
	}

}
*/