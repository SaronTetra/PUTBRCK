#include "game.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>
#include "myrandom.h"

game::game(sf::RenderWindow &App) : App(App) {
	sf::Event Event;
	running = true;
	paused = false;
	



	info.setFont(font);
	info.setCharacterSize(128);
	info.setOutlineColor(sf::Color::Black);
	info.setOutlineThickness(2);
	info.setOrigin(info.getLocalBounds().width / 2, info.getLocalBounds().height / 2);
	info.setPosition(App.getSize().x / 2, App.getSize().y/2);

	infoScore.setFont(font);
	infoScore.setCharacterSize(48);
	infoScore.setPosition({ 10, 10 });

	infoCannon.setFont(font);
	infoCannon.setCharacterSize(48);
	infoCannon.setPosition({ 10, 100 });
	infoCannon.setString("Cannon: ");

	sp_background.setTexture(tx["background"]);
	sp_background.setColor(sf::Color(255, 255, 255, 255));
	if (!font.loadFromFile("assets/fonts/Sarpanch-Regular.ttf")) {
		std::cerr << "Error loading Sarpanch-Regular.ttf" << std::endl;
		throw - 1;
	}
	pad = new paddle(App, tx["paddle"]);


		currentLevel = 0;
		level temp;
		temp.readFromFile("assets/levels/official/c0.txt"); levels.push_back(temp);
		temp.readFromFile("assets/levels/official/c1.txt"); levels.push_back(temp);
		temp.readFromFile("assets/levels/official/c2.txt"); levels.push_back(temp);
		temp.readFromFile("assets/levels/official/c3.txt"); levels.push_back(temp);
		temp.readFromFile("assets/levels/official/c4.txt"); levels.push_back(temp);
		temp.readFromFile("assets/levels/official/c5.txt"); levels.push_back(temp);
		temp.readFromFile("assets/levels/official/c6.txt"); levels.push_back(temp);


		levels[currentLevel].loadBricks(bricks);
		for (auto &e : bricks) {
			e.setTexture(tx["brick"]);
		}


	balls.emplace_back(ball(App, tx["ball"], { 300, 300 }, { pad->x(), pad->y() + 10 }));
	balls.back().moving = false;

	for (int i = 0; i < 25; ++i) {
		points.emplace_back(ball(App, tx["point"], { 0, 0 }, { -100, -100 }));
		points.back().sprite().setColor(sf::Color(0,255,255));
	}
	currentPoint = 0;
	cannon = 0;
	cannonFire = true;
	bool laserPlayed = false;
	toReset = false;
}

//==========================================================================
//NEXT LEVEL
//==========================================================================
void game::nextLevel() {
	if (currentLevel >= levels.size() - 1) {
		return;
	}
	if(toReset) {
		currentLevel = -1;
		
	}
	++currentLevel;
	std::cout << "Current level: " << currentLevel << std::endl;
	levels[currentLevel].loadBricks(bricks);
	for (auto &e : bricks) {
		e.setTexture(tx["brick"]);
	}
	balls.clear();
	balls.emplace_back(ball(App, tx["ball"], { 300, 300 }, { pad->x(), pad->y() + 10 }));
	balls.back().moving = false;	
	paused = false;	
	laserScale = 1;
	toReset = false;
	
}

