#include "Timer.hpp"

using namespace zenith;

Timer::Timer(Minutes minutes, Seconds seconds) {
	_status = STOPPED;
	_remainingTime = minutes + seconds;
}

void Timer::Start() {
	if (_status == STOPPED) {
		_targetTime = Clock::now() + _remainingTime;
		_status = RUNNING;
	}
}

void Timer::Pause() {
	if (_status == RUNNING) {
		_remainingTime = std::chrono::duration_cast<Seconds>(
			_targetTime - Clock::now()
		);
		_status = PAUSED;
	}
}

void Timer::Resume() {
	if (_status == PAUSED) {
		_targetTime = Clock::now() + _remainingTime;
		_status = RUNNING;
	}
}