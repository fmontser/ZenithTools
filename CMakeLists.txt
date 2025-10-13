cmake_minimum_required(VERSION 3.31)
project(ZenithTools LANGUAGES CXX)

# Google Test
include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        release-1.12.1
)
	# C++17 mandatory for google tests
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# FTXUI
FetchContent_Declare(
	ftxui
	GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
	GIT_TAG        v5.0.0
)
FetchContent_MakeAvailable(ftxui)

# Compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(ENABLE_ASAN)
	message(STATUS "AddressSanitizer enabled.")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
endif()
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

# Libraries
add_library(ZenithToolsLib STATIC
	src/Timer.cpp
	src/Exceptions.cpp
)

target_include_directories(ZenithToolsLib PUBLIC include)

# Executable
add_executable(zenith
	src/main.cpp
)

target_link_libraries(zenith PRIVATE ZenithToolsLib)

# Extra
enable_testing()
add_subdirectory(tests)
