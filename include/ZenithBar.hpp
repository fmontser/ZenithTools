#pragma once

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <memory>

namespace zenith {

	class ZenithBar {
		public:
			ZenithBar();
			~ZenithBar();

			sf::RenderWindow& GetWindow() const;

		private:
			std::unique_ptr<sf::RenderWindow> _window;
			sf::VideoMode _dynamicResolution;

			void InitWindow();
	};
}