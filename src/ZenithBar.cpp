#include "ZenithBar.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Exceptions.hpp"

using namespace zenith;

//TODO remove hardcoded values
ZenithBar::ZenithBar(): _session(PomodoroSession(8, Minutes(25),Minutes(5), Minutes(15))) {
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
	DrawPomodoroWindow(viewport);
	DrawNoiseGeneratorWindow(viewport);
}

void ZenithBar::DrawPomodoroWindow(ImGuiViewport* viewport) {
	ImVec2 winSize = ImVec2(viewport->WorkSize.x / 2, viewport->WorkSize.y);

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(winSize);

	ImGuiWindowFlags windowFlags = 0
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings;

/* 	const PomodoroSession::Round& _actualRound = _session.GetActualRound();

	const Timer *_actualTimer = nullptr;
	if (_actualRound.mode == PomodoroSession::RoundMode::WORKING)
		_actualTimer = &_actualRound.workTimer;
	else if (_actualRound.mode == PomodoroSession::RoundMode::RESTING)
		_actualTimer = &_actualRound.restTimer;

	float _progress = _actualRound.progress;

	string _remainingTime = _actualTimer->GetStatus().remaining;
	string _elapsedTime; */

	if (ImGui::Begin("PomodoroWindow",nullptr, windowFlags)) {
		

		ImGui::End();
	};

}

void ZenithBar::DrawNoiseGeneratorWindow(ImGuiViewport *viewport) {


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
