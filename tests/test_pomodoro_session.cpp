#include "gtest/gtest.h"
#include "PomodoroSession.hpp"
#include <thread>

#include <iostream>

namespace zenith {

	// Verify initial mode
	TEST(PomodoroSessionTest, SessionIsIdle) {
		PomodoroSession session = PomodoroSession(1,Seconds(2),Seconds(0),Seconds(0));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::IDLE );
	}

	// Verify ongoing mode
	TEST(PomodoroSessionTest, SessionToOngoing) {
		PomodoroSession session = PomodoroSession(1,Seconds(2),Seconds(0),Seconds(0));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::IDLE );
		session.StartActualRound();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
	}
 
	// Verify completed mode
	TEST(PomodoroSessionTest, SessionToCompleted) {
		PomodoroSession session = PomodoroSession(1,Seconds(2),Seconds(2),Seconds(0));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::IDLE );
		session.StartActualRound();
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		session.StartRestingPeriod();
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::COMPLETED);
	}

	// Verify completed mode
	TEST(PomodoroSessionTest, SessionRoundsToCompleted) {
		PomodoroSession session = PomodoroSession(2,Seconds(2),Seconds(2),Seconds(0));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::IDLE );
		session.StartActualRound();
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		session.StartRestingPeriod();
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));

		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::IDLE );
		session.StartActualRound();
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		session.StartRestingPeriod();
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));
		
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::COMPLETED);
	}

	// Verify largerest to completed
	TEST(PomodoroSessionTest, SessionLargeRestToCompleted) {
		PomodoroSession session = PomodoroSession(4,Seconds(2),Seconds(2),Seconds(3));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::IDLE );
		session.StartActualRound();
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		session.StartRestingPeriod();
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));

		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::IDLE );
		session.StartActualRound();
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		session.StartRestingPeriod();
		std::this_thread::sleep_for(std::chrono::milliseconds(3100));

		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::IDLE );
		session.StartActualRound();
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		session.StartRestingPeriod();
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));

		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::IDLE );
		session.StartActualRound();
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));
		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::ONGOING);
		session.StartRestingPeriod();
		std::this_thread::sleep_for(std::chrono::milliseconds(2100));

		EXPECT_EQ(session.GetStatus().mode, PomodoroSession::Mode::COMPLETED);
	}

}
