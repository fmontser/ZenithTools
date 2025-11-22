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
	 * @brief ZenithBar containts all the controls and logic for the window ui.
	 */
	class ZenithBar {
		public:
			/**
			 * @brief Default constructor
			 */		
			ZenithBar();

			/**
			 * @brief Returns the RenderWindow object
			 */
			sf::RenderWindow& GetRenderWindow() const;
			
			/**
			 * @brief Renders the next frame
			 */
			void Render();

			/**
			 * @brief Set the large font used for the timer
			 * @param font A pointer to the loaded ImFont data
			 */
			void SetLargeFont(ImFont* font);

		private:
			std::unique_ptr<sf::RenderWindow> _renderWindow;
			std::unique_ptr<PomodoroSession> _session;
			std::vector<std::unique_ptr<NoiseGenerator>> _noiseGenerators;
			ImFont* _largeFont;

			bool _sessionStarted;
			bool _animateNoise;
			int _animateNoiseInterval;
			float _animateNoiseStrength;
			
			void InitView();
			void RestartSession(int rounds, int workTime, int restTime, int longRestTime);
			void DrawPomodoroWindow();
			void DrawSessionSettings();
			void DrawSessionTokens(Status status);
			void DrawTimerStatus(Status status);
			void DrawSessionControl(Status status);
			void DrawNoiseGeneratorWindow();
			void DrawFilteredNoiseSliders();
			void DrawAnimationControls();
			void AnimateNoiseSliders();
			void SetDynamicResolution();
	};
}