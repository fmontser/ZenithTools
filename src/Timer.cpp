#include "Timer.hpp"
#include "Exceptions.hpp"
#include <sstream>
#include <iomanip>

using namespace zenith;
using std::string;
using std::stringstream;

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

const string Timer::GetRemainingTime() const {
	auto remainingTime = _targetTime - Clock::now();
	auto remainingMinutes = std::chrono::duration_cast<Minutes>(remainingTime);
	auto remainingSeconds = std::chrono::duration_cast<Seconds>(remainingTime);

	stringstream timeSS;
	timeSS	<< std::setw(2) << std::setfill('0') << remainingMinutes.count()
			<< ":"
			<< std::setw(2) << std::setfill('0')
			<< (remainingSeconds - remainingMinutes).count();

	return timeSS.str();
}

#include <exception>

string const Timer::GetElapsedTime() const {
	throw NotImplementedException();
}
