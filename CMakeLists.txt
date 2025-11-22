cmake_minimum_required(VERSION 3.31)
project(ZenithTools LANGUAGES CXX)
include(FetchContent)

# DEPENDENCIES

	# GTEST
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG        release-1.12.1
	)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)

	# SFML as STATIC so no need to copy .dlls arround
	# TODO OpenAl32.dll still has to be copied (upgrade to SFML >3.0 when stable)
	if(WIN32)
		set(BUILD_SHARED_LIBS OFF)
		set(SFML_USE_STATIC_STD_LIBS OFF)
	endif()

	FetchContent_Declare(
		SFML
		GIT_REPOSITORY https://github.com/SFML/SFML.git
		GIT_TAG        2.6.2
	)
	FetchContent_MakeAvailable(SFML)

	# IMGUI
	FetchContent_Declare(
		imgui
		GIT_REPOSITORY https://github.com/ocornut/imgui.git
		GIT_TAG        v1.89.9
	)
	FetchContent_MakeAvailable(imgui)

	# IMGUI-SFML
	set(IMGUI_DIR ${imgui_SOURCE_DIR})# TODO check if necesary
	option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
	FetchContent_Declare(
		imgui-sfml
		GIT_REPOSITORY https://github.com/SFML/imgui-sfml
		GIT_TAG        v2.6.1
	)
	FetchContent_MakeAvailable(imgui-sfml)

# COMPILE OPTIONS

	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)

	if(ENABLE_ASAN)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
	endif()
	option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

# BINARIES

	set(APP_SOURCES
		src/main.cpp
		src/Timer.cpp
		src/Exceptions.cpp
		src/ZenithUi.cpp
		src/ZenithBar.cpp
		src/PomodoroSession.cpp
		src/SoundGenerator.cpp
		src/NoiseGenerator.cpp
	)

	set(APP_LIBS
		sfml-system
		sfml-window
		sfml-graphics
		sfml-audio
		ImGui-SFML
	)

	add_executable(zenith ${APP_SOURCES})

	target_include_directories(zenith PUBLIC include)
	target_link_libraries(zenith PUBLIC ${APP_LIBS})

	# This is needed to link SFML statically on windows
	if(WIN32)
		target_compile_definitions(zenith PUBLIC SFML_STATIC)
	endif()

	# Copies OpenAL32.dll to build dir
	# TODO move to SFML > 3.0 to avoid this
	if (WIN32)
		add_custom_command(TARGET zenith POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${CMAKE_SOURCE_DIR}/build/_deps/sfml-src/extlibs/bin/x64/openal32.dll"
				$<TARGET_FILE_DIR:zenith>
		)
	endif()


	# Apple is so special
	if(APPLE)
		target_link_libraries(zenith PUBLIC
			"-framework Cocoa"
			"-framework IOKit"
			"-framework CoreVideo"
			"-framework QuartzCore"
			"-framework Metal"
		)
	endif()

# TESTING
	enable_testing()
	add_subdirectory(tests)
