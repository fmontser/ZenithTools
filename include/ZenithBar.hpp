#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <functional>

namespace zenith {

	class ZenithBar {
		public:
			ZenithBar();
			~ZenithBar();

			sf::RenderWindow& GetWindow() const;
			void Render();

		private:
			sf::VideoMode _dynamicResolution;
			std::unique_ptr<sf::RenderWindow> _window;

			void InitWindow();
	};
}