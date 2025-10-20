#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <functional>
#include "Timer.hpp"

namespace zenith {

	class ZenithBar {
		public:
			ZenithBar();
			~ZenithBar();

			sf::RenderWindow& GetWindow() const;
			void Render();

		private:
			std::unique_ptr<sf::RenderWindow> _viewport;
			Timer _currentTimer;

			void InitViewport();
			void SetControls();
			void SetDynamicResolution();
	};
}