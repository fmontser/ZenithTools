/**
 * @file Timer.hpp
 * @author https://github.com/fmontser
 * @brief Timer class definition.
 */

#pragma once

#include <chrono>
#include <string>

using std::string;

namespace zenith {

	/**
	 * @brief Manages a timer with minutes and seconds.
	 * 
	 * Allows to start, pause, resume and stop a timer.
	 */
	class Timer {
		public:
			/**
			 * @brief Represents the possible modes of the Timer.
			 */
			enum class Mode {
				Stopped, ///< The timer is not started.
				Paused,  ///< The timer is paused.
				Running, ///< The timer is actively counting down.
				Ended    ///< The timer has finished its countdown.
			};

			/**
			 * @brief A snapshot of the timer's current status.
			 */
			struct Status {
				Mode mode;             ///< The current mode of the timer.
				string remaining;        ///< Formatted string of the remaining time (e.g., "24:59").
				string elapsed;          ///< Formatted string of the elapsed time.
				float progress = 0.0f;   ///< 0.0 to 1.0 progress elapsed
			};

			/**
			 * @brief Constructs a new Timer with a specific duration.
			 * @param minutes The minutes part of the timer's duration.
			 * @param seconds The seconds part of the timer's duration.
			 */
			Timer(Minutes minutes, Seconds seconds);

			/**
			 * @brief Starts the timer countdown.
			 * @note If the timer was already running, it will be reset and started again.
			 */
			void Start();

			/**
			 * @brief Pauses the timer's countdown.
			 * @note If the timer is not running, this method has no effect.
			 */
			void Pause();

			/**
			 * @brief Resumes the timer's countdown from where it was paused.
			 * @note If the timer was not paused, this method has no effect.
			 */
			void Resume();

			/**
			 * @brief Resets the timer to its initial duration and stops it.
			 */
			void Reset();

			/**
			 * @brief Retrieves a snapshot of the timer's current status.
			 * @return A const Status struct containing the mode, remaining time, elapsed time and progress.
			 * @note Status is only update when calling this function.
			 */
			const Status GetStatus();

		private:
			Status _status;           ///< Holds the current snapshot of the timer's status.
			Seconds _durationTime;    ///< The total duration the timer was set for.
			Seconds _remainingTime;   ///< The time remaining when paused.
			TimePoint _startTime;     ///< The time point when the timer was last (re)started.
			TimePoint _targetTime;    ///< The time point when the timer is scheduled to end.

			/**
			 * @brief Calculates the remaining time based on the current time (internal, non-locking).
			 * @return The remaining seconds.
			 */

			/**
			 * @brief Acquires a lock and calculates the remaining time.
			 * @return The remaining seconds.
			 */
			const Seconds FetchRemainingTime() const;

			/**
			 * @brief Formats a duration in seconds into a MM:SS string.
			 * @param seconds The duration to format.
			 * @return A string in "MM:SS" format.
			 */
			const string FormatTimer(const Seconds& seconds) const;

			/**
			 * @brief Returns progress with float
			 * @return A float 0.0g to 1.0f 
			 */
			float CalculateProgress(const Seconds& elapsedTime) const;
	};

	/// @brief A clock that cannot be set and represents monotonic time.
	using Clock = std::chrono::steady_clock;
	/// @brief A specific point in time, as measured by a Clock.
	using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
	/// @brief A duration measured in seconds.
	using Seconds = std::chrono::seconds;
	/// @brief A duration measured in minutes.
	using Minutes = std::chrono::minutes;

}