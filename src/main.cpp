#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>

int main(int argc, char* argv[]){
	
	//TODO Delete Imgui library test

	sf::VideoMode resolution = sf::VideoMode::getDesktopMode();
	resolution.height /= 10;
	
	sf::RenderWindow window	(
		resolution,
		"ZenithTools",
		sf::Style::None | sf::Style::Close
	);

	window.setFramerateLimit(60);
	window.setPosition(sf::Vector2i(0,0));


	ImGui::SFML::Init(window);

	sf::Clock deltaClock;
	while (window.isOpen()) {
		
		
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(window, event);
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());
		window.clear();
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}