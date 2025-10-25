#include "Timer.hpp"
#include "Exceptions.hpp"
#include <sstream>
#include <iomanip>
#include <thread>

using namespace zenith;
using std::string;
using std::stringstream;

Timer::Timer(Minutes minutes,Seconds seconds) {
	_remainingTime = minutes + seconds;
	_durationTime = _remainingTime;
	_status.mode = Mode::Stopped;
	_status.remaining = FormatTimer(_remainingTime);
	_status.elapsed = FormatTimer(Seconds::zero());
}

void Timer::Start() {
	if (_status.mode == Mode::Ended) {
		_remainingTime = _durationTime;
		_status.mode = Mode::Stopped;
	}

	if (_status.mode == Mode::Stopped) {
		_startTime = Clock::now();
		_targetTime = _startTime + _remainingTime;
		_status.mode = Mode::Running;
	}
}

void Timer::Pause() {
	if (_status.mode == Mode::Running) {
		_remainingTime = FetchRemainingTime();
		_status.mode = Mode::Paused;
	}
}

void Timer::Resume() {
	if (_status.mode == Mode::Paused) {
		_targetTime = Clock::now() + _remainingTime;
		_status.mode = Mode::Running;
	}
}

void Timer::Reset() {
	if (_status.mode != Mode::Stopped) {
		_remainingTime = _durationTime;
		_startTime = Clock::now();
		_targetTime = _startTime + _remainingTime;
		_status.progress = 0.0f;
		_status.mode = Mode::Stopped;
	}
}

const Timer::Status Timer::GetStatus() {
	auto remainingTime = FetchRemainingTime();
	if (remainingTime <= Seconds::zero()) {
		remainingTime = Seconds::zero();
		_status.mode = Mode::Ended;
	}

	auto elapsedTime = _durationTime - remainingTime;
	if (elapsedTime > _durationTime)
		elapsedTime = _durationTime;

	_status.remaining = FormatTimer(remainingTime);
	_status.elapsed = FormatTimer(elapsedTime);
	_status.progress = CalculateProgress(elapsedTime);
	return _status;
}

const Seconds Timer::FetchRemainingTime() const {
	switch (_status.mode)
	{
		case Mode::Stopped:
			return _durationTime;
		case Mode::Paused:
			return _remainingTime;
		case Mode::Running:
			return std::chrono::round<Seconds>(
			_targetTime - Clock::now());
		case Mode::Ended:
			return Seconds::zero();
		default:
			throw InvalidModeException();
	}
}

float Timer::CalculateProgress(const Seconds& elapsedTime) const {
	return static_cast<float>(elapsedTime.count()) / _durationTime.count();
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
