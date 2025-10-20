#include "PomodoroSession.hpp"
#include "Exceptions.hpp"

using namespace zenith;

PomodoroSession::PomodoroSession(uint32_t rounds, uint32_t workMinutes,
	uint32_t restMinutes, uint32_t largeRestMinutes) {

		for (int i = 0; i < rounds; ++i) {
			auto restTime = restMinutes;
			if (i == (rounds / 2))
				restTime = largeRestMinutes;

			_roundQueue.push(Round(
				Timer(workMinutes, 0),
				Timer(restTime, 0)
			));
		}

		_status.actualRound = &_roundQueue.front();
}

const PomodoroSession::Status& PomodoroSession::GetStatus() {
	UpdateRound();
	return _status;
}

void PomodoroSession::StartActualRound() {
	Round& round = _roundQueue.front();
	if (_status.state == State::IDLE)
		_status.state = State::ONGOING;

	if (round.state == RoundState::IDLE) {

		//TODO @@@@@@@ thread problem!!!

		round.workTimer.Start();
		round.state = RoundState::WORKING;
	}
}

void zenith::PomodoroSession::SetNextRound() {
	Round& round = _roundQueue.front();
	if (round.state == RoundState::COMPLETED) {
		_roundQueue.pop();
		if (_roundQueue.empty())
			_status.state = State::COMPLETED;
	}
}

void PomodoroSession::UpdateRound() {
	Round& round = _roundQueue.front();

	switch (round.state)
	{
		case RoundState::IDLE:
			break;
		case RoundState::WORKING:
			round.progress = round.workTimer.GetStatus().progress;
			break;
		case RoundState::RESTING:
			round.progress = round.restTimer.GetStatus().progress;
			break;
		case RoundState::COMPLETED:
			break;	
		default:
			throw InvalidStateException();
			return;
	}
}
