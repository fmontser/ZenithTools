#include "ZenithUi.hpp"
#include "ZenithBar.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

using namespace zenith;

ZenithUi::ZenithUi() : _window(_bar.GetWindow()) {}

void ZenithUi::Run() {
	RenderLoop();
}

void ZenithUi::RenderLoop() {
	
	sf::Clock deltaClock;

	ImGui::SFML::Init(_window);
	while (_window.isOpen()) {
		
		ProcessEvents();

		ImGui::SFML::Update(_window, deltaClock.restart());

		//TODO _bar draw logic

		_window.clear();
		ImGui::SFML::Render(_window);
		_window.display();
	}

	ImGui::SFML::Shutdown();
}

void zenith::ZenithUi::ProcessEvents()
{
	sf::Event event;
	while (_window.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(_window, event);
		for (const auto& f : _bar.GetUiEventList())
			f(event);
	}
}
