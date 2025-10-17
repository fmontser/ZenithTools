
set(TEST_SOURCES
	test_timer.cpp
)

add_executable(run_tests ${TEST_SOURCES} ${APP_SOURCES})


target_include_directories(run_tests PRIVATE ../include)


target_link_libraries(run_tests PRIVATE
	gtest_main
	${APP_LIBS}
)


include(GoogleTest)
gtest_discover_tests(run_tests)