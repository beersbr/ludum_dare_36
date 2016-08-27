#include "timer.hpp"

Timer::Timer() {
	timer_start = SDL_GetTicks();
	Reset();
}


Timer::~Timer() {

}


long Timer::GetTimer() const {
	return SDL_GetTicks();
}


long Timer::GetStartTimeMilliseconds() const {
	return (SDL_GetTicks() - timer_start);
}


void Timer::Sleep(const long milliseconds) const {
	long start = SDL_GetTicks();
	while(start + milliseconds > SDL_GetTicks());
}


void Timer::Reset() {
	stopwatch_start = SDL_GetTicks();
}


bool Timer::Stopwatch(const long milliseconds) {
	if(SDL_GetTicks() > stopwatch_start + milliseconds) {
		stopwatch_start = SDL_GetTicks();
		return true;
	} else { 
		return false;
	}
}