#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <functional>

namespace zenith {

	using UiWindow = std::unique_ptr<sf::RenderWindow>;
	using UiEventList = std::vector<std::function<void (sf::Event e)>>;
	
	class ZenithBar {
		public:
			ZenithBar();
			~ZenithBar();

			sf::RenderWindow& GetWindow() const;
			const UiEventList& GetUiEventList() const;
			void Render();

		private:
			sf::VideoMode _dynamicResolution;
			UiWindow _window;
			UiEventList _eventList;

			void InitWindow();
			void SetupEvents();
	};
}