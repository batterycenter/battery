
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

	static sf::Time getEpochTime() {
		using namespace std::chrono;
		return sf::microseconds(duration_cast<std::chrono::microseconds>(system_clock::now().time_since_epoch()).count());
	}

	sf::Time GetRuntime() {
		
		if (runtime_start.asMicroseconds() == 0)	// Not initialized yet
			return sf::microseconds(0);

		return getEpochTime() - runtime_start;
	}

	void ClearRuntime() {
		
		runtime_start = getEpochTime();
	}

	void Sleep(double seconds) {
		std::this_thread::sleep_for(std::chrono::microseconds((size_t)(seconds * 1000000.0)));
	}

	void Sleep(sf::Time time) {
		std::this_thread::sleep_for(std::chrono::microseconds((size_t)(time.asMicroseconds())));
	}
}
