/**
 * @file PomodoroSession.hpp
 * @author https://github.com/fmontser
 * @brief PomodoroSession class definition.
 */

#pragma once

#include "Timer.hpp"
#include "SoundGenerator.hpp"
#include <queue>
#include <memory>
#include <cstdint>

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
				IDLE, ONGOING, PAUSED, COMPLETED
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
				uint32_t roundsLeft = 8;
				uint32_t roundsTotal = 8;
			};
			
			/**
			 * @brief Constructs a new session with a number of rounds and periods values.
			 * @note Time is managed with <chrono> so you can use any time type.
			 * @param rounds The number of rounds for a session.
			 * @param workTime The number in seconds for work periods.
			 * @param restTime The number in seconds for rest periods.
			 * @param largeRestTime The number in seconds for large rest period wich occur at half the session.
			 */
			PomodoroSession(
				std::unique_ptr<ISoundGenerator> sg,
				uint32_t rounds = 4,
				Seconds workTime = Minutes(25),
				Seconds restTime = Minutes(5),
				Seconds largeRestTime = Minutes(15)
			);
			
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

			/**
			 * @brief Pauses the actual period.
			 */
			void PausePeriod();

			/**
			 * @brief Resumes the actual paused period.
			 */
			void ResumePeriod();

			/**
			 * @brief Resets the actual period.
			 */
			void ResetPeriod();

			/**
			 * @brief Skips the actual period.
			 */
			void SkipPeriod();

		private:
			Status _status;                        ///< Holds the status data for the session.
			std::queue<Round> _roundQueue;         ///< A queue containing every round for a given session.
			std::unique_ptr<ISoundGenerator> _sg;  ///< Contains the sound generator instance for this session.

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
			bool IsHalfSessionRound(uint32_t rounds, uint32_t index);

			/**
			 * @brief Gets the actual work/rest timer.
			 * @note Class internal use only.
			 */
			Timer& GetActualTimer();

	};

	/// @brief The mode of a session, idle, ongoing, paused or completed
	using Mode = PomodoroSession::Mode;
	/// @brief The mode of a round, idle, working, resting or completed
	using RoundMode = PomodoroSession::RoundMode;
	/// @brief A structure of data that contains information about the session
	using Status = PomodoroSession::Status;
}