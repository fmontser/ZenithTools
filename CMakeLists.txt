cmake_minimum_required(3.10)
project(ZenithTools LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(zenith
	src/main.cpp
)

target_include_directories(zenith PUBLIC include)