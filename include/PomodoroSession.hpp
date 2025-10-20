#pragma once

#include "Timer.hpp"
#include <vector>
#include <queue>

namespace zenith {

	class PomodoroSession {
		public:
			PomodoroSession(uint32_t rounds, uint32_t workMinutes,
				uint32_t restMinutes, uint32_t largeRestMinutes);

			enum class RoundState {
				IDLE, WORKING, RESTING, COMPLETED
			};

			struct Round {
				Timer& workTimer;
				Timer& restTimer;
				float progress = 0.0f;
				RoundState state = RoundState::IDLE;
				Round(Timer workTimer, Timer restTimer)
					: workTimer(workTimer), restTimer(restTimer) {}
			};

			const Round& GetActualRound();
			void StartActualRound();
			void SetNextRound();

		private:
			std::queue<Round> _roundQueue;

			void UpdateRound();

	};
}