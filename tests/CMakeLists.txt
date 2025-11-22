set(TEST_SOURCES
	test_timer.cpp
	test_pomodoro_session.cpp
	${CMAKE_SOURCE_DIR}/src/Exceptions.cpp
	${CMAKE_SOURCE_DIR}/src/Timer.cpp
	${CMAKE_SOURCE_DIR}/src/PomodoroSession.cpp
)

add_executable(zenith_tests ${TEST_SOURCES})

target_include_directories(zenith_tests PRIVATE ../include)

target_link_libraries(zenith_tests PRIVATE
	gtest_main
	gmock
	sfml-audio
)

include(GoogleTest)
gtest_discover_tests(zenith_tests)