int game::Run() {
	sf::Event event;
	ImGui::SFML::Init(App);
	auto io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("assets/fonts/Font Awesome 5 Free-Regular-400.otf", 16.f);
	io.Fonts->AddFontFromFileTTF("assets/fonts/Sarpanch-Regular.ttf", 16.f);
	ImGui::SFML::UpdateFontTexture();
	info.setString("Press Space to start");
	while (running) {		
		if (currentLevel >= levels.size() - 1) {
			return 2;
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
					paused = true;
					return (3);
				case sf::Keyboard::Space:
					//bonuses.emplace_back(bonus(App, tx["bonusSlower"], bonusType::slowerBall, { 0, 100 }, { 500.0f, 500.0f }));
					for(auto &e:balls) {
						e.moving = true;
					}
					info.setString("");
					break;
				case sf::Keyboard::LBracket:
					if (!balls.empty()) {
						balls.pop_back();
					}
					break;
				case sf::Keyboard::RBracket:
					balls.emplace_back(ball(App, tx["ball"], { 500, 500 }, { 400.0f, 500.0f }));
					break;
				case sf::Keyboard::End:
					pause();
					return 1;
					break;
				case sf::Keyboard::Delete:
					pause();
					return 2;
					break;
				case sf::Keyboard::P:
					paused ? resume() : pause();
					//pause();
					break;
				case sf::Keyboard::U:
					nextLevel();
					resume();
					break;
				case sf::Keyboard::R:
					restart();
					break;
				default:
					break;
				}
			}
		}
		if (!paused) {
			switch (currentLevel) {
			case 0:
				music["bg1_A"].setVolume(VOLUME);
				music["bg1_B"].setVolume(0);
				music["bg1_C"].setVolume(0);
				music["bg1_D"].setVolume(0);
				music["bg1_E"].setVolume(0);
				music["bg1_F"].setVolume(0);
				break;
			case 1:
				music["bg1_A"].setVolume(0);
				music["bg1_B"].setVolume(VOLUME);
				music["bg1_C"].setVolume(0);
				music["bg1_D"].setVolume(0);
				music["bg1_E"].setVolume(0);
				music["bg1_F"].setVolume(0);
				break;
			case 2:
				music["bg1_A"].setVolume(0);
				music["bg1_B"].setVolume(0);
				music["bg1_C"].setVolume(VOLUME);
				music["bg1_D"].setVolume(0);
				music["bg1_E"].setVolume(0);
				music["bg1_F"].setVolume(0);
				break;
			case 3:
				music["bg1_A"].setVolume(0);
				music["bg1_B"].setVolume(0);
				music["bg1_C"].setVolume(0);
				music["bg1_D"].setVolume(VOLUME);
				music["bg1_E"].setVolume(0);
				music["bg1_F"].setVolume(0);
				break;
			case 4:
				music["bg1_A"].setVolume(0);
				music["bg1_B"].setVolume(0);
				music["bg1_C"].setVolume(0);
				music["bg1_D"].setVolume(0);
				music["bg1_E"].setVolume(VOLUME);
				music["bg1_F"].setVolume(0);
				break;
			case 5:
				music["bg1_A"].setVolume(0);
				music["bg1_B"].setVolume(0);
				music["bg1_C"].setVolume(0);
				music["bg1_D"].setVolume(0);
				music["bg1_E"].setVolume(0);
				music["bg1_F"].setVolume(VOLUME);
				break;
			default:
				//audio.play("hit2");
				break;
			}

			sp_background.setColor(sf::Color(255, 255, 255, 255));
			//std::list<entity> allObjects(bricks.begin(), bricks.end());


			//quad.insert(*pad);



			//Clearing screen
			App.clear();
			//Drawing
			App.draw(sp_background);

			for (auto &e : balls) {
				e.move(elapsed_);
				if(e.moving == false) {
					e.move( pad->x(), pad->y() - (2*e.r()+  5) );
				}		
			}
			
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && cannon >= 10 && 
				cooldownClock.getElapsedTime() > sf::milliseconds(1000) && 
				!cannonFire) {
				infoCannon.setString("Cannon: ");
				infoCannon.setFillColor(sf::Color::White);
				laserScale = 1;
				cannon -= 10;
				cooldownClock.restart();
				cannonFire = true;
				laserPlayed = false;
			}
			if(cannonFire) {
				sf::RectangleShape temp;
				temp.setFillColor(sf::Color::Red);
				sf::Sprite laser;
				laser.setTexture(tx["laser"]);
				laser.setOrigin({ laser.getLocalBounds().width / 2, 0 });
			
				auto time = cooldownClock.getElapsedTime().asMilliseconds();
				if(time < 1000) {
					temp.setPosition(pad->x(), PLAYAREA_Y);
					temp.setSize({ 1, laser.getLocalBounds().height });
					App.draw(temp);
				}
				else if(time < 2000 && laserScale > 0) {
					if (!laserPlayed) {
						for (auto& e : bricks) {
							if (e.x1() < pad->x() + laser.getLocalBounds().width/2 && e.x2() > pad->x()-laser.getLocalBounds().width / 2) {
								e.toDelete = true;
							}
						}
						audio.play("laser");
						laserPlayed = true;
					}				
					laser.setPosition(pad->x(), PLAYAREA_Y);
					laser.setScale({ laserScale, 0.95f });	
					laser.setColor(sf::Color(255-laserScale*1, 100 - laserScale * 1, 0));
					laserScale -= 0.05;
					std::cout << laser.getScale().x << std::endl;
					App.draw(laser);							
				}
				else {					
					cannonFire = false;					
				}
			}
			pad->move();
			for (auto &e : balls) {
				if (e.checkCollision() == true) {
					if (!balls.empty()) {
						balls.erase(
							std::remove_if(balls.begin(), balls.end(),
								[&](ball &o) { return o.toDelete; }),
							balls.end());
					}
				}
				std::string temp;
				collision direction = checkCollision(e, pad);
				e.bounce(direction);
				//debug.setString( temp );			
			}
			for (auto &e : balls) {
				if (!balls.empty()) {
					App.draw(e.sprite());
				}
			}		


			for (auto &e : points) {
				e.move(elapsed_);
				if (e.y() > PLAYAREA_Y + PLAYAREA_HEIGHT) {
					e.move(-100, -100);
					e.setSpeed(0, 0);
				}
				collision direction = checkCollision(e, pad);
				if(direction != collision::none) {
					e.move(-100, -100);
					e.setSpeed(0, 0);
					score += 10;
					audio.play("point");					
					if (cannon <= 10) {
						infoCannon.setString(infoCannon.getString() + "|");
						cannon++;
					}else {
						infoCannon.setFillColor(sf::Color::Yellow);
					}
				}
				App.draw(e.sprite());
			}
			
	

			for (auto &e : bricks) {
				for (auto &f : balls) {
					collision direction = checkCollision(f, &e);
					f.bounce(direction);
					if(direction != collision::none) {
						App.draw(e.sprite());
						points[currentPoint].setSpeed(0, 500);
						points[currentPoint].sprite().rotate(randomInt(0,360));
						points[currentPoint].move(f.x(), f.y());
						currentPoint++;
						if (currentPoint >= 25) {
							currentPoint = 0;
						}
					}
					infoScore.setString("Score: " + std::to_string(score));					
				}
				for (auto &f : bullets) {
					collision direction = checkCollision(f, &e);
					if (direction != collision::none) {
						App.draw(e.sprite());
						f.move(-100, -100);
						f.setSpeed(0, 0);
					}
					infoScore.setString("Score: " + std::to_string(score));
					point.setPosition(f.sprite().getPosition());
				}
				App.draw(e.sprite());

			}
			if(bricks.size() <=5) {
				if (cannon <= 10) {
					infoCannon.setString(infoCannon.getString() + "|");
					cannon++;
				} else {
					infoCannon.setFillColor(sf::Color::Yellow);
				}
			}
			
			for(auto &e:bricks) {
				bricks.erase(
				std::remove_if(bricks.begin(), bricks.end(),
				[&](brick &o) { return o.toDelete; }),
				bricks.end());
			}
			
			for (auto &e : bonuses) {
				e.move(elapsed_);
				e.checkCollision();
				collision direction = checkCollision(e, pad);
				e.bounce(direction);
				App.draw(e.sprite());				
			}


			for (auto &e : bonuses) {
				bonuses.erase(
					std::remove_if(bonuses.begin(), bonuses.end(),
						[&](bonus &o) { return o.toDelete; }),
					bonuses.end());
			}


			if(balls.empty()) {
				info.setString("Game over");
				paused = true;
				//return -1;
			}
			if(bricks.empty()) {
				info.setString("Level completed");
				paused = true;
				nextLevel();
			}
			//info.setString("Game over");
			info.setOrigin(info.getLocalBounds().width / 2, info.getLocalBounds().height / 2);
			//==========================================================================
			//RENDER
			//==========================================================================
			//debug.setString(std::to_string((int)std::floor(1 / elapsed_.asSeconds())));
			//std::cout << elapsed_.asSeconds() << std::endl;
			App.setMouseCursorVisible(false);
			App.draw(point);
			App.draw(pad->sprite());
			App.draw(info);
			App.draw(infoScore);		
			App.draw(infoCannon);		
			App.display();
			restartClock();
		}
		else {
			info.setString("Paused");
		}
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}

