#include "PomodoroSession.hpp"
#include "Exceptions.hpp"

using namespace zenith;

PomodoroSession::PomodoroSession(uint rounds, Seconds workTime,
	Seconds restTime, Seconds largeRestTime) {

		for (int i = 0; i < rounds; ++i) {
			auto restSeconds = restTime;
			if (i == (rounds / 2) && rounds > 1)
				restSeconds = largeRestTime;

			_roundQueue.push(Round(
				Timer(Minutes::zero(), workTime),
				Timer(Minutes::zero(), restSeconds)
			));
		}
}

const PomodoroSession::Status& PomodoroSession::GetStatus() {
	if (!_roundQueue.empty())
		UpdateRound();
	return _status;
}

void PomodoroSession::StartActualRound() {
	Round& round = _roundQueue.front();
	if (_status.mode == Mode::IDLE)
		_status.mode = Mode::ONGOING;

	if (round.mode == RoundMode::IDLE) {
		round.mode = RoundMode::WORKING;
		round.workTimer.Start();
	}
}


void zenith::PomodoroSession::SetNextRound() {
	Round& round = _roundQueue.front();
	if (round.mode == RoundMode::COMPLETED) {
		_roundQueue.pop();
		if (_roundQueue.empty()) {
			_status.mode = Mode::COMPLETED;
		}
	}
}

void PomodoroSession::UpdateRound() {
	Round& round = _roundQueue.front();

	switch (round.mode)
	{
		case RoundMode::IDLE:
			break;
		case RoundMode::WORKING:
			round.progress = round.workTimer.GetStatus().progress;
			if (round.workTimer.GetStatus().mode == Timer::Mode::Ended) {
				round.mode = RoundMode::RESTING;
				round.restTimer.Start();
			}
			break;
		case RoundMode::RESTING:
			round.progress = round.restTimer.GetStatus().progress;
			if (round.restTimer.GetStatus().mode == Timer::Mode::Ended) {
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
}
