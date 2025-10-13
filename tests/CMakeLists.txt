add_executable(run_tests
	test_timer.cpp
)

target_link_libraries(run_tests PRIVATE
	gtest_main
	ZenithToolsLib
)

include(GoogleTest)
gtest_discover_tests(run_tests)
