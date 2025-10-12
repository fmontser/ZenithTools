#include "gtest/gtest.h"
#include "Timer.hpp"
#include <thread>
#include <chrono>

namespace zenith {

	// Verify timer starts with stopped state
	TEST(TimerTest, IsInitiallyStopped) {
		Timer timer(Minutes(1), Seconds(0));
		EXPECT_EQ(timer.GetStatus().state, Timer::State::Stopped);
	}

	// Verify initial remaining time
	TEST(TimerTest, InitialRemainingTimeIsCorrect) {
		Timer timer(Minutes(5), Seconds(30));
		EXPECT_EQ(timer.GetStatus().remaining, "05:30");
	}

	// Verify initial elapsed time
	TEST(TimerTest, InitialElapsedTimeIsZero) {
		Timer timer(Minutes(5), Seconds(30));
		EXPECT_EQ(timer.GetStatus().elapsed, "00:00");
	}

	// Verify Start() changes state to Running
	TEST(TimerTest, StartChangesStateToRunning) {
		Timer timer(Minutes(1), Seconds(0));
		timer.Start();
		EXPECT_EQ(timer.GetStatus().state, Timer::State::Running);
	}

	// Verify time progresses after starting
	TEST(TimerTest, TimeDecreasesAfterStart) {
		Timer timer(Minutes(0), Seconds(2));
		timer.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(1100));
		auto status = timer.GetStatus();
		EXPECT_EQ(status.remaining, "00:01");
		EXPECT_EQ(status.elapsed, "00:01");
	}

}
