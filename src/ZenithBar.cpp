#include "ZenithBar.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Exceptions.hpp"

//TODO borrar test
#include <iostream>
#include <filesystem>

using namespace zenith;

//TODO remove hardcoded values
/* ZenithBar::ZenithBar(): _session(PomodoroSession(2, Minutes(25),Minutes(5), Minutes(15))) {
	InitViewport();
}
 */
ZenithBar::ZenithBar() : _session(PomodoroSession(std::make_unique<SoundGenerator>(), 4, Seconds(4),Seconds(2), Seconds(3))) {
	_sg = std::move(std::make_unique<SoundGenerator>());
	InitView();
}

ZenithBar::~ZenithBar() {}


void ZenithBar::Render() {

	DrawPomodoroWindow();
	DrawNoiseGeneratorWindow();
	_renderWindow->clear();
	ImGui::SFML::Render(*_renderWindow);
	_renderWindow->display();
}

sf::RenderWindow& ZenithBar::GetRenderWindow() const { return *_renderWindow; }

void ZenithBar::RestartSession() {
	//TODO hardcoded values
	_session = PomodoroSession(std::make_unique<SoundGenerator>(), 4, Seconds(4),Seconds(2), Seconds(3));
}

void ZenithBar::InitView()
{
	_renderWindow = std::make_unique<sf::RenderWindow>(
		sf::VideoMode::getDesktopMode(),
		"ZenithTools",
		sf::Style::None
	);

	//TODO remove hardcoded values
	_renderWindow->setFramerateLimit(60);
	_renderWindow->setSize(sf::Vector2u(1000, 300));
	_renderWindow->setPosition(sf::Vector2i(500,100));
	ImGui::SFML::Init(*_renderWindow);

	_viewport = ImGui::GetMainViewport();;
}


void ZenithBar::DrawPomodoroWindow() {


	ImVec2 winSize = ImVec2(_viewport->WorkSize.x / 2, _viewport->WorkSize.y);

	ImGui::SetNextWindowPos(_viewport->WorkPos);
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
	uint _roundsTotal = status.roundsTotal;
	
	if (ImGui::Begin("PomodoroWindow",nullptr, windowFlags)) {
		
		// Round tokens
		ImGui::Text("ROUNDS ");
		ImGui::BeginDisabled();
		for (uint i = 0; i < _roundsTotal; ++i) {
			ImGui::SameLine();
			if (i < _roundsLeft)
				ImGui::RadioButton("##Round_",true);
			else
				ImGui::RadioButton("##Round_",false);
		}
		ImGui::EndDisabled();

		// Primary timer
		ImGui::SetWindowFontScale(5.0f);
		ImGui::Text(_remainingTime.c_str());
		ImGui::SetWindowFontScale(1.0f);
		
		//Progress bar and indicator
		switch (_roundMode)
		{
			case RoundMode::IDLE: 
				ImGui::Text("IDLE");
				break;
			case RoundMode::WORKING: 
				ImGui::Text("WORK");
				break;
			case RoundMode::RESTING: 
				ImGui::Text("REST");
				break;
			case RoundMode::COMPLETED: 
				ImGui::Text("DONE");
				break;
			default:
				throw InvalidModeException();
				break;
		}
		ImGui::SameLine();
		ImGui::ProgressBar(_progress, ImVec2(-1.0f, 0.0f),_elapsedTime.c_str());

		//User controls
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

		if (_mode != Mode::COMPLETED) {
			ImGui::SameLine();
			if (ImGui::Button("Reset", ImVec2(50,50)))
				_session.ResetPeriod();
			ImGui::SameLine();
			if (ImGui::Button("Restart", ImVec2(50,50)))
				RestartSession();
		} else {
			if (ImGui::Button("Restart", ImVec2(50,50)))
				RestartSession();
		}

		ImGui::End();
	};

}

void ZenithBar::DrawNoiseGeneratorWindow() {


	ImVec2 winSize = ImVec2(500, 300);
	ImVec2 winPos = ImVec2(500, 0);
	SoundGenerator* sg = dynamic_cast<SoundGenerator*>(_sg.get());
	static float newVolume = 50.0f;

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
			_renderWindow->close();

		if (ImGui::VSliderFloat("##NoiseVolumeSlider", ImVec2(20,80), &newVolume , 0.0f, 100.0f, ""))
			sg->setVolume(newVolume);

		//TODO delete test button
		if (ImGui::Button("NOISE", ImVec2(20,20))) {
			sg->PlayRestBell();
		}

		ImGui::End();
	};

}

void ZenithBar::SetDynamicResolution() {
	//TODO not implemented
	throw NotImplementedException();
}


