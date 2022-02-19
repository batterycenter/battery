#pragma once

namespace Battery {
	namespace Platform {

		/// <summary>
		/// Get the last error with GetLastError() from the Windows API. Converts the error code
		/// to string format. Empty if there's no error.
		/// </summary>
		std::string GetLastErrorAsString();

	}
}
