#pragma once

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <memory>
#include "Timer.hpp"
#include "PomodoroSession.hpp"

namespace zenith {

	class ZenithBar {
		public:
			ZenithBar();
			~ZenithBar();

			sf::RenderWindow& GetRenderWindow() const;
			void Render();

		private:
			std::unique_ptr<sf::RenderWindow> _viewport;
			PomodoroSession _session;

			void InitViewport();
			void SetWindows();
			void DrawPomodoroWindow(ImGuiViewport* viewport);
			void DrawNoiseGeneratorWindow(ImGuiViewport* viewport);
			void SetDynamicResolution();

	};
}