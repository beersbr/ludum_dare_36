#ifndef _LD_TIMER_
#define _LD_TIMER_

#include <SDL2/SDL.h>

// NOTE(Brett):This is from advanced 2d gamedev THE BOOK!

class Timer {
private:
	long timer_start;
	long stopwatch_start;

public:
	Timer();
	~Timer();
	long GetTimer() const;
	long GetStartTimeMilliseconds() const;
	void Sleep(const long milliseconds) const;
	void Reset();
	bool Stopwatch(const long milliseconds);
};


#endif