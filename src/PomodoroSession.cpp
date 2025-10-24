#include "PomodoroSession.hpp"
#include "Exceptions.hpp"
//TODO borrar
#include <iostream>

using namespace zenith;

PomodoroSession::PomodoroSession(uint rounds, Seconds workTime,
	Seconds restTime, Seconds largeRestTime) {

		for (uint i = 0; i < rounds; ++i) {
			auto restSeconds = restTime;
			if (IsHalfSessionRound(rounds, i))
				restSeconds = largeRestTime;

			_roundQueue.push(Round(
				Timer(Minutes::zero(), workTime),
				Timer(Minutes::zero(), restSeconds)
			));
		}
		_status.roundsLeft = rounds;
}

const PomodoroSession::Status& PomodoroSession::GetStatus() {
	Update();
	return _status;
}

void PomodoroSession::StartActualRound() {
	Update();
	Round& round = _roundQueue.front();
	if (_status.mode == Mode::IDLE)
		_status.mode = Mode::ONGOING;

	if (round.mode == RoundMode::IDLE) {
		round.mode = RoundMode::WORKING;
		round.workTimer.Start();
	}
}

void PomodoroSession::StartRestingPeriod() {
	Update();
	Round& round = _roundQueue.front();
	if (round.mode == RoundMode::RESTING) {
		_status.mode = Mode::ONGOING;
		round.restTimer.Start();
	}
}

void PomodoroSession::PausePeriod() {
	//TODO
	throw NotImplementedException();
}

void PomodoroSession::ResumePeriod() {
	//TODO
	throw NotImplementedException();
}

void PomodoroSession::ResetPeriod() {
	//TODO
	throw NotImplementedException();
}

void PomodoroSession::RestartSession() {
	//TODO
	throw NotImplementedException();
}

void zenith::PomodoroSession::SetNextRound() {
	Round& round = _roundQueue.front();
	if (round.mode == RoundMode::COMPLETED) {
		_roundQueue.pop();
		if (_status.roundsLeft > 0)
			_status.roundsLeft--;
		if (_roundQueue.empty())
			_status.mode = Mode::COMPLETED;
		else
			_status.mode = Mode::IDLE;
	}
}

void PomodoroSession::Update() {
	if (_roundQueue.empty())
		return;

	Round& round = _roundQueue.front();

	switch (round.mode)
	{
		case RoundMode::IDLE:
			break;
		case RoundMode::WORKING:
			round.progress = round.workTimer.GetStatus().progress;
			_status.remainingTime = round.workTimer.GetStatus().remaining;
			_status.elapsedTime = round.workTimer.GetStatus().elapsed;
			if (round.workTimer.GetStatus().mode == Timer::Mode::Ended) {
				_status.mode = Mode::IDLE;
				round.mode = RoundMode::RESTING;
			}
			break;
		case RoundMode::RESTING:
			round.progress = round.restTimer.GetStatus().progress;
			_status.remainingTime = round.restTimer.GetStatus().remaining;
			_status.elapsedTime = round.restTimer.GetStatus().elapsed;
			if (round.restTimer.GetStatus().mode == Timer::Mode::Ended) {
				_status.mode = Mode::IDLE;
				round.mode = RoundMode::COMPLETED;
				SetNextRound();
			}
			break;
		case RoundMode::COMPLETED:
			break;
		default:
			throw InvalidModeException();
			return;
	}

	_status.roundMode = round.mode;
	_status.progress = round.progress;
	_status.roundsLeft = _roundQueue.size();
}

bool PomodoroSession::IsHalfSessionRound(uint rounds, uint index) {
	const uint IDX_OFFSET = 1;
	return ((index + IDX_OFFSET) == (rounds / 2) && rounds > 1);
}
