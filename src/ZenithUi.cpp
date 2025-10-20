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
	auto& window = _bar.GetRenderWindow();

	sf::Clock deltaClock;

	ImGui::SFML::Init(window);
	while (window.isOpen()) {
		ProcessEvents(window);
		ImGui::SFML::Update(window, deltaClock.restart());
		_bar.Render();
	}
	ImGui::SFML::Shutdown();
}

#include <iostream>
void ZenithUi::SetupEvents() {

	// Alt+F4 close //TODO not working due to sf::Style::None
	_eventList.push_back([this](sf::Event e){
		if (e.type == sf::Event::KeyPressed 
			&& e.key.code == sf::Keyboard::F4 && e.key.alt)
				this->_bar.GetRenderWindow().close();
	});

	// Other close events //TODO not working due to sf::Style::None
	_eventList.push_back([this](sf::Event e){
		if (e.type == sf::Event::Closed)
			 this->_bar.GetRenderWindow().close();
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

