#include "ZenithBar.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Exceptions.hpp"

using namespace zenith;

constexpr uint FRAME_LIMIT = 60;
constexpr uint WINDOW_WIDTH = 1280;
constexpr uint WINDOW_HEIGHT = 240;
constexpr uint DEF_ANIM_INTERVAL = 10;
constexpr float DEF_ANIM_STRENGTH = 0.2f;

ZenithBar::ZenithBar() {
		InitView();
}

sf::RenderWindow& ZenithBar::GetRenderWindow() const { return *_renderWindow; }

void ZenithBar::Render() {
	DrawPomodoroWindow();
	DrawNoiseGeneratorWindow();
	AnimateNoiseSliders();
	_renderWindow->clear();
	ImGui::SFML::Render(*_renderWindow);
	_renderWindow->display();
}

void ZenithBar::InitView() {
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	_renderWindow = std::make_unique<sf::RenderWindow>(
		desktop,
		"ZenithTools",
		sf::Style::None
	);
	
	_renderWindow->setFramerateLimit(FRAME_LIMIT);
	_renderWindow->setSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));
	_renderWindow->setPosition(sf::Vector2i((desktop.width / 2) - (WINDOW_WIDTH / 2), 0));
	ImGui::SFML::Init(*_renderWindow);

	_sessionStarted =  false;
	_session = std::make_unique<PomodoroSession>(std::make_unique<SoundGenerator>());
	
	for (auto &&band : NoiseGenerator::GetDefaultBands())
		_noiseGenerators.push_back(std::make_unique<NoiseGenerator>(band));

	_animateNoise = false;
	_animateNoiseInterval = DEF_ANIM_INTERVAL;
	_animateNoiseStrength = DEF_ANIM_STRENGTH;
}

void ZenithBar::RestartSession(
	int rounds = 4, int workTime = 25, int restTime = 5, int longRestTime = 15) {
	_session = std::make_unique<PomodoroSession>(
		std::make_unique<SoundGenerator>(),
		static_cast<uint>(rounds),
		Minutes(workTime),
		Minutes(restTime),
		Minutes(longRestTime)
	);
}

void ZenithBar::DrawPomodoroWindow() {

	static ImGuiViewport* viewPort = ImGui::GetMainViewport();
	static ImVec2 winSize = ImVec2(viewPort->WorkSize.x / 2, viewPort->WorkSize.y);

	ImGui::SetNextWindowPos(viewPort->WorkPos);
	ImGui::SetNextWindowSize(winSize);

	static ImGuiWindowFlags windowFlags = 0
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings;

	Status status = _session.get()->GetStatus();
	
	if (ImGui::Begin("PomodoroWindow",nullptr, windowFlags)) {
		DrawSessionSettings();
		DrawSessionTokens(status);
		DrawTimerStatus(status);
		DrawSessionControl(status);
	};
}

void ZenithBar::DrawSessionSettings() {
	static int rounds = 4;
	static int workTime = 25;
	static int restTime = 5;
	static int longRestTime = 15;
	bool sessionChanged = false;

	if (ImGui::BeginTable("SessionControls", 4))
	{
		if (_sessionStarted)
			ImGui::BeginDisabled();
		
		ImGui::TableNextRow();
			
		ImGui::TableSetColumnIndex(0);
		
		ImGui::SetNextItemWidth(100);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Rounds");
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputInt("##Rounds", &rounds))
			sessionChanged = true;
		rounds = std::clamp(rounds, 4, 8);

		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(100);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Work time");
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputInt("##WorkTime", &workTime))
			sessionChanged = true;
		workTime = std::clamp(workTime, 25, 55);
		
		ImGui::TableSetColumnIndex(2);
		ImGui::SetNextItemWidth(100);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Rest time");
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputInt("##RestTime", &restTime))
			sessionChanged = true;
		restTime = std::clamp(restTime, 5, 15);

		ImGui::TableSetColumnIndex(3);
		ImGui::SetNextItemWidth(100);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Long rest time");
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputInt("##LongRestTime", &longRestTime))
			sessionChanged = true;
		longRestTime = std::clamp(longRestTime, 15, 30);
		
		if (_sessionStarted)
			ImGui::EndDisabled();

		if (sessionChanged) {
			RestartSession(rounds, workTime, restTime, longRestTime);
			_sessionStarted = false;
			sessionChanged = false;
		}

		ImGui::EndTable();
	}
}

