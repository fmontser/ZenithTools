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
			std::unique_ptr<sf::RenderWindow> _viewport;

			void InitViewport();
			void SetControls();
			void SetDynamicResolution();
	};
}