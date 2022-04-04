#pragma once

#include "Battery/Log/Log.h"
#include "Battery/Core/Exception.h"
#include "Battery/Platform/Dialog.h"
#include "Battery/Core/Allegro.h"

namespace Battery {

	// Singleton pattern, so only one instance can ever exist
	class AllegroContext {

		// Private constructors
		AllegroContext();
		~AllegroContext();

	public:

		bool Initialize(const std::string& applicationName);
		bool IsInitialized();
		void Destroy();

		// Functions for the singleton pattern
		AllegroContext(AllegroContext const&) = delete;
		void operator=(AllegroContext const&) = delete;

		static AllegroContext* GetInstance() {
			static AllegroContext instance;
			return &instance;
		}

	private:
		//bool initialized = false;
	};
}
