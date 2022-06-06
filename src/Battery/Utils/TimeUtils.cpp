
#include "Battery/Utils/TimeUtils.h"

namespace Battery {

	double runtime_start = 0.0;

	std::time_t ConvertTimestamp(const std::string& timestamp) {
		std::istringstream ss(timestamp);

		std::tm t{};
		ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");

		if (ss.fail()) {
			return 0;
		}

		return mktime(&t);
	}

	double GetRuntime() {
		
		if (runtime_start == 0.0)
			return 0.0;
		
		using namespace std::chrono;
		double timestamp = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() / 1000000.0;
		return timestamp - runtime_start;
	}

	void ClearRuntime() {
		runtime_start = GetRuntime();
	}

	void Sleep(double seconds) {
		std::this_thread::sleep_for(std::chrono::microseconds((size_t)(seconds * 1000000.0)));
	}
}
