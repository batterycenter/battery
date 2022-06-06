
#include "Battery/Utils/TimeUtils.h"

namespace Battery {

	sf::Time runtime_start = sf::microseconds(0);

	std::time_t ConvertTimestamp(const std::string& timestamp) {
		std::istringstream ss(timestamp);

		std::tm t{};
		ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");

		if (ss.fail()) {
			return 0;
		}

		return mktime(&t);
	}

	sf::Time GetRuntime() {
		
		if (runtime_start.asMicroseconds() == 0)	// Not initialized yet
			return sf::microseconds(0);

		using namespace std::chrono;
		uint64_t timestamp = duration_cast<std::chrono::microseconds>(system_clock::now().time_since_epoch()).count();
		return sf::microseconds(timestamp - runtime_start.asMicroseconds());
	}

	void ClearRuntime() {
		runtime_start = GetRuntime();
	}

	void Sleep(double seconds) {
		std::this_thread::sleep_for(std::chrono::microseconds((size_t)(seconds * 1000000.0)));
	}

	void Sleep(sf::Time time) {
		std::this_thread::sleep_for(std::chrono::microseconds((size_t)(time.asMicroseconds())));
	}
}
