cmake_minimum_required(VERSION 3.10)
project(ZenithTools LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(zenith
	src/main.cpp
	src/Exceptions.cpp
	src/Timer.cpp
)

target_include_directories(zenith PUBLIC include)