sf::Time game::getElapsed() {
	return elapsed_;
}
void game::restartClock() {
	elapsed_ = clock_.restart();
}


//==========================================================================
//BONUS
//==========================================================================
void game::applyBonus(bonusType bonus) {
	switch (bonus) {
	case bonusType::bonusBall:
		balls.emplace_back(ball(App, tx["ball"], { 200, 200 }, { pad->sprite().getPosition().x, pad->sprite().getPosition().y }));
		break;
	case bonusType::powerBall:
		for (auto& e : balls) {
			e.isPower = true;
			e.setTexture(tx["powerBall"]);
		}
		break;
	case bonusType::fasterBall:
		for(auto& e:balls) {
			e.setSpeed(e.getSpeed().x * 1.5f, e.getSpeed().y * 1.5f);
		}
		break;
	case bonusType::slowerBall:
		for (auto& e : balls) {
			e.setSpeed(e.getSpeed().x * 0.5f, e.getSpeed().y * 0.5f);
		}
		break;
	default:

		break;
	}
	audio.play("bonus");
}

//==========================================================================
//COLLISION
//==========================================================================
collision game::checkCollision(ball& ball, entity* object) {
	auto ballX = ball.x();
	auto ballY = ball.y();


	float objectX = clamp(ballX,
		object->x() - object->size().width / 2,
		object->x() + object->size().width / 2);

	float objectY = clamp(ballY,
		object->y() - object->size().height / 2,
		object->y() + object->size().height / 2);

	/*sf::CircleShape point(4);
	point.setOrigin(point.getRadius(), point.getRadius());
	point.setFillColor(sf::Color::Red);
	point.setPosition(objectX, objectY);
	App.draw(point);*/
	//if (sqrt(pow(objectX - ballX, 2) + pow(objectY - ballY, 2)) < ball.getLocalBounds().height / 2) {
	if (pow(objectX - ballX, 2) + pow(objectY - ballY, 2) < pow(ball.r(), 2)) {		

		//float offsetX = 0;// objectX - ballX + ball.r();
		//float offsetY = ballY+ ball.r() - objectY;
		//std::cout << ballX << ", " << ballY<< " - " << offsetX << ", " << offsetY << std::endl;
		//ball.offset(offsetX, offsetY);

		//TODO: Calculate collision offset/distance

		if (object->type() == type::brick) {
			auto tempX = object->x();
			auto tempY = object->y();
			if (object->destroy()) {
				score += object->points();
				switch (object->bonusInside()) {
				case bonusType::bonusBall:
					bonuses.emplace_back(bonus(App, tx["bonusBall"], object->bonusInside(), { 0, 200 }, { tempX,tempY }));
					break;
				case bonusType::fasterBall:
					bonuses.emplace_back(bonus(App, tx["bonusFaster"], object->bonusInside(), { 0, 200 }, { tempX, tempY }));
					break;
				case bonusType::powerBall:
					bonuses.emplace_back(bonus(App, tx["bonusPower"], object->bonusInside(), { 0, 200 }, { tempX, tempY }));
					break;
				case bonusType::slowerBall:
					bonuses.emplace_back(bonus(App, tx["bonusSlower"], object->bonusInside(), { 0, 200 }, { tempX,tempY }));
					break;
				case bonusType::none:
					break;
				default:
					break;
				}
			}
		}
		if (ball.isPower==true && object->type() != type::paddle) {
			return collision::power;
		}
		//TODO: Get bonus type from object
		if (ball.type() == type::bonus) {
			applyBonus(ball.bonusInside());
			object->destroy();
		}

		float rad = atan2(ballX - objectX, ballY - objectY);
		if (rad > -3 * M_PI / 4 && rad > 3 * M_PI / 4) {
			ball.move(objectX, objectY - ball.r());
			return collision::up;
		}
		if ((-M_PI / 4) < rad &&  rad < (M_PI / 4)) {
			if (object->type() == type::paddle) {
				ball.move(objectX, objectY - ball.r());
				return collision::up; // ball shoudln't be able to bounce from the bottom if lives are on
			}
			ball.move(objectX, objectY + ball.r());
			return collision::down;
		}
		if (-M_PI / 4 > rad  && rad > -3 * M_PI / 4) {
			ball.move(objectX - ball.r(), objectY);
			return collision::left;
		}
		if (M_PI / 4 < rad && rad < 3 * M_PI / 4) {
			ball.move(objectX + ball.r(), objectY);
			return collision::right;
		}

	}
	return collision::none;
}


