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
	_status.state = State::Stopped;
	_status.remaining = FormatTimer(_remainingTime);
	_status.elapsed = FormatTimer(Seconds::zero());
}

Timer::Timer(unsigned int minutes,unsigned int seconds) {
	_remainingTime = Minutes(minutes) + Seconds(seconds);
	_durationTime = _remainingTime;
	_status.state = State::Stopped;
	_status.remaining = FormatTimer(_remainingTime);
	_status.elapsed = FormatTimer(Seconds::zero());
}

void Timer::Start() {
	if (_status.state == State::Ended) {
		_remainingTime = _durationTime;
		_status.state = State::Stopped;
	}

	if (_status.state == State::Stopped) {
		_startTime = Clock::now();
		_targetTime = _startTime + _remainingTime;
		_status.state = State::Running;
	}
}

void Timer::Pause() {
	if (_status.state == State::Running) {
		_remainingTime = FetchRemainingTime();
		_status.state = State::Paused;
	}
}

void Timer::Resume() {
	if (_status.state == State::Paused) {
		_targetTime = Clock::now() + _remainingTime;
		_status.state = State::Running;
	}
}

void Timer::Reset() {
	if (_status.state != State::Stopped) {
		_remainingTime = _durationTime;
		_status.state = State::Stopped;
	}
}

const Timer::Status Timer::GetStatus(){
	auto remainingTime = FetchRemainingTime();
	if (remainingTime <= Seconds::zero()) {
		remainingTime = Seconds::zero();
		_status.state = State::Ended;
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
	switch (_status.state)
	{
		case State::Stopped:
			return _durationTime;
		case State::Paused:
			return _remainingTime;
		case State::Running:
			return std::chrono::round<Seconds>(
			_targetTime - Clock::now());
		case State::Ended:
			return Seconds::zero();
		default:
			throw InvalidStateException();
	}
}

float Timer::CalculateProgress(const Seconds& elapsedTime) {
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
