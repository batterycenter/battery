
#include "Battery/pch.h"
#include "Battery/Core/AllegroContext.h"

namespace Battery {

	AllegroContext::AllegroContext() {
	}

	AllegroContext::~AllegroContext() {
		if (IsInitialized()) {
			LOG_CORE_WARN("The Allegro context is being destroyed in the destructor: "
						  "It should have been destroyed earlier explicitly!");
			Destroy();
		}
	}

	bool AllegroContext::Initialize(const std::string& applicationName) {

		if (IsInitialized()) {
			LOG_CORE_WARN("The Allegro context is already initialized!");
			return true;
		}

		LOG_CORE_TRACE("Initializing Allegro context...");

		// Set the application name for the %appdata% paths
		al_set_app_name(applicationName.c_str());
		LOG_CORE_INFO("Global Application folder name is '" + applicationName + "'");

		// Initialize the Allegro Framework and all components
		if (!al_init()) {
			LOG_CORE_CRITICAL("Allegro could not be initialized! Please check the graphics drivers!"); 
			ShowErrorMessageBox("Allegro could not be initialized! Please check the graphics drivers!");
			return false;
		}

		if (!al_init_font_addon()) {
			LOG_CORE_CRITICAL("Failed to initialize the Allegro font addon!");
			ShowErrorMessageBox("Failed to initialize the Allegro font addon!");
			return false;
		}

		if (!al_init_ttf_addon()) {
			LOG_CORE_CRITICAL("Failed to initialize the Allegro TrueType font addon!");
			ShowErrorMessageBox("Failed to initialize the Allegro TrueType font addon!");
			return false;
		}

		if (!al_init_primitives_addon()) {
			LOG_CORE_CRITICAL("Failed to initialize the Allegro Primitives addon!");
			ShowErrorMessageBox("Failed to initialize the Allegro Primitives addon!");
			return false;
		}

		if (!al_init_image_addon()) {
			LOG_CORE_CRITICAL("Failed to initialize the Allegro Image addon!");
			ShowErrorMessageBox("Failed to initialize the Allegro Image addon!");
			return false;
		}

		if (!al_install_keyboard()) {
			LOG_CORE_CRITICAL("Failed to install the Allegro keyboard module!");
			ShowErrorMessageBox("Failed to install the Allegro keyboard module!");
			return false;
		}

		if (!al_install_mouse()) {
			LOG_CORE_CRITICAL("Failed to install the Allegro mouse module!");
			ShowErrorMessageBox("Failed to install the Allegro mouse module!");
			return false;
		}

		LOG_CORE_TRACE("Allegro context created");

		return true;
	}

	bool AllegroContext::IsInitialized() {
		return al_is_system_installed();
	}

	void AllegroContext::Destroy() {
		if (IsInitialized()) {
			LOG_CORE_TRACE("Destroying Allegro context...");

			// This function automatically destroys all submodules
			al_uninstall_system();

			LOG_CORE_TRACE("Allegro context destroyed");
		}
		else {
			LOG_CORE_WARN("Can't destroy Allegro context: Was not initialized!");
		}
	}

}
