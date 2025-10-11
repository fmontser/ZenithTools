#pragma once

#include <thread>
#include <chrono>
#include <string>
#include <mutex>

using std::string;

namespace zenith {

	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
	using Seconds = std::chrono::seconds;
	using Minutes = std::chrono::minutes;


	class Timer {
		public:
			enum class State {
				Stopped, Paused, Running, Ended
			};

			struct Status {
				State state;
				string remaining;
				string elapsed;
			};

			Timer(Minutes minutes, Seconds seconds);
			~Timer();

			void Start();
			void Pause();
			void Resume();
			void Reset();

			const Status GetStatus();

		private:
			Status _status;
			std::thread _thread;
			std::mutex _statusMutex;
			Seconds _durationTime;
			Seconds _remainingTime;
			TimePoint _startTime;
			TimePoint _targetTime;

			const Seconds FetchRemainingTime_locked() const;
			const Seconds FetchRemainingTime();
			const string FormatTimer(const Seconds& seconds) const;
			void Daemon();
	};

}
