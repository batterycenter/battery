
#include "Battery/pch.h"
#include "Battery/Core/Application.h"
#include "Battery/Platform/Dialog.h"
#include "Battery/Log/Log.h"

namespace Battery {
	extern Battery::Application* applicationPointer;
}

int main(int argc, const char** argv) {

	try {	// Logging is initialized with the first LOG call

		// Create the client application
		LOG_CORE_TRACE("Creating Application");
		std::unique_ptr<Battery::Application> app(Battery::CreateApplication());

		// Save application pointer, so it can be accessed globally
		Battery::applicationPointer = app.get();	// This is done so that a shared instance wouldn't be given away: The engine must stay in control!

		// Start the engine up
		LOG_CORE_TRACE("Application created, running");
		app->Run(argc, argv);

		// Destroy the application
		LOG_CORE_TRACE("Application stopped, destroying");
	}
	catch (const Battery::Exception& e) {
		LOG_CORE_CRITICAL(std::string("Unhandled Battery::Exception: '") + e.what() + "'");
		Battery::ShowErrorMessageBox(std::string("Unhandled Battery::Exception: '") + e.what() + "'");
	}
	catch (const std::exception& e) {
		LOG_CORE_CRITICAL(std::string("Unhandled std::exception: '") + e.what() + "'");
		Battery::ShowErrorMessageBox(std::string("Unhandled std::exception: '") + e.what() + "'");
	}
	catch (...) {
		LOG_CORE_CRITICAL("Unhandled exception: Unknown exception type, make sure to catch it correctly!");
		Battery::ShowErrorMessageBox("Unhandled exception: Unknown exception type, make sure to catch it correctly!");
	}

	LOG_CORE_TRACE("Application destroyed, main() returned");
}
