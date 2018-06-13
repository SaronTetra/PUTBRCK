#include <iostream>
#include "credits.h"

void credits::loadCredits(std::string filePath) {
	std::fstream file(filePath, std::ios::in);
	if (file) {
		std::string creditsLine;
		while (file) {
			std::getline(file, creditsLine);
			creditsText.push_back(creditsLine);
		}
		std::cout << "Credits loaded" << std::endl;
	} else {
		std::cerr << "Could not load credits file" << std::endl;
	}
	file.close();
}

credits::credits(sf::RenderWindow & App, game* thisGame) : App(App), thisGame(thisGame) {
	sp_logo.setTexture(tx["logo"]);
	sp_logo.setColor(sf::Color(255, 255, 255, 255));
	sp_logo.setOrigin(sp_logo.getLocalBounds().width / 2, sp_logo.getLocalBounds().height/2);
	sp_logo.setPosition( App.getSize().x/2, App.getSize().y/2);
	running = true;

	if (!font_.loadFromFile("assets/fonts/Sarpanch-Regular.ttf")) {
		std::cerr << "Error loading Sarpanch-Regular.ttf" << std::endl;
		throw - 1;
	}

	loadCredits("assets/credits.txt");

	sf::Vector2f lastPos = { sp_logo.getPosition().x, sp_logo.getPosition().y + 1024};
	for(auto& e:creditsText) {
		sf::Text temp;
		temp.setString(e);
		temp.setFont(font_);
		temp.setCharacterSize(64);
		temp.setOrigin(temp.getLocalBounds().width/2, temp.getLocalBounds().height / 2);
		temp.setPosition(App.getSize().x/2, lastPos.y + 128);		
		lastPos = temp.getPosition();
		text_.push_back(temp);
	}
}

int credits::Run() {
	sf::Event event;

	sp_logo.setPosition(App.getSize().x / 2, App.getSize().y / 2);
	sf::Vector2f lastPos = { sp_logo.getPosition().x, sp_logo.getPosition().y + 1024 };
	for (auto& e : text_) {
		e.setPosition(App.getSize().x / 2, lastPos.y + 128);
		lastPos = e.getPosition();
	}

	for(auto &e:music) {	
		e.second.setVolume(0);
	}

	music["credits"].play();
	music["credits"].setVolume(100);
	clock_.restart();

	//==========================================================================
	//VARIABLES
	//==========================================================================


	//==========================================================================
	//MAIN LOOP
	//==========================================================================
	while (running) {
		//==========================================================================
		//KEYBOARD INPUT
		//==========================================================================
		//Verifying events
		while (App.pollEvent(event)) {			
			// Window closed
			if (event.type == sf::Event::Closed) {
				return (-1);
			}
			//Key pressed
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					music["credits"].stop();
					return (3);
				case sf::Keyboard::Home:
					thisGame->resume();
					return 0;
				default:
					break;
				}
			}
		}
		if (clock_.getElapsedTime() > sf::seconds(3)) {
			sp_logo.setPosition(sp_logo.getPosition().x, sp_logo.getPosition().y - 2);
			for (auto &e : text_) {
				e.setPosition(e.getPosition().x, e.getPosition().y - 2);
			}
		}

		//Clearing screen
		App.clear();
		//Drawing
		App.draw(sp_logo);
		for(auto &e:text_) {
			App.draw(e);
		}


		App.display();
		//App.setMouseCursorVisible(true);
		//restartClock();

	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}