void game::restart() {
	running = true;
	paused = false;

	infoCannon.setString("Cannon: ");

	currentLevel = 0;
	//toReset = true;
	//nextLevel();
	levels.clear();
	balls.clear();
	bonuses.clear();
	for(auto& e:points) {
		e.move(-100, -100);
		e.setSpeed(0, 0);
	}
	level temp;
	temp.readFromFile("assets/levels/official/c0.txt"); levels.push_back(temp);
	temp.readFromFile("assets/levels/official/c1.txt"); levels.push_back(temp);
	temp.readFromFile("assets/levels/official/c2.txt"); levels.push_back(temp);
	temp.readFromFile("assets/levels/official/c3.txt"); levels.push_back(temp);
	temp.readFromFile("assets/levels/official/c4.txt"); levels.push_back(temp);
	temp.readFromFile("assets/levels/official/c5.txt"); levels.push_back(temp);
	temp.readFromFile("assets/levels/official/c6.txt"); levels.push_back(temp);
	levels[currentLevel].loadBricks(bricks);
	for (auto &e : bricks) {
		e.setTexture(tx["brick"]);
	}

	balls.emplace_back(ball(App, tx["ball"], { 300, 300 }, { pad->x(), pad->y() + 10 }));
	balls.back().moving = false;

	score = 0;

	currentPoint = 0;
	cannon = 0;
	cannonFire = true;
	bool laserPlayed = false;
	
}



void game::restart(std::string levelName) {
	running = true;
	paused = false;

	infoCannon.setString("Cannon: ");

	currentLevel = 0;
	//toReset = true;
	//nextLevel();
	levels.clear();
	balls.clear();
	bonuses.clear();
	for (auto& e : points) {
		e.move(-100, -100);
		e.setSpeed(0, 0);
	}
	level temp;
	temp.readFromFile("assets/levels/"+ levelName +".txt"); levels.push_back(temp);
	temp.readFromFile("assets/levels/official/c6.txt"); levels.push_back(temp);
	levels[currentLevel].loadBricks(bricks);
	for (auto &e : bricks) {
		e.setTexture(tx["brick"]);
	}

	balls.emplace_back(ball(App, tx["ball"], { 300, 300 }, { pad->x(), pad->y() + 10 }));
	balls.back().moving = false;

	score = 0;

	currentPoint = 0;
	cannon = 0;
	cannonFire = true;
	bool laserPlayed = false;

}

