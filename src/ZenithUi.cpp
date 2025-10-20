#include "ZenithUi.hpp"
#include "ZenithBar.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

using namespace zenith;

ZenithUi::ZenithUi() {}

void ZenithUi::Run() {
	SetupEvents();
	RenderLoop();
}

void ZenithUi::RenderLoop() {
	auto& window = _bar.GetWindow();

	sf::Clock deltaClock;

	ImGui::SFML::Init(window);
	while (window.isOpen()) {
		ProcessEvents(window);
		ImGui::SFML::Update(window, deltaClock.restart());
		_bar.Render();
	}
	ImGui::SFML::Shutdown();
}

void ZenithUi::SetupEvents() {
	auto& window = _bar.GetWindow();

	_eventList.push_back([&, this](sf::Event e){
		if (e.type == sf::Event::Closed)
			window.close();
	});

}

void ZenithUi::ProcessEvents(sf::RenderWindow& window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(window, event);
		for (const auto& f : _eventList)
			f(event);
	}
}

