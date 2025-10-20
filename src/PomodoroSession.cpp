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
}

const PomodoroSession::Round& PomodoroSession::GetActualRound() {
	UpdateRound();
	return _roundQueue.front();
}

void PomodoroSession::StartActualRound() {
	//TODO
	throw NotImplementedException();
}

void zenith::PomodoroSession::SetNextRound() {
	//TODO
	throw NotImplementedException();
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
