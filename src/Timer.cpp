#include "Timer.hpp"

using namespace zenith;

Timer::Timer(Minutes minutes, Seconds seconds) {
	_status = Status::Stopped;
	_remainingTime = minutes + seconds;
}

void Timer::Start() {
	if (_status == Status::Stopped) {
		_targetTime = Clock::now() + _remainingTime;
		_status = Status::Running;
	}
}

void Timer::Pause() {
	if (_status == Status::Running) {
		_remainingTime = std::chrono::duration_cast<Seconds>(
			_targetTime - Clock::now()
		);
		_status = Status::Paused;
	}
}

void Timer::Resume() {
	if (_status == Status::Paused) {
		_targetTime = Clock::now() + _remainingTime;
		_status = Status::Running;
	}
}