
#include "battery/time.hpp"

#include <chrono>
#include <thread>

#ifdef B_OS_WINDOWS
#include <Windows.h>
#include <timeapi.h>
#else
#include <errno.h>
#include <time.h>
#endif

namespace b {

double time()
{
    using namespace std::chrono;
    static auto program_start = high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<microseconds>(high_resolution_clock::now()
                                                             - program_start);
    return static_cast<double>(duration.count()) / 1000000.0;
}

std::time_t epoch_time()
{
    using namespace std::chrono;
    return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

std::time_t epoch_time_ms()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

std::time_t epoch_time_us()
{
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}

void sleep(double seconds)
{
    sleep_ms(std::max(seconds * 1000.0, 0.0));
}

void sleep_precise(double seconds)
{
    auto start = b::time();
    while (b::time() - start < seconds) {
        ; // Busy-wait
    }
}

void sleep_ms(double milliseconds)
{
#ifdef B_OS_WINDOWS
    TIMECAPS tc; // Choose the minimum timer resolution
    timeGetDevCaps(&tc, sizeof(TIMECAPS));
    timeBeginPeriod(tc.wPeriodMin);

    ::Sleep(static_cast<DWORD>(std::max(milliseconds, 0.0)));

    timeEndPeriod(tc.wPeriodMin); // And back to default
#else
    timespec ti;
    ti.tv_sec = static_cast<time_t>(std::max(milliseconds, 0.0) / 1000);
    ti.tv_nsec = static_cast<long>((milliseconds - (ti.tv_sec * 1000)) * 1000000);

    // If nanosleep returns -1, we check errno. If it is EINTR
    // nanosleep was interrupted and has set ti to the remaining
    // duration. We continue sleeping until the complete duration
    // has passed. We stop sleeping if it was due to an error.
    while ((nanosleep(&ti, &ti) == -1) && (errno == EINTR)) {
        std::this_thread::yield();
    }
#endif
}
}
