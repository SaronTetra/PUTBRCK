#include <iostream>
#include "menu.h"
#include <string>


menu::menu(sf::RenderWindow & App, game* thisGame) : App(App), thisGame(thisGame) {
	sp_logo.setTexture(tx["logo"]);
	sp_logo.setColor(sf::Color(255, 255, 255, 255));
	sp_logo.setOrigin(sp_logo.getLocalBounds().width / 2, sp_logo.getLocalBounds().height / 2);
	sp_logo.setPosition(App.getSize().x / 2, 200);
	running = true;

	sp_bg.setTexture(tx["backgroundMenu"]);
	sp_bg.setColor(sf::Color(255, 255, 255, 255));

	if (!font_.loadFromFile("assets/fonts/DEJAVUSANSMONO.ttf")) {
		std::cerr << "Error loading DEJAVUSANSMONO.ttf" << std::endl;
		throw - 1;
	}

	menuText = { "Start", "Restart", "Editor", "Credits", "Quit"};
	sf::Vector2f lastPos = { sp_logo.getPosition().x, sp_logo.getPosition().y + 100 };
	for (auto& e : menuText) {
		sf::Text temp;
		temp.setString(e);
		temp.setFont(font_);
		temp.setCharacterSize(64);
		temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
		temp.setPosition(App.getSize().x / 2, lastPos.y + 128);
		lastPos = temp.getPosition();
		text_.push_back(temp);
	}
}

bool menu::mouseOverlap(sf::Text& e) {
	float mousePosX = sf::Mouse::getPosition(App).x;
	float mousePosY = sf::Mouse::getPosition(App).y;
	float width2 = e.getLocalBounds().width / 2;
	float height2 = e.getLocalBounds().height / 2;
	float x1 = e.getPosition().x - width2;
	float x2 = e.getPosition().x + width2;
	float y1 = e.getPosition().y - height2;
	float y2 = e.getPosition().y + height2;
	return (mousePosX > x1 && mousePosX < x2 && mousePosY > y1 && mousePosY < y2);
}

int menu::Run() {
	sf::Event event;

	for (auto &e : music) {
		e.second.setVolume(0);
	}

	//music["menu"].play();
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
					return (-1);
				case sf::Keyboard::Home:
					//thisGame->resume();
					return 0;
				default:
					break;
				}
			}
		}
		
		for(auto& e:text_) {
			if(mouseOverlap(e)) {
				e.setFillColor(sf::Color(244, 59, 154));
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if(e.getString() == "Start") {
						thisGame->resume();
						return 0;
					}
					if(e.getString() == "Editor") {
						return 1;
					}
					if(e.getString() == "Credits") {
						return 2;
					}
					if(e.getString() == "Quit") {
						return -1;
					}
					if(e.getString() == "Restart") {
						thisGame->restart();
						thisGame->resume();
						return 0;
					}
				}
			}
			else {
				//e.setFillColor(sf::Color(53, 190, 235));
				e.setFillColor(sf::Color::White);
			}
		}

		//Clearing screen
		App.clear();
		//Drawing
		App.draw(sp_bg);
		App.draw(sp_logo);
		for (auto &e : text_) {
			App.draw(e);
		}


		App.display();
		App.setMouseCursorVisible(true);
		//restartClock();

	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}