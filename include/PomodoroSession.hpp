#pragma once

#include "Timer.hpp"
#include <queue>

namespace zenith {

	class PomodoroSession {
		public:
		
			PomodoroSession(uint rounds, Seconds workTime,
				Seconds restTime, Seconds largeRestTime);

			enum class RoundMode {
				IDLE, WORKING, RESTING, COMPLETED
			};

			enum class Mode {
				IDLE, ONGOING, COMPLETED
			};

			struct Round {
				Timer workTimer;
				Timer restTimer;
				float progress = 0.0f;
				RoundMode mode = RoundMode::IDLE;
				Round(Timer workTimer, Timer restTimer)
				: workTimer(workTimer), restTimer(restTimer) {}
			};
			
			struct Status {
				Mode mode = Mode::IDLE;
			};

			const Status& GetStatus();
			void StartActualRound();
			void SetNextRound();
			
			private:
			Status _status;
			std::queue<Round> _roundQueue;
			
			void UpdateRound();


	};
}