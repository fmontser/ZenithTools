/**
 * @file ZenithUi.hpp
 * @author https://github.com/fmontser
 * @brief ZenithUi class definition.
 */

#pragma once

#include "ZenithBar.hpp"
#include <vector>
#include <functional>

namespace zenith {

	/// @brief Represents a list (vector) of functions for sf::events.
	using UiEventList = std::vector<std::function<void (sf::Event e)>>;
	
	class ZenithUi {
		public:

			/**
			 * @brief Constructs a ZenithUi object that controls the ui logic loop and events of the program.
			 */
			ZenithUi();
		
			/**
			 * @brief Begins the execution and drawing of the ui.
			 */
			void Run();

		private:
			ZenithBar _bar;           ///< Contains the ZenithBar object
			UiEventList _eventList;   ///< Contains the external or global events list.

			/**
			 * @brief The main loop for ui rendering.
			 */
			void RenderLoop();

			/**
			 * @brief Setups the fonts used in gui.
			 */
			void SetupFonts();

			/**
			 * @brief Defines the custom style for ImGui.
			 */
			void SetupStyle();

			/**
			 * @brief Setups any external or global event.
			 */
			void SetupEvents();

			/**
			 * @brief Process any external or global event previously set on SetupEvents().
			 */
			void ProcessEvents(sf::RenderWindow& window);
	};
}