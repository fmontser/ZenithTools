#pragma once

#include "ZenithBar.hpp"

namespace zenith {

	using UiEventList = std::vector<std::function<void (sf::Event e)>>;
	
	class ZenithUi {
		public:
			ZenithUi();
			
			void Run();

		private:
			ZenithBar _bar;
			UiEventList _eventList;

			void RenderLoop();
			void SetupEvents();
			void ProcessEvents(sf::RenderWindow& window);
	};
}