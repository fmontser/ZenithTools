#pragma once

#include <chrono>

namespace zenith {

	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
	using Seconds = std::chrono::seconds;
	using Minutes = std::chrono::minutes;
	using Duration = std::chrono::duration<int>;

	class Timer {
		enum Status {
			STOPPED, PAUSED, RUNNING
		};

		Status _status;
		Duration _remainingTime;
		TimePoint _targetTime;

		public:
			Timer(Minutes minutes, Seconds seconds);
			
			void Start();
			void Pause();
			void Resume();
	};

}