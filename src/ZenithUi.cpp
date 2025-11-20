#include "ZenithUi.hpp"
#include "Fonts.hpp"

using namespace zenith;

ZenithUi::ZenithUi() {}

void ZenithUi::Run() {
	SetupEvents();
	SetupStyle();
	RenderLoop();
}

void ZenithUi::RenderLoop() {
	auto& window = _bar.GetRenderWindow();
	sf::Clock deltaClock;
	ImGui::SFML::Init(window);
	while (window.isOpen()) {
		ProcessEvents(window);
		ImGui::SFML::Update(window, deltaClock.restart());
		_bar.Render();
	}
	ImGui::SFML::Shutdown();
}

void ZenithUi::SetupStyle() {

	ImVec4* colors = ImGui::GetStyle().Colors;
	Fonts();

	colors[ImGuiCol_WindowBg]         = ImVec4(0.51f, 0.60f, 0.57f, 0.94f);
	colors[ImGuiCol_Border]           = ImVec4(0.74f, 0.81f, 0.76f, 0.50f);
	colors[ImGuiCol_FrameBg]          = ImVec4(0.39f, 0.46f, 0.44f, 1.00f);
	colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.36f, 0.40f, 0.39f, 1.00f);
	colors[ImGuiCol_FrameBgActive]    = ImVec4(0.24f, 0.26f, 0.24f, 1.00f);
	colors[ImGuiCol_CheckMark]        = ImVec4(0.00f, 1.00f, 0.75f, 1.00f);
	colors[ImGuiCol_SliderGrab]       = ImVec4(0.00f, 1.00f, 0.75f, 0.48f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 0.59f, 1.00f);
	colors[ImGuiCol_Button]           = ImVec4(0.51f, 0.66f, 0.61f, 1.00f);
	colors[ImGuiCol_ButtonHovered]    = ImVec4(0.60f, 0.78f, 0.72f, 1.00f);
	colors[ImGuiCol_ButtonActive]     = ImVec4(0.68f, 0.91f, 0.76f, 1.00f);
	colors[ImGuiCol_PlotHistogram]    = ImVec4(0.00f, 1.00f, 0.75f, 1.00f);

	ImGui::GetStyle().FrameRounding = 5;
	ImGui::GetStyle().FrameBorderSize = 1;
	ImGui::GetStyle().GrabRounding = 5;
}

void ZenithUi::SetupEvents() {
	_eventList.push_back([this](sf::Event e){
		if (e.type == sf::Event::KeyPressed 
			&& e.key.code == sf::Keyboard::F4 && e.key.alt)
				this->_bar.GetRenderWindow().close();
	});

	_eventList.push_back([this](sf::Event e){
		if (e.type == sf::Event::Closed)
			 this->_bar.GetRenderWindow().close();
	});
}

void ZenithUi::ProcessEvents(sf::RenderWindow& window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(window, event);
		for (const auto& f : _eventList)
			f(event);
	}
}

