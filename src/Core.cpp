
#include "Battery/pch.h"
#include "Battery/Core.h"
#include "Battery/Core/Exception.h"
#include "Battery/Utils/FileUtils.h"
#include "Battery/AllegroDeps.h"

namespace Battery {
	namespace Core {





		std::string GetApplicationName() {
			return al_get_app_name();
		}

		std::string GetOrganizationName() {
			return al_get_org_name();
		}

		void SetApplicationName(const std::string& name) {
			al_set_app_name(name.c_str());
		}

		void SetOrganizationName(const std::string& name) {
			al_set_org_name(name.c_str());
		}

		std::string GetAllegroVersion() {
			uint32_t version = al_get_allegro_version();
			int major = version >> 24;
			int minor = (version >> 16) & 255;
			int revision = (version >> 8) & 255;
			int release = version & 255;

			return std::to_string(major) + "." + std::to_string(minor) + "." + 
				   std::to_string(revision) + "." + std::to_string(release);
		}








		

	}
}
