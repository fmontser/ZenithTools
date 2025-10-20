#include "ZenithBar.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Exceptions.hpp"

using namespace zenith;

ZenithBar::ZenithBar() {
	InitViewport();
}

ZenithBar::~ZenithBar() {}


void ZenithBar::Render() {

	SetControls();
	_viewport->clear();
	ImGui::SFML::Render(*_viewport);
	_viewport->display();
}

sf::RenderWindow& ZenithBar::GetWindow() const { return *_viewport; }


void ZenithBar::InitViewport()
{
	_viewport = std::make_unique<sf::RenderWindow>(
		sf::VideoMode::getDesktopMode(),
		"ZenithTools",
		sf::Style::None
	);

	//TODO remove hardcoded values
	_viewport->setFramerateLimit(60);
	_viewport->setSize(sf::Vector2u(1080, 386));
	_viewport->setPosition(sf::Vector2i(300,100));

	ImGui::SFML::Init(*_viewport);
}

void ZenithBar::SetControls() {

	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);

	ImGuiWindowFlags windowFlags = 0
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::Begin("ZenithBar",nullptr, windowFlags)) {
		ImGui::SameLine(10.0, 5.0);
		if (ImGui::Button("X", ImVec2(50,50)))
			_viewport->close();

		ImGui::End();
	};
}

void ZenithBar::SetDynamicResolution()
{
	//TODO
	throw NotImplementedException();
}
