/**
 * @file PomodoroSession.hpp
 * @author https://github.com/fmontser
 * @brief PomodoroSession class definition.
 */

#pragma once

#include "Timer.hpp"
#include <queue>

namespace zenith {

	/**
	 * @brief Creates and manages a complete pomodoro session, including rounds and work/rest periods
	 */
	class PomodoroSession {
		public:
		
			/**
			 * @brief Represents the mode of a round, the actual state.
			 */
			enum class RoundMode {
				IDLE, WORKING, RESTING, COMPLETED
			};
			
			/**
			 * @brief Contains any necesary data of a single round.
			 */
			struct Round {
				Timer workTimer;
				Timer restTimer;
				float progress = 0.0f;
				RoundMode mode = RoundMode::IDLE;
				Round(Timer workTimer, Timer restTimer)
				: workTimer(workTimer), restTimer(restTimer) {}
			};
			
			/**
			 * @brief Represents the mode of the session itself, the actual state.
			 */
			enum class Mode {
				IDLE, ONGOING, COMPLETED
			};

			/**
			 * @brief Contains data concerning de status of the session.
			 */
			struct Status {
				Mode mode = Mode::IDLE;
				RoundMode roundMode = RoundMode::IDLE;
				string remainingTime = "00:00";
				string elapsedTime = "00:00";
				float progress = 0.0f;
				uint roundsLeft = 8;
			};
			
			/**
			 * @brief Constructs a new seesion with a number of rounds and periods values.
			 * @param rounds The number of rounds for a session.
			 * @param workTime The number in seconds for work periods.
			 * @param restTime The number in seconds for rest periods.
			 * @param largeRestTime The number in seconds for large rest period wich occur at half the session.
			 */
			PomodoroSession(uint rounds, Seconds workTime,
				Seconds restTime, Seconds largeRestTime);
			
			/**
			 * @brief Obtains the status data of.the session.
			 * @note This triggers and propagates updates on the data model.
			 * @details Call this function from the ui once per frame.
			 */
			const Status& GetStatus();

			/**
			 * @brief Starts and IDLE session/round.
			 * @details Will start a work period timer.
			 */
			void StartActualRound();

			/**
			 * @brief Continues a session/round after a work period.
			 * @details Will start a rest period timer.
			 */
			void StartRestingPeriod();

		private:
			Status _status;                 ///< Holds the status data for the session.
			std::queue<Round> _roundQueue;  ///< A queue containing every round for a given session.
			
			/**
			 * @brief Ends actual round and replaces it for the next if present.
			 * @details Will end session if no more rounds are left.
			 */
			void SetNextRound();

			/**
			 * @brief Updates the round mode and data on behalf of the period timers.
			 */
			void Update();

			/**
			 * @brief Determines if the round's index refers to the half of the session.
			 * @note Used to assign large rest period on the zenith of the session.
			 */
			bool IsHalfSessionRound(uint rounds, uint index);

	};

	/// @brief The mode of a session, idle, ongoing or completed
	using Mode = PomodoroSession::Mode;
	/// @brief The mode of a round, idle, working, resting or completed
	using RoundMode = PomodoroSession::RoundMode;
	/// @brief A structure of data that contains information about the session
	using Status = PomodoroSession::Status;
}