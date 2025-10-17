#pragma once

#include "ZenithBar.hpp"

namespace zenith {

	class ZenithUi {
		public:
			ZenithUi();
			
			void Run();

		private:
			ZenithBar _bar;
			sf::RenderWindow& _window;

			void RenderLoop();
			void ProcessEvents();
	};
}