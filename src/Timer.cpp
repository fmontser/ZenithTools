#include "Timer.hpp"
#include "Exceptions.hpp"
#include <sstream>
#include <iomanip>

using namespace zenith;
using std::string;
using std::stringstream;

Timer::Timer(Minutes minutes,Seconds seconds) {
	_status = Status::Stopped;
	_remainingTime = minutes + seconds;
	_durationTime = _remainingTime;
}

void Timer::Start() {
	if (_status == Status::Stopped) {
		_startTime = Clock::now();
		_targetTime = _startTime + _remainingTime;
		_status = Status::Running;
	}
}

void Timer::Pause() {
	if (_status == Status::Running) {
		_remainingTime = FetchRemainingTime();
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
	auto remainingTime = FetchRemainingTime();
	return FormatTimer(remainingTime);
}

const string Timer::GetElapsedTime() const {
	auto remainingTime = FetchRemainingTime();
	auto elapsedTime = _durationTime - remainingTime;
	return FormatTimer(elapsedTime);
}

const Seconds Timer::FetchRemainingTime() const {
	switch (_status)
	{
		case Status::Stopped:
			return _durationTime;
		case Status::Paused:
			return _remainingTime;
		case Status::Running:
			return std::chrono::duration_cast<Seconds>(
				_targetTime - Clock::now());
		default:
			throw InvalidStateException();
	}
}

const string Timer::FormatTimer(const Seconds& seconds) const {
	auto minutes = std::chrono::duration_cast<Minutes>(seconds);

	stringstream timeSS;
	timeSS	<< std::setw(2) << std::setfill('0') << minutes.count()
			<< ":"
			<< std::setw(2) << std::setfill('0')
			<< (seconds - minutes).count();

	return timeSS.str();
}