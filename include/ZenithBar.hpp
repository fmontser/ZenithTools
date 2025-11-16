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

	class ZenithBar {
		public:

			/**
			 * @brief Constructs a ZenithBar, a window that contains the ui.
			 */
			ZenithBar();
			~ZenithBar();

			sf::RenderWindow& GetRenderWindow() const;   ///< Gets the RenderWindow.
			void Render();                               ///< Render the next frame.

		private:
			std::unique_ptr<sf::RenderWindow> _renderWindow;                ///< Contains a smart pointer to the render window.
			ImGuiViewport* _viewport;                                       ///< Contains a pointer to the viewport.
			PomodoroSession _session;                                       ///< Contains the actual pomodoro session.
			std::vector<std::unique_ptr<NoiseGenerator>> _noiseGenerators;  ///< Contains the noise generators pointers.

			//TODO documentar
			bool _animateNoise;
			int _animateNoiseInterval;
			float _animateNoiseStrength;

			void AnimateNoiseSliders();

			/**
			 * @brief Restarts the actual session.
			 */
			void RestartSession();

			/**
			 * @brief Initializes the viewport on top of the OS window.
			 */
			void InitView();

			/**
			 * @brief Draws the pomodoro window and controls inside the bar.
			 * @note Uses ImGui library syntax.
			 */
			void DrawPomodoroWindow();

			/**
			 * @brief Draws the noise generator window and controls inside the bar.
			 * @note Uses ImGui library syntax.
			 */
			void DrawNoiseGeneratorWindow();

			/**
			 * @brief Sets the resolution for the bar dinamically.
			 * @note Uses OS resolution.
			 */
			void SetDynamicResolution();
	};
}