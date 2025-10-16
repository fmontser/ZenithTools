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

	# SFML (IMGUI backend)
	FetchContent_Declare(
		SFML
		GIT_REPOSITORY https://github.com/SFML/SFML.git
		GIT_TAG        2.6.1
	)
	FetchContent_MakeAvailable(SFML)

	# IMGUI
	FetchContent_Declare(
		imgui
		GIT_REPOSITORY https://github.com/ocornut/imgui.git
		GIT_TAG        v1.90.8
	)
	FetchContent_MakeAvailable(imgui)



	# IMGUI-SFML Binding
	FetchContent_Declare(
		imgui-sfml
		GIT_REPOSITORY https://github.com/SFML/imgui-sfml
		GIT_TAG        v2.6
	)
	set(IMGUI_DIR ${imgui_SOURCE_DIR})
	option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
	FetchContent_MakeAvailable(imgui-sfml)


# COMPILE OPTIONS
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

	if(ENABLE_ASAN)
		message(STATUS "AddressSanitizer enabled.")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
	endif()
	option(ENABLE_ASAN "Enable AddressSanitizer" OFF)


# LIBS
	add_library(ZenithToolsLib STATIC
		src/Timer.cpp
		src/Exceptions.cpp
	)
	target_include_directories(ZenithToolsLib PUBLIC include)

	add_library(imguiLib STATIC
		${imgui_SOURCE_DIR}/imgui.cpp
		${imgui_SOURCE_DIR}/imgui_demo.cpp
		${imgui_SOURCE_DIR}/imgui_draw.cpp
		${imgui_SOURCE_DIR}/imgui_tables.cpp
		${imgui_SOURCE_DIR}/imgui_widgets.cpp
	)
	target_include_directories(imguiLib PUBLIC ${imgui_SOURCE_DIR})

# BINARIES
	add_executable(zenith
		src/main.cpp
	)

	target_link_libraries(zenith PRIVATE
		ZenithToolsLib
		imguiLib
		sfml-system
		sfml-window
		sfml-graphics
		sfml-audio
		ImGui-SFML
	)

# MISC
	enable_testing()
	add_subdirectory(tests)
