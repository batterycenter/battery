#pragma once

#include "Battery/pch.h"
#include "Battery/Core/Config.h"
#include "Battery/Log/Log.h"

namespace Battery {

	/// <summary>
	/// Convert a Unix timestamp like '2021-01-02T10:34:52Z' to an epoch number, which is 1609580092 in this case.
	/// Returns 0 on failure
	/// </summary>
	/// <param name="timestamp">- Unix timestamp in string format</param>
	/// <returns>std::time_t - The epoch time in seconds or 0 on failure</returns>
	std::time_t ConvertTimetamp(const std::string& timestamp);

	/// <summary>
	/// Get the time in seconds since Allegro has been initalized, 
	/// Precision goes down to the microseconds range. If not initialized, returns 0.
	/// </summary>
	/// <returns>double - Time since engine is running in seconds</returns>
	double GetRuntime();

	/// <summary>
	/// Put the current thread to sleep for x seconds.
	/// </summary>
	/// <param name="microseconds">- Time to sleep in seconds</param>
	void Sleep(double seconds);









	// TODO: Fix profiling
	// TODO: Clean this shit


	// A container for the results from profiling
	struct ProfileResults {
		char names[BATTERY_PROFILING_MAX_SCOPED_NUMBER][BATTERY_PROFILING_SCOPED_STRING_LENGTH];
		double times[BATTERY_PROFILING_MAX_SCOPED_NUMBER];
		size_t nextIndex = 0;

		ProfileResults() {
			Clear();
		}

		void Clear() {
			for (size_t i = 0; i < BATTERY_PROFILING_MAX_SCOPED_NUMBER; i++) {
				memset(names[i], 0, BATTERY_PROFILING_SCOPED_STRING_LENGTH);
			}
			memset(times, 0, BATTERY_PROFILING_MAX_SCOPED_NUMBER);
			nextIndex = 0;
		}

		void AddResult(const char* name, double value) {

			if (nextIndex >= BATTERY_PROFILING_MAX_SCOPED_NUMBER) {
				LOG_CORE_ERROR(__FUNCTION__"(): Can't add another profiler result: The result buffer is full! "
					"Up to " + std::to_string(BATTERY_PROFILING_MAX_SCOPED_NUMBER) + " results are supported!");
				return;
			}

			times[nextIndex] = value;
			strncpy_s(names[nextIndex], name, BATTERY_PROFILING_SCOPED_STRING_LENGTH);
			nextIndex++;
		}
	};

	// A class to profile with timestamps
	class TimestampProfiler {
	public:

		size_t nextIndex = 0;
		double timestamps[BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER];
		char names[BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER][BATTERY_PROFILING_TIMEPOINT_STRING_LENGTH];

		TimestampProfiler() {
			memset(timestamps, 0, BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER);
			for (size_t i = 0; i < BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER; i++) {
				memset(names[i], 0, BATTERY_PROFILING_TIMEPOINT_STRING_LENGTH);
			}
		}

		void StartProfiling(const std::string& name) {
			for (size_t i = 0; i < BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER; i++) {
				timestamps[i] = 0.0;
				memset(names[i], 0, BATTERY_PROFILING_TIMEPOINT_STRING_LENGTH);
			}
			nextIndex = 0;
			AddTimestamp(name);
		}

		void StartProfiling(const char* name) {
			for (size_t i = 0; i < BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER; i++) {
				timestamps[i] = 0.0;
				memset(names[i], 0, BATTERY_PROFILING_TIMEPOINT_STRING_LENGTH);
			}
			nextIndex = 0;
			AddTimestamp(name);
		}

		void AddTimestamp(const std::string& name) {
			AddTimestamp(name.c_str());
		}

		void AddTimestamp(const char* name) {

			if (nextIndex >= BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER) {
				LOG_CORE_ERROR(__FUNCTION__"(): Can't add another timepoint: The profiler timepoint buffer is full! "
					"Up to " + std::to_string(BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER) + " timepoints are supported!");
				return;
			}

			timestamps[nextIndex] = GetRuntime();
			strncpy_s(names[nextIndex], name, BATTERY_PROFILING_TIMEPOINT_STRING_LENGTH);
			nextIndex++;
		}
	};

