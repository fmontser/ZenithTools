#pragma once

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <memory>
#include "Timer.hpp"

namespace zenith {

	class ZenithBar {
		public:
			ZenithBar();
			~ZenithBar();

			sf::RenderWindow& GetRenderWindow() const;
			void Render();

		private:
			std::unique_ptr<sf::RenderWindow> _viewport;
			Timer _currentTimer;

			void InitViewport();
			void SetWindows();
			void SetPomodoroWindow(ImGuiViewport* viewport);
			void SetNoiseGeneratorWindow(ImGuiViewport* viewport);
			void SetDynamicResolution();
	};
}