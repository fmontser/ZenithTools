#include "gtest/gtest.h"
#include "PomodoroSession.hpp"

namespace zenith {

	// Verify initial state
	TEST(PomodoroSessionTest, IsInitiallyIdle) {
		PomodoroSession session = PomodoroSession(1,1,0,0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		EXPECT_EQ(session.GetStatus().state, PomodoroSession::State::IDLE );
	}

	// Verify ongoing state
	TEST(PomodoroSessionTest, SwitchesToOnGoing) {
		PomodoroSession session = PomodoroSession(1,1,0,0);
		EXPECT_EQ(session.GetStatus().state, PomodoroSession::State::IDLE );
		session.StartActualRound();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(session.GetStatus().state, PomodoroSession::State::ONGOING);
	}

	// Verify completed state
	TEST(PomodoroSessionTest, SwitchesToCompleted) {
		PomodoroSession session = PomodoroSession(1,1,0,0);
		EXPECT_EQ(session.GetStatus().state, PomodoroSession::State::IDLE );
		session.StartActualRound();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(session.GetStatus().state, PomodoroSession::State::ONGOING);
		std::this_thread::sleep_for(std::chrono::minutes(1));
		EXPECT_EQ(session.GetStatus().state, PomodoroSession::State::COMPLETED);
	}


}
