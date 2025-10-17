#include "ZenithBar.hpp"

using namespace zenith;

ZenithBar::ZenithBar() {
	InitWindow();
}

ZenithBar::~ZenithBar() {}

sf::RenderWindow& ZenithBar::GetWindow() const { return *_window; }

void ZenithBar::InitWindow()
{
	_dynamicResolution = sf::VideoMode::getDesktopMode();
	_dynamicResolution.height /= 10; // 10% heigh;
	
	_window = std::make_unique<sf::RenderWindow>(
		_dynamicResolution,
		"ZenithTools",
		sf::Style::None | sf::Style::Close
	);
	_window->setFramerateLimit(60);
	_window->setPosition(sf::Vector2i(0,0));

	ImGui::SFML::Init(*_window);
}

