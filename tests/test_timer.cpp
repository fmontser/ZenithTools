#include "gtest/gtest.h"
#include "Timer.hpp"

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
		Timer timer(Minutes(0), Seconds(10));
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

	// Verify time stops after pausing
	TEST(TimerTest, TimeStopAfterPause) {
		Timer timer(Minutes(0), Seconds(10));
		timer.Start();
		std::this_thread::sleep_for(std::chrono::seconds(2));
		timer.Pause();
		std::this_thread::sleep_for(std::chrono::seconds(2));
		auto status = timer.GetStatus();
		EXPECT_EQ(status.state, Timer::State::Paused);
		EXPECT_EQ(status.remaining, "00:08");
		EXPECT_EQ(status.elapsed, "00:02");
	}

	// Verify timer pauses only in running state
	TEST(TimerTest, TimerPausesOnlyRunning) {
		Timer timer(Minutes(0), Seconds(2));
		timer.Start();
		std::this_thread::sleep_for(std::chrono::seconds(3));
		timer.Pause();
		auto status = timer.GetStatus();
		EXPECT_EQ(status.state, Timer::State::Ended);
		EXPECT_EQ(status.remaining, "00:00");
		EXPECT_EQ(status.elapsed, "00:02");
		timer.Reset();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		timer.Pause();
		status = timer.GetStatus();
		EXPECT_EQ(status.state, Timer::State::Stopped);
		EXPECT_EQ(status.remaining, "00:02");
		EXPECT_EQ(status.elapsed, "00:00");
	}

	// Verify time continue after resuming a pause
	TEST(TimerTest, TimeContinueAfterResume) {
		Timer timer(Minutes(0), Seconds(10));
		timer.Start();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		timer.Pause();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		timer.Resume();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		auto status = timer.GetStatus();
		EXPECT_EQ(status.state, Timer::State::Running);
		EXPECT_EQ(status.remaining, "00:08");
		EXPECT_EQ(status.elapsed, "00:02");
	}

	// Verify timer resets to default
	TEST(TimerTest, TimerReset) {
		Timer timer(Minutes(0), Seconds(10));
		timer.Start();
		std::this_thread::sleep_for(std::chrono::seconds(2));
		timer.Reset();
		auto status = timer.GetStatus();
		EXPECT_EQ(status.state, Timer::State::Stopped);
		EXPECT_EQ(status.remaining, "00:10");
		EXPECT_EQ(status.elapsed, "00:00");
	}

	// Verify timer resets after end
	TEST(TimerTest, TimerResetAfterEnd) {
		Timer timer(Minutes(0), Seconds(2));
		timer.Start();
		std::this_thread::sleep_for(std::chrono::seconds(3));
		timer.Reset();
		auto status = timer.GetStatus();
		EXPECT_EQ(status.state, Timer::State::Stopped);
		EXPECT_EQ(status.remaining, "00:02");
		EXPECT_EQ(status.elapsed, "00:00");
	}

	// Verify timer ends after target time elapsed
	TEST(TimerTest, TimerEnded) {
		Timer timer(Minutes(0), Seconds(2));
		timer.Start();
		std::this_thread::sleep_for(std::chrono::seconds(3));
		auto status = timer.GetStatus();
		EXPECT_EQ(status.state, Timer::State::Ended);
		EXPECT_EQ(status.remaining, "00:00");
		EXPECT_EQ(status.elapsed, "00:02");
	}

	// Verify timer start again after ended
	TEST(TimerTest, TimerStartAfterEnd) {
		Timer timer(Minutes(0), Seconds(2));
		timer.Start();
		std::this_thread::sleep_for(std::chrono::seconds(3));
		auto status = timer.GetStatus();
		EXPECT_EQ(status.state, Timer::State::Ended);
		EXPECT_EQ(status.remaining, "00:00");
		EXPECT_EQ(status.elapsed, "00:02");
		timer.Start();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		status = timer.GetStatus();
		EXPECT_EQ(status.state, Timer::State::Running);
		EXPECT_EQ(status.remaining, "00:01");
		EXPECT_EQ(status.elapsed, "00:01");
	}
}
