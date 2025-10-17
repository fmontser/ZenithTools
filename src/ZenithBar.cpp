#include "ZenithBar.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

using namespace zenith;

ZenithBar::ZenithBar() {
	InitWindow();
	SetupEvents();
}

ZenithBar::~ZenithBar() {}


void ZenithBar::Render() {

	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGuiWindowFlags windowFlags = 0
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings;

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);

	if (ImGui::Begin("ZenithBar",nullptr, windowFlags)) {
		ImGui::SameLine(10.0, 5.0);
		if (ImGui::Button("Close", ImVec2(100,50)))
			_window->close();

	};
	ImGui::End();

	_window->clear();
	ImGui::SFML::Render(*_window);
	_window->display();
}

sf::RenderWindow& ZenithBar::GetWindow() const { return *_window; }

const UiEventList& ZenithBar::GetUiEventList() const { return _eventList; }

void ZenithBar::InitWindow()
{
	_dynamicResolution = sf::VideoMode::getDesktopMode();
	_dynamicResolution.height /= 5; //TODO temp 5% heigh;
	
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
