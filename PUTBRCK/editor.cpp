#include "editor.h"
#include <iostream>

editor::editor(sf::RenderWindow & App, game* thisGame) : App(App), thisGame(thisGame) {

	sf::Texture texture;
	texture.loadFromFile("assets/textures/background.png"); tx["background"] = texture;
	texture.loadFromFile("assets/textures/ball.png"); tx["ball"] = texture;
	texture.loadFromFile("assets/textures/paddle.png"); tx["paddle"] = texture;
	texture.loadFromFile("assets/textures/brick.png"); tx["brick"] = texture;
	texture.loadFromFile("assets/textures/bonus.png"); tx["bonus"] = texture;

	debug.setFont(font);
	debug.setCharacterSize(96);
	debug.setPosition({ 100, 100 });

	infoScore.setFont(font);
	infoScore.setCharacterSize(48);
	infoScore.setPosition({ 10, 10 });

	sp_background.setTexture(tx["background"]);
	sp_background.setColor(sf::Color(255, 255, 255, 255));
	if (!font.loadFromFile("assets/fonts/DEJAVUSANSMONO.ttf")) {
		std::cerr << "Error loading DEJAVUSANSMONO.ttf" << std::endl;
		throw - 1;
	}
	running = true;

	indicator.setSize({ tx["brick"].getSize().x * 1.0f, tx["brick"].getSize().y * 1.0f });
	indicator.setFillColor({ 255,255,255, 100 });
}

sf::Event event;
int editor::Run() {
	while (running) {

		float indicatorX = std::fmod(sf::Mouse::getPosition(App).x, indicator.getSize().x);
		float indicatorY = std::fmod(sf::Mouse::getPosition(App).y, indicator.getSize().y);
		float newBrickX = std::fmod(PLAYAREA_X, indicator.getSize().x) - indicator.getSize().x / 2 + 10 + sf::Mouse::getPosition(App).x - indicatorX;
		float newBrickY = std::fmod(PLAYAREA_Y, indicator.getSize().y) - indicator.getSize().y / 2 + 10 + sf::Mouse::getPosition(App).y - indicatorY;
		bool taken = false;
		if (newBrickX < PLAYAREA_X) { taken = true; }
		if (newBrickY < PLAYAREA_Y) { taken = true; }
		if (newBrickX > PLAYAREA_X + PLAYAREA_WIDTH) { taken = true; }
		if (newBrickY > PLAYAREA_Y + PLAYAREA_HEIGHT - indicator.getSize().y * 4) { taken = true; }
		indicator.setOrigin(indicator.getSize().x/2, indicator.getSize().y/2);
		indicator.setPosition({ newBrickX , newBrickY });

		for (auto &e : bricks) {
			if (newBrickX == e.x() && newBrickY == e.y()) {
				taken = true;
			}
		}
		if (taken) {
			indicator.setFillColor(sf::Color(255, 255, 255, 100));
		} else {
			indicator.setFillColor(sf::Color(100, 255, 100, 200));
		}

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
				case sf::Keyboard::Space:
					bonuses.emplace_back(bonus(App, tx["bonus"], { 0, 100 }, { 500.0f, 500.0f }));
					break;
				case sf::Keyboard::Q:
					audio.play("hit2");
					break;
				case sf::Keyboard::Home:
					thisGame->resume();
					return 0;		
				case sf::Keyboard::W:
					for (auto &e : bricks) {
						if (newBrickX == e.x() && newBrickY == e.y()) {
							taken = true;
						}
					}
					  if (!taken) {
						  bricks.emplace_back(brick(tx["brick"], 1, 100, { newBrickX, newBrickY }));
						  std::cout << "New brick added at: " << newBrickX << ", " << newBrickY << std::endl;
					  }
					  break;
				case sf::Keyboard::Return:
					level_.saveBricks(bricks);
					level_.writeToFile("assets/levels/test.txt");
					break;
				case sf::Keyboard::L:
					level_.readFromFile("assets/levels/test.txt");
					//bricks.erase(bricks.begin(), bricks.end());
					//bricks.clear();
					level_.loadBricks(bricks);
					for(auto &e:bricks) {
						e.setTexture(tx["brick"]); 
					}
					break;
				case sf::Keyboard::R:
					bricks.clear();

					break;				
				case sf::Keyboard::T:
					for (auto &e : bricks) {
						e.setTexture(tx["brick"]);
					}

					break;
				default:
					break;
				}
			}
			if(event.type == sf::Event::MouseButtonPressed) {
				switch (event.key.code) {
					case sf::Mouse::Left:						
						if (!taken) {
							bricks.emplace_back(brick(tx["brick"], 1, 100, { newBrickX, newBrickY }));
							std::cout << "New brick added at: " << newBrickX << ", " << newBrickY << std::endl;
						}
						break;
					case sf::Mouse::Right:
						for (auto &e : bricks) {
							if (newBrickX == e.x() && newBrickY == e.y()) {
								taken = true;
							}
						}
						if (taken) {
							bricks.emplace_back(brick(tx["brick"], 1, 100, { newBrickX, newBrickY }));
							bricks.erase(
								std::remove_if(bricks.begin(), bricks.end(),
									[&](brick &o) { return newBrickX == o.x() && newBrickY == o.y(); }),
								bricks.end());
							std::cout << "Brick deleted at: " << newBrickX << ", " << newBrickY << std::endl;
						}
						break;
					default:
						break;
				}
			}
		}
		
		sp_background.setColor(sf::Color(255, 255, 255, 255));

		//Clearing screen
		App.clear();
		//Drawing
		App.draw(sp_background);

		for (auto &e : bricks) {
			App.draw(e.sprite());
		}
		
		sf::CircleShape point(6);
		point.setFillColor(sf::Color::Red);

	

		App.draw(indicator);
		App.draw(point);
		App.draw(debug);
		App.draw(infoScore);
		App.display();

		//restartClock();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}