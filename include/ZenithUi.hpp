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

	/**
	 * @brief Represents a list (vector) of functions for sf::events.
	 */ 
	using UiEventList = std::vector<std::function<void (sf::Event e)>>;
	
	class ZenithUi {
		public:

			/**
			 * @brief Default constructor
			 */
			ZenithUi();
		
			/**
			 * @brief Begins the execution and drawing of the ui.
			 */
			void Run();

		private:
			ZenithBar _bar;
			UiEventList _eventList;

			void RenderLoop();
			void SetupFonts();
			void SetupStyle();
			void SetupEvents();
			void ProcessEvents(sf::RenderWindow& window);
	};
}