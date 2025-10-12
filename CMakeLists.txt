cmake_minimum_required(VERSION 3.10)
project(ZenithTools LANGUAGES CXX)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG    release-1.12.1
)
# C++17 mandatory for google tests
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Option
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Libraries
add_library(ZenithToolsLib
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
