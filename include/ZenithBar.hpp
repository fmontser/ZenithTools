/**
 * @file ZenithBar.hpp
 * @author https://github.com/fmontser
 * @brief ZenithBar class definition.
 */

#pragma once

#include "Timer.hpp"
#include "PomodoroSession.hpp"
#include "NoiseGenerator.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace zenith {

	/**
	 * @brief ZenithBar containts all the controls and logui for the window ui.
	 */
	class ZenithBar {
		public:
			ZenithBar();                                ///< Default constructor.
			sf::RenderWindow& GetRenderWindow() const;  ///< Returns the RenderWindow object.
			void Render();                              ///< Render the next frame.

		private:
			std::unique_ptr<sf::RenderWindow> _renderWindow;
			std::unique_ptr<PomodoroSession> _session;
			std::vector<std::unique_ptr<NoiseGenerator>> _noiseGenerators;
			bool _sessionStarted;
			bool _animateNoise;
			int _animateNoiseInterval;
			float _animateNoiseStrength;
			
			void InitView();
			void RestartSession(int rounds, int workTime, int restTime, int longRestTime);
			void DrawPomodoroWindow();
			void DrawNoiseGeneratorWindow();
			void DrawSessionSettings();
			void DrawSessionTokens(Status status);
			void DrawTimerStatus(Status status);
			void DrawSessionControl(Status status);
			void AnimateNoiseSliders();
			void SetDynamicResolution();
	};
}