#include "ZenithBar.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

using namespace zenith;

ZenithBar::ZenithBar() {
	InitWindow();
	SetupEvents();
}

ZenithBar::~ZenithBar() {}

sf::RenderWindow& ZenithBar::GetWindow() const { return *_window; }

const UiEventList& ZenithBar::GetUiEventList() const { return _eventList; }

void ZenithBar::InitWindow()
{
	_dynamicResolution = sf::VideoMode::getDesktopMode();
	_dynamicResolution.height /= 10; //TODO temp 10% heigh;
	
	_window = std::make_unique<sf::RenderWindow>(
		_dynamicResolution,
		"ZenithTools",
		sf::Style::None | sf::Style::Close
	);
	_window->setFramerateLimit(60);
	_window->setPosition(sf::Vector2i(0,0));

	ImGui::SFML::Init(*_window);
}

void ZenithBar::SetupEvents() {

	_eventList.push_back([this](sf::Event e){
		if (e.type == sf::Event::Closed)
			_window->close();
	});

}