	// A singleton storage class, which stores profiling results globally until they're displayed
	class ProfilerStorage {
	public:
		static ProfilerStorage& GetInstance() {
			static ProfilerStorage instance;
			return instance;
		}

		void ApplyProfiles(double frametime) {

			engineProfilingResults.Clear();
			clientProfilingResults.Clear();
			profilerResultsRenderTime.Clear();

			engineProfilingResults = temp_engineProfilingResults;
			clientProfilingResults = temp_clientProfilingResults;
			profilerResultsRenderTime = temp_profilerResultsRenderTime;

			temp_engineProfilingResults.Clear();
			temp_clientProfilingResults.Clear();
			temp_profilerResultsRenderTime.Clear();

			timestampProfilerComplete = timestampProfiler;

			double recordedFrametime = 0;
			size_t size = timestampProfilerComplete.nextIndex;
			if (size > 0) {
				recordedFrametime = timestampProfilerComplete.timestamps[size - 1] - timestampProfilerComplete.timestamps[0];
			}
			frametimeMissed = frametime - recordedFrametime;
		}

		TimestampProfiler timestampProfiler;
		TimestampProfiler timestampProfilerComplete;
		double frametimeMissed = 0;

		ProfileResults engineProfilingResults;
		ProfileResults clientProfilingResults;
		ProfileResults profilerResultsRenderTime;
		double entireFrametime = 0;

		ProfileResults temp_engineProfilingResults;
		ProfileResults temp_clientProfilingResults;
		ProfileResults temp_profilerResultsRenderTime;

		ProfilerStorage(ProfilerStorage const&) = delete;
		void operator=(ProfilerStorage const&) = delete;

	private:
		ProfilerStorage() {}
	};

	// A macro for scoped profiling
#ifdef BATTERY_PROFILING
#define TIMEUTILS_TOKENPASTE(x, y) x ## y
#define TIMEUTILS_TOKENPASTE2(x, y) TIMEUTILS_TOKENPASTE(x, y)

#define PROFILE_CORE_SCOPE(name) TimeUtils::ScopedTimer TIMEUTILS_TOKENPASTE2(timer, __LINE__) \
		(name, &TimeUtils::ProfilerStorage::GetInstance().temp_engineProfilingResults)
#define PROFILE_SCOPE(name) TimeUtils::ScopedTimer TIMEUTILS_TOKENPASTE2(timer, __LINE__) \
		(name, &TimeUtils::ProfilerStorage::GetInstance().temp_clientProfilingResults)

#define PROFILE_TIMESTAMP_START(name) TimeUtils::ProfilerStorage::GetInstance().timestampProfiler.StartProfiling(name)
#define PROFILE_TIMESTAMP(name) TimeUtils::ProfilerStorage::GetInstance().timestampProfiler.AddTimestamp(name)
#else
#define PROFILE_CORE_SCOPE(name)
#define PROFILE_SCOPE(name)
#define PROFILE_TIMESTAMP_START(name)
#define PROFILE_TIMESTAMP(name)
#endif

		// A class for measuring the time from contruction to destruction
	class ScopedTimer {

		char name[BATTERY_PROFILING_SCOPED_STRING_LENGTH];
		double startTime = 0.0;
		ProfileResults* bufferReference = nullptr;

	public:

		ScopedTimer(const std::string& name, ProfileResults* buffer) :
			bufferReference(buffer)
		{
			// Start the timer
			startTime = GetRuntime();
			strncpy_s(this->name, name.c_str(), BATTERY_PROFILING_SCOPED_STRING_LENGTH);
		}

		ScopedTimer(const char* name, ProfileResults* buffer) :
			bufferReference(buffer)
		{
			// Start the timer
			startTime = GetRuntime();
			strncpy_s(this->name, name, BATTERY_PROFILING_SCOPED_STRING_LENGTH);
		}

		~ScopedTimer() {
			// Stop the timer and push back
			double difference = GetRuntime() - startTime;
			bufferReference->AddResult(name, difference);
		}
	};

	// A class for measuring the time between calls to Update()
	class ConsistentTimer {

		double lastTime = 0.0;

	public:

		ConsistentTimer() {
			// Initialize the timer
			lastTime = GetRuntime();
		}

		double Update() {

			double now = GetRuntime();
			double difference = 0.0;
			if (lastTime != 0.0) {
				difference = now - lastTime;
			}
			lastTime = now;

			return difference;
		}
	};
}
