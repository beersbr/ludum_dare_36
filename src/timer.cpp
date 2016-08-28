#include "timer.hpp"

Timer::Timer() {
	timer_start = GetTicks();
	Reset();
}


Timer::~Timer() {

}


long Timer::GetTimer() const {
	return GetTicks();
}


long Timer::GetStartTimeMilliseconds() const {
	return (GetTicks() - timer_start);
}


void Timer::Sleep(const long milliseconds) const {
	long start = GetTicks();
	while(start + milliseconds > GetTicks());
}


void Timer::Reset() {
	stopwatch_start = GetTicks();
}


bool Timer::Stopwatch(const long milliseconds) {
	if(GetTicks() > stopwatch_start + milliseconds) {
		stopwatch_start = GetTicks();
		return true;
	} else { 
		return false;
	}
}


long Timer::GetTicks() const {
	#ifdef __APPLE__
	static mach_timebase_info_data_t sTimebaseInfo;
	static uint64_t ratio;
	static const uint64_t NANOS_PER_USEC = 1000ULL;
	static const uint64_t NANOS_PER_MILLISEC = 1000ULL * NANOS_PER_USEC;
	static const uint64_t NANOS_PER_SEC = 1000ULL * NANOS_PER_MILLISEC;

	if(sTimebaseInfo.denom == 0) {
        mach_timebase_info(&sTimebaseInfo);
        ratio = sTimebaseInfo.numer / sTimebaseInfo.denom;
    }

    long result = ((mach_absolute_time() * ratio) / NANOS_PER_MILLISEC);
	return result;


	#elif _WIN32

	return SDL_GetTicks();

	#endif	
}