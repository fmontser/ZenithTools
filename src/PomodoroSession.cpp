#include "PomodoroSession.hpp"
#include "Exceptions.hpp"

using namespace zenith;

PomodoroSession::PomodoroSession(std::unique_ptr<ISoundGenerator> sg, uint32_t rounds,
	Seconds workTime, Seconds restTime, Seconds largeRestTime) {

		for (uint32_t i = 0; i < rounds; ++i) {
			auto restSeconds = restTime;
			if (IsHalfSessionRound(rounds, i))
				restSeconds = largeRestTime;

			_roundQueue.push(Round(
				Timer(Minutes::zero(), workTime),
				Timer(Minutes::zero(), restSeconds)
			));
		}
		_status.roundsTotal = rounds;
		_status.roundsLeft = rounds;
		_sg = std::move(sg);


		//TODO protect every _roundQueue acces if empty
		//TODO refactor modes/roundmodes into single sequence
		//TODO refactor unification on status updates.
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
	Timer& timer = GetActualTimer();
	_status.mode = Mode::PAUSED;
	timer.Pause();
}

void PomodoroSession::ResumePeriod() {
	Timer& timer = GetActualTimer();
	_status.mode = Mode::ONGOING;
	timer.Resume();
}

void PomodoroSession::ResetPeriod() {
	Round& round = _roundQueue.front();
	Timer& timer = GetActualTimer();

	if (_status.roundMode == RoundMode::WORKING) {
		round.mode = RoundMode::IDLE;
		_status.roundMode = RoundMode::IDLE;
		_status.mode = Mode::IDLE;
	} else if (_status.roundMode == RoundMode::RESTING) {
		_status.mode = Mode::IDLE;
	}
	timer.Reset();
	_status.remainingTime = timer.GetStatus().remaining;
	_status.elapsedTime = timer.GetStatus().elapsed;
	round.progress = timer.GetStatus().progress;
}

void PomodoroSession::SkipPeriod() {
	//TODO not implemented
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
		else {
			_status.mode = Mode::IDLE;
			_status.elapsedTime = "00:00";
		}
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
				_sg->PlayRestBell();
			}
			break;
		case RoundMode::RESTING:
			round.progress = round.restTimer.GetStatus().progress;
			_status.remainingTime = round.restTimer.GetStatus().remaining;
			_status.elapsedTime = round.restTimer.GetStatus().elapsed;
			if (round.restTimer.GetStatus().mode == Timer::Mode::Ended) {
				_status.mode = Mode::IDLE;
				round.mode = RoundMode::COMPLETED;
				_sg->PlayWorkBell();
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

bool PomodoroSession::IsHalfSessionRound(uint32_t rounds, uint32_t index) {
	const uint32_t IDX_OFFSET = 1;
	return ((index + IDX_OFFSET) == (rounds / 2) && rounds > 1);
}

Timer& PomodoroSession::GetActualTimer() {
	Update();
	Round& round = _roundQueue.front();
	if (_status.roundMode == RoundMode::WORKING || _status.roundMode == RoundMode::IDLE)
		return round.workTimer;
	else
		return round.restTimer;
}
