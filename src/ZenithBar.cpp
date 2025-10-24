#include "ZenithBar.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Exceptions.hpp"

using namespace zenith;

//TODO remove hardcoded values
/* ZenithBar::ZenithBar(): _session(PomodoroSession(2, Minutes(25),Minutes(5), Minutes(15))) {
	InitViewport();
}
 */
ZenithBar::ZenithBar(): _session(PomodoroSession(2, Seconds(5),Seconds(2), Seconds(3))) {
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

	Status status = _session.GetStatus();
	Mode _mode = status.mode;
	RoundMode _roundMode = status.roundMode;
	float _progress = status.progress;
	string _remainingTime = status.remainingTime;
	string _elapsedTime = status.elapsedTime;
	uint _roundsLeft = status.roundsLeft;
	
	if (ImGui::Begin("PomodoroWindow",nullptr, windowFlags)) {
		
		ImGui::BeginDisabled();
		for (uint i = 0; i < _roundsLeft; ++i) {
			ImGui::SameLine();
			ImGui::RadioButton("##Round_",false);
		}
		ImGui::EndDisabled();

		ImGui::SetWindowFontScale(5.0f);
		ImGui::Text(_remainingTime.c_str());
		ImGui::SetWindowFontScale(1.0f);


		ImGui::ProgressBar(_progress, ImVec2(-1.0f, 0.0f),_elapsedTime.c_str());


		if (_mode == Mode::IDLE) {
			if (ImGui::Button("Start", ImVec2(50,50))){
				if (_roundMode == RoundMode::IDLE)
					_session.StartActualRound();
				else if (_roundMode == RoundMode::RESTING)
					_session.StartRestingPeriod();
			}
		} else if (_mode == Mode::ONGOING) {
			if (ImGui::Button("Pause", ImVec2(50,50)))
				_session.PausePeriod();
		} else if (_mode == Mode::PAUSED) {
			if (ImGui::Button("Resume", ImVec2(50,50)))
				_session.ResumePeriod();
		}

		ImGui::SameLine();
		if (ImGui::Button("Reset", ImVec2(50,50)))
			_session.ResetPeriod();

		
		ImGui::SameLine();
		if (ImGui::Button("Restart", ImVec2(50,50)))
			_session.RestartSession();


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