void ZenithBar::DrawSessionTokens(Status status) {

	uint _roundsLeft = status.roundsLeft;
	uint _roundsTotal = status.roundsTotal;

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
}

void ZenithBar::DrawTimerStatus(Status status) {

	RoundMode _roundMode = status.roundMode;
	float _progress = status.progress;
	string _remainingTime = status.remainingTime;
	string _elapsedTime = status.elapsedTime;

	ImGui::SetWindowFontScale(5.0f);
	ImGui::Text(_remainingTime.c_str());
	ImGui::SetWindowFontScale(1.0f);

	//TODO refactor switch into session class
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
}

void ZenithBar::DrawSessionControl(Status status) {
	Mode _mode = status.mode;
	RoundMode _roundMode = status.roundMode;

	if (_mode == Mode::IDLE) {
		if (ImGui::Button("Start", ImVec2(50,50))){
			if (!_sessionStarted)
				_sessionStarted = true;
			if (_roundMode == RoundMode::IDLE)
				_session.get()->StartActualRound();
			else if (_roundMode == RoundMode::RESTING)
				_session.get()->StartRestingPeriod();
		}
	} else if (_mode == Mode::ONGOING) {
		if (ImGui::Button("Pause", ImVec2(50,50)))
			_session.get()->PausePeriod();
	} else if (_mode == Mode::PAUSED) {
		if (ImGui::Button("Resume", ImVec2(50,50)))
			_session.get()->ResumePeriod();
	}

	if (_mode != Mode::COMPLETED) {
		if (_sessionStarted)
			_sessionStarted = false;
		ImGui::SameLine();
		if (ImGui::Button("Reset", ImVec2(50,50)))
			_session.get()->ResetPeriod();
		ImGui::SameLine();
		if (ImGui::Button("Restart", ImVec2(50,50)))
			RestartSession();
	} else {
		if (ImGui::Button("Restart", ImVec2(50,50)))
			RestartSession();
	}

	ImGui::End();
}

void ZenithBar::DrawNoiseGeneratorWindow() {
	static ImGuiViewport* viewPort = ImGui::GetMainViewport();
	static ImVec2 winSize = ImVec2(viewPort->WorkSize.x / 2, viewPort->WorkSize.y);
	static ImVec2 winPos = ImVec2(viewPort->WorkSize.x / 2, 0);

	ImGui::SetNextWindowPos(winPos);
	ImGui::SetNextWindowSize(winSize);

	static ImGuiWindowFlags windowFlags = 0
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings;



	if (ImGui::Begin("NoiseGenWindow",nullptr, windowFlags)) {
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

		if (ImGui::Button(_animateNoise ? "Fixed" : "Anime", ImVec2(50, 50)))
			_animateNoise = !_animateNoise;

		static int animMinutes = 0;
		static int animSeconds = DEF_ANIM_INTERVAL;

		ImGui::SameLine();
		ImGui::BeginGroup();
		ImGui::Text("Minutes");
		ImGui::AlignTextToFramePadding();
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputInt("##AnimMinutes", &animMinutes))
			animMinutes = std::clamp(animMinutes, 0, 60);
		ImGui::EndGroup();

		ImGui::SameLine();
		ImGui::BeginGroup();
		ImGui::Text("Seconds");
		ImGui::AlignTextToFramePadding();
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputInt("##AnimSeconds", &animSeconds))
			animSeconds = std::clamp(animSeconds, 1, 60);
		ImGui::EndGroup();

		_animateNoiseInterval = (animMinutes * 60) + animSeconds;
		
		ImGui::SameLine();
		if (ImGui::Button("CLOSE", ImVec2(50,50)))
			_renderWindow->close();
		ImGui::End();
	};

}

void ZenithBar::AnimateNoiseSliders() {
	static uint frameNumber = 0;
	static uint elapsedSecs = 0;
	static uint fadeFrames = FRAME_LIMIT;
	static uint lastInterval = _animateNoiseInterval;

	if (lastInterval != _animateNoiseInterval) {
		frameNumber = 0;
		elapsedSecs = 0;
		lastInterval = _animateNoiseInterval;
	}
	
	static std::map<NoiseGenerator*, float> genTargetVolumeMap;
	if (genTargetVolumeMap.empty()){
		for (auto &&gen : _noiseGenerators) {
			genTargetVolumeMap[gen.get()] = 50.0f;
		}
	}

	if (!_animateNoise || NoiseGenerator::masterMuted)
		return;

	if (frameNumber++ == FRAME_LIMIT){
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