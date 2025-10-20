#include "ZenithBar.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Exceptions.hpp"

using namespace zenith;

ZenithBar::ZenithBar() : _currentTimer(Timer(25,0)){
	InitViewport();
}

ZenithBar::~ZenithBar() {}


void ZenithBar::Render() {

	SetWindows();
	_viewport->clear();
	ImGui::SFML::Render(*_viewport);
	_viewport->display();
}

sf::RenderWindow& ZenithBar::GetRenderWindow() const { return *_viewport; }


void ZenithBar::InitViewport()
{
	_viewport = std::make_unique<sf::RenderWindow>(
		sf::VideoMode::getDesktopMode(),
		"ZenithTools",
		sf::Style::None
	);

	//TODO remove hardcoded values
	_viewport->setFramerateLimit(60);
	_viewport->setSize(sf::Vector2u(1000, 300));
	_viewport->setPosition(sf::Vector2i(500,100));

	ImGui::SFML::Init(*_viewport);
}

void ZenithBar::SetWindows() {
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	SetPomodoroWindow(viewport);
	SetNoiseGeneratorWindow(viewport);
}

void ZenithBar::SetPomodoroWindow(ImGuiViewport* viewport) {
	ImVec2 winSize = ImVec2(viewport->WorkSize.x / 2, viewport->WorkSize.y);

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(winSize);

	ImGuiWindowFlags windowFlags = 0
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::Begin("PomodoroWindow",nullptr, windowFlags)) {
		
		ImGui::BeginDisabled(true);
		ImGui::RadioButton(" ", true);
	
		ImGui::SameLine(0, 10);
		ImGui::RadioButton(" ", false);
		ImGui::SameLine(0, 10);
		ImGui::RadioButton(" ", false);
		ImGui::SameLine(0, 10);
		ImGui::RadioButton(" ", false);
		ImGui::SameLine(0, 10);
		ImGui::RadioButton(" ", false);
		ImGui::SameLine(0, 10);
		ImGui::RadioButton(" ", false);
		ImGui::SameLine(0, 10);
		ImGui::RadioButton(" ", false);
		ImGui::SameLine(0, 10);
		ImGui::RadioButton(" ", false);
		ImGui::EndDisabled();

		if (ImGui::Button("Start", ImVec2(50,50)))
			_currentTimer.Start();

		ImGui::SameLine(0, 5);
		if (ImGui::Button("Pause", ImVec2(50,50)))
			_currentTimer.Pause();

		ImGui::SameLine(0, 5);
		if (ImGui::Button("Resume", ImVec2(50,50)))
			_currentTimer.Resume();

		ImGui::SameLine(0, 5);
		if (ImGui::Button("Reset", ImVec2(50,50)))
			_currentTimer.Reset();

		ImGui::ProgressBar(0.90, ImVec2(-1.0f, 0.0f),
			_currentTimer.GetStatus().remaining.c_str());

		ImGui::End();
	};

}

void ZenithBar::SetNoiseGeneratorWindow(ImGuiViewport *viewport) {


	ImVec2 winSize = ImVec2(500, 300);
	ImVec2 winPos = ImVec2(500, 0);

	ImGui::SetNextWindowPos(winPos);
	ImGui::SetNextWindowSize(winSize);

	ImGuiWindowFlags windowFlags = 0
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::Begin("NoiseGenWindow",nullptr, windowFlags)) {
		
		ImGui::SameLine(450, 0);
		if (ImGui::Button("X", ImVec2(50,50)))
			_viewport->close();

		ImGui::SetCursorPos(ImVec2(100,120));
		ImGui::Text("NOISE GEN PLACEHOLDER");

		ImGui::End();
	};

}

void ZenithBar::SetDynamicResolution()
{
	//TODO
	throw NotImplementedException();
}
