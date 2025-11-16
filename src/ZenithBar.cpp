#include "ZenithBar.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Exceptions.hpp"
#include <map>

using namespace zenith;

constexpr uint frameLimit = 60;

//TODO remove hardcoded values
ZenithBar::ZenithBar() : _session(PomodoroSession(
	std::make_unique<SoundGenerator>(), 4, Seconds(4),Seconds(2), Seconds(3))) {
		InitView();
}
	
void ZenithBar::InitView() {
	_renderWindow = std::make_unique<sf::RenderWindow>(
		sf::VideoMode::getDesktopMode(),
		"ZenithTools",
		sf::Style::None
	);
	
	//TODO remove hardcoded values
	_renderWindow->setFramerateLimit(frameLimit);
	_renderWindow->setSize(sf::Vector2u(1000, 300));
	_renderWindow->setPosition(sf::Vector2i(500,100));
	ImGui::SFML::Init(*_renderWindow);
	
	_viewport = ImGui::GetMainViewport();
	
	for (auto &&band : NoiseGenerator::GetDefaultBands())
		_noiseGenerators.push_back(std::make_unique<NoiseGenerator>(band));

	_animateNoise = false;
	_animateNoiseInterval = 10;
	_animateNoiseStrength = 0.2f;
}

ZenithBar::~ZenithBar() {}

void ZenithBar::Render() {
	DrawPomodoroWindow();
	DrawNoiseGeneratorWindow();
	AnimateNoiseSliders();
	_renderWindow->clear();
	ImGui::SFML::Render(*_renderWindow);
	_renderWindow->display();
}

sf::RenderWindow& ZenithBar::GetRenderWindow() const { return *_renderWindow; }

void ZenithBar::RestartSession()
{
	//TODO hardcoded values
	_session = PomodoroSession(std::make_unique<SoundGenerator>(), 4, Seconds(4),Seconds(2), Seconds(3));
}

void ZenithBar::DrawPomodoroWindow() {


	static ImVec2 winSize = ImVec2(_viewport->WorkSize.x / 2, _viewport->WorkSize.y);

	ImGui::SetNextWindowPos(_viewport->WorkPos);
	ImGui::SetNextWindowSize(winSize);

	static ImGuiWindowFlags windowFlags = 0
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
	static ImVec2 winSize = ImVec2(500, 300);
	static ImVec2 winPos = ImVec2(500, 0);

	ImGui::SetNextWindowPos(winPos);
	ImGui::SetNextWindowSize(winSize);

	static ImGuiWindowFlags windowFlags = 0
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::Begin("NoiseGenWindow",nullptr, windowFlags)) {
		
		ImGui::SameLine(450, 0);
		if (ImGui::Button("X", ImVec2(50,50)))
			_renderWindow->close();


		if (ImGui::BeginTable("VolumeControls", 11))
		{
			ImGui::TableNextRow();

 			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Master");

			if (ImGui::VSliderFloat("##MasterVolumeSlider", ImVec2(30,100),
				&NoiseGenerator::masterVolume, 0.0f, 1.0f, "")) {
					for (auto &&gen : _noiseGenerators)
						gen->setVolume(gen->volume * NoiseGenerator::masterVolume);
			}

			if (ImGui::Button(NoiseGenerator::masterMuted ? "S" : "P", ImVec2(30,30))) {
				NoiseGenerator::masterMuted = !NoiseGenerator::masterMuted;
				for (auto &&gen : _noiseGenerators) {
					if (gen->getStatus() == SoundGenerator::Playing)
						gen->stop();
					else if (!gen->muted)
						gen->play();
				}
			}

			uint col = 1;
			for (auto &&gen : _noiseGenerators) {
				ImGui::TableSetColumnIndex(col++);
				ImGui::PushID(gen.get());

				ImGui::AlignTextToFramePadding();
				ImGui::Text(gen->GetBandText().c_str());

				if (ImGui::VSliderFloat("##VolumeSlider", ImVec2(30,100), &gen->volume ,
					0.0f, 100.0f, ""))
						gen->setVolume(gen->volume * NoiseGenerator::masterVolume);

				if (ImGui::Button(gen->muted ? "P" : "S", ImVec2(30,30))) {
					gen->muted = !gen->muted;
					if (gen->getStatus() == SoundGenerator::Playing)
						gen->stop();
					else
						gen->play();
				}

				ImGui::PopID();
			}
			
			ImGui::EndTable();
		}


			if (ImGui::Button(_animateNoise ? "Fixed" : "Anime", ImVec2(50, 20)))
				_animateNoise = !_animateNoise;

			ImGui::SliderInt("##AnimationTimeSlider", &_animateNoiseInterval, 1,60, "%ds");
			ImGui::SliderFloat("##AnimationStrenghtSlider", &_animateNoiseStrength, 0.1f,0.3f, "%.1f POWER");



		ImGui::End();
	};

}

void ZenithBar::AnimateNoiseSliders() {
	static uint frameNumber = 0;
	static uint elapsedSecs = 0;
	static uint fadeFrames = frameLimit;
	
	static std::map<NoiseGenerator*, float> genTargetVolumeMap;
	if (genTargetVolumeMap.empty()){
		for (auto &&gen : _noiseGenerators) {
			genTargetVolumeMap[gen.get()] = 50.0f;
		}
	}

	if (!_animateNoise || NoiseGenerator::masterMuted)
		return;

	if (frameNumber++ == frameLimit){
		frameNumber = 0;
		elapsedSecs++;
	}
	
	if (elapsedSecs == _animateNoiseInterval){
		elapsedSecs = 0;
		
		for (auto &&gen : _noiseGenerators) {
			if (gen->muted)
			continue;
			float gain = (rand() % 201 - 100) * _animateNoiseStrength;
			genTargetVolumeMap[gen.get()] = std::clamp(gen->volume + gain, 0.0f, 100.0f);
		}
	}

	for (auto &&gen : _noiseGenerators) {
		float targetVolume = genTargetVolumeMap[gen.get()];
		if (gen->volume <= targetVolume) {
			gen->volume += targetVolume / fadeFrames;
			if (gen->volume > targetVolume)
				gen->volume = targetVolume;
		}
		else if (gen->volume >= targetVolume) {
			gen->volume -= targetVolume / fadeFrames;
			if (gen->volume < targetVolume)
				gen->volume = targetVolume;
		}
		gen->setVolume(gen->volume * NoiseGenerator::masterVolume);
	}
}


void ZenithBar::SetDynamicResolution() {
	//TODO not implemented
	throw NotImplementedException();
}


