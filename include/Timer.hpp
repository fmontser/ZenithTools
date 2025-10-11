#pragma once

#include <chrono>
#include <string>

using std::string;

namespace zenith {

	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
	using Seconds = std::chrono::seconds;
	using Minutes = std::chrono::minutes;

	class Timer {
		enum class Status {
			Stopped, Paused, Running
		};

		Status _status;
		Seconds _durationTime;
		Seconds _remainingTime;
		TimePoint _startTime;
		TimePoint _targetTime;

		public:
			Timer(Minutes minutes, Seconds seconds);
			
			void Start();
			void Pause();
			void Resume();

			const string GetRemainingTime() const;
			const string GetElapsedTime() const;

	};

}
