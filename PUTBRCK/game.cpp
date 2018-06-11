#include "game.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>

game::game(sf::RenderWindow &App) : App(App) {
	sf::Event Event;
	running = true;
	paused = false;
	//-------------------------------------------------
	//---------------------TEXTURES--------------------
	//-------------------------------------------------
	/*	sf::Texture texture;
	texture.loadFromFile("assets/textures/background.png"); tx["background"] = texture;
	texture.loadFromFile("assets/textures/sb.png"); tx["ball"] = texture;
	texture.loadFromFile("assets/textures/paddle_rect.png"); tx["paddle"] = texture;
	texture.loadFromFile("assets/textures/brick_small.png"); tx["brick"] = texture;
	texture.loadFromFile("assets/textures/bonus.png"); tx["bonus"] = texture;*/


	tx.loadTexture("assets/textures/background.png", "background");
	tx.loadTexture("assets/textures/ball.png", "ball");
	tx.loadTexture("assets/textures/paddle.png", "paddle");
	tx.loadTexture("assets/textures/brick.png", "brick");
	tx.loadTexture("assets/textures/bonus.png", "bonus");

	music["bg1_A"].openFromFile("assets/music/PM_ATG_2_100BPM_A.ogg");
	music["bg1_B"].openFromFile("assets/music/PM_ATG_2_100BPM_B.ogg");
	music["bg1_C"].openFromFile("assets/music/PM_ATG_2_100BPM_C.ogg");
	music["bg1_D"].openFromFile("assets/music/PM_ATG_2_100BPM_D.ogg");
	music["bg1_E"].openFromFile("assets/music/PM_ATG_2_100BPM_E.ogg");
	music["bg1_F"].openFromFile("assets/music/PM_ATG_2_100BPM_F.ogg");

	music["bg1_A"].play();
	music["bg1_B"].play();
	music["bg1_C"].play();
	music["bg1_D"].play();
	music["bg1_E"].play();
	music["bg1_F"].play();

	music["bg1_A"].setLoop(true);
	music["bg1_B"].setLoop(true);
	music["bg1_C"].setLoop(true);
	music["bg1_D"].setLoop(true);
	music["bg1_E"].setLoop(true);
	music["bg1_F"].setLoop(true);

	music["bg1_A"].setVolume(0);
	music["bg1_B"].setVolume(0);
	music["bg1_C"].setVolume(0);
	music["bg1_D"].setVolume(0);
	music["bg1_E"].setVolume(0);
	music["bg1_F"].setVolume(0);

	audio.loadAudio("assets/sfx/PM_SA_IMPACT_33.ogg", "hit1");
	audio.loadAudio("assets/sfx/PM_SA_CINEMATIC_IMPACT_21.ogg", "hit2");
	audio.loadAudio("assets/sfx/PM_SA_CINEMATIC_IMPACT_32.ogg", "bonus");
	audio.loadAudio("assets/sfx/PM_SA_CINEMATIC_IMPACT_27.ogg", "ballLost");
	audio.play("hit1");
	audio.play("hit2");



	debug.setFont(font);
	debug.setCharacterSize(96);
	debug.setPosition({ 100, 100 });

	infoScore.setFont(font);
	infoScore.setCharacterSize(48);
	infoScore.setPosition({ 10, 10 });


	point.setRadius(1);
	point.setFillColor(sf::Color::Red);

	sp_background.setTexture(tx["background"]);
	sp_background.setColor(sf::Color(255, 255, 255, 255));
	if (!font.loadFromFile("assets/fonts/Sarpanch-Regular.ttf")) {
		std::cerr << "Error loading Sarpanch-Regular.ttf" << std::endl;
		throw - 1;
	}
	for (int i = 0; i < 1; i++) {
		//balls.emplace_back(ball(App, tx["ball"], { 500, 500 }, { 100.0f+i*400.0f, 500.0f }));
	}
	pad = new paddle(App, tx["paddle"]);

	for (float i = 0; i < 18; ++i) {
		for (float j = 0; j < 10; ++j) {
			bricks.emplace_back(brick(tx["brick"], 1, 100, { i * tx["brick"].getSize().x + PLAYAREA_X + tx["brick"].getSize().x / 2 + 10, j * tx["brick"].getSize().y + PLAYAREA_Y + tx["brick"].getSize().y / 2 + 10 }));
		}
	}

		/*level_.readFromFile("assets/levels/test.txt");
		level_.loadBricks(bricks);
		for (auto &e : bricks) {
			e.setTexture(tx["brick"]);
		}*/


	//quad.clear();
	//quad.fill();

	//for (auto& e : bricks) {
	//	quad.insert(&e);
	//}

	//for (auto& e : balls) {
	//	quad.insert(&e);
	//}

	//bonuses.emplace_back(bonus(App, tx["bonus"], { 0, 100 }, { 500.0f, 500.0f }));


}

int game::Run() {
	sf::Event event;
	ImGui::SFML::Init(App);
	auto io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("assets/fonts/Font Awesome 5 Free-Regular-400.otf", 16.f);
	io.Fonts->AddFontFromFileTTF("assets/fonts/Sarpanch-Regular.ttf", 16.f);
	ImGui::SFML::UpdateFontTexture();
	while (running) {
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
				case sf::Keyboard::LBracket:
					if (!balls.empty()) {
						balls.pop_back();
					}
					break;
				case sf::Keyboard::RBracket:
					balls.emplace_back(ball(App, tx["ball"], { 500, 500 }, { 400.0f, 500.0f }));
					break;
				case sf::Keyboard::Q:
					audio.play("hit2");
					break;

				case sf::Keyboard::I:
					ImGui::ShowDemoWindow();
					break;			


				case sf::Keyboard::Num1:
					music["bg1_A"].setVolume(100);
					music["bg1_B"].setVolume(0);
					music["bg1_C"].setVolume(0);
					music["bg1_D"].setVolume(0);
					music["bg1_E"].setVolume(0);
					music["bg1_F"].setVolume(0);
					break;
				case sf::Keyboard::Num2:
					music["bg1_A"].setVolume(0);
					music["bg1_B"].setVolume(100);
					music["bg1_C"].setVolume(0);
					music["bg1_D"].setVolume(0);
					music["bg1_E"].setVolume(0);
					music["bg1_F"].setVolume(0);
					break;
				case sf::Keyboard::Num3:
					music["bg1_A"].setVolume(0);
					music["bg1_B"].setVolume(0);
					music["bg1_C"].setVolume(100);
					music["bg1_D"].setVolume(0);
					music["bg1_E"].setVolume(0);
					music["bg1_F"].setVolume(0);
					break;
				case sf::Keyboard::Num4:
					music["bg1_A"].setVolume(0);
					music["bg1_B"].setVolume(0);
					music["bg1_C"].setVolume(0);
					music["bg1_D"].setVolume(100);
					music["bg1_E"].setVolume(0);
					music["bg1_F"].setVolume(0);
					break;
				case sf::Keyboard::Num5:
					music["bg1_A"].setVolume(0);
					music["bg1_B"].setVolume(0);
					music["bg1_C"].setVolume(0);
					music["bg1_D"].setVolume(0);
					music["bg1_E"].setVolume(100);
					music["bg1_F"].setVolume(0);
					break;
				case sf::Keyboard::Num6:
					music["bg1_A"].setVolume(0);
					music["bg1_B"].setVolume(0);
					music["bg1_C"].setVolume(0);
					music["bg1_D"].setVolume(0);
					music["bg1_E"].setVolume(0);
					music["bg1_F"].setVolume(100);
					break;
				case sf::Keyboard::End:
					pause();
					return 1;
					break;
				case sf::Keyboard::P:
					paused ? resume() : pause();
					//pause();
					break;
				case sf::Keyboard::Return:
					pause();
					sf::sleep(sf::milliseconds(16));
					resume();
				default:
					break;
				}
			}
		}
		if (!paused) {
			switch (score) {
			case 100:
				music["bg1_A"].setVolume(100);
				music["bg1_B"].setVolume(0);
				music["bg1_C"].setVolume(0);
				music["bg1_D"].setVolume(0);
				music["bg1_E"].setVolume(0);
				music["bg1_F"].setVolume(0);
				break;
			case 1000:
				music["bg1_A"].setVolume(0);
				music["bg1_B"].setVolume(100);
				music["bg1_C"].setVolume(0);
				music["bg1_D"].setVolume(0);
				music["bg1_E"].setVolume(0);
				music["bg1_F"].setVolume(0);
				break;
			case 2000:
				music["bg1_A"].setVolume(0);
				music["bg1_B"].setVolume(0);
				music["bg1_C"].setVolume(100);
				music["bg1_D"].setVolume(0);
				music["bg1_E"].setVolume(0);
				music["bg1_F"].setVolume(0);
				break;
			case 3000:
				music["bg1_A"].setVolume(0);
				music["bg1_B"].setVolume(0);
				music["bg1_C"].setVolume(0);
				music["bg1_D"].setVolume(100);
				music["bg1_E"].setVolume(0);
				music["bg1_F"].setVolume(0);
				break;
			case 4000:
				music["bg1_A"].setVolume(0);
				music["bg1_B"].setVolume(0);
				music["bg1_C"].setVolume(0);
				music["bg1_D"].setVolume(0);
				music["bg1_E"].setVolume(100);
				music["bg1_F"].setVolume(0);
				break;
			case 5000:
				music["bg1_A"].setVolume(0);
				music["bg1_B"].setVolume(0);
				music["bg1_C"].setVolume(0);
				music["bg1_D"].setVolume(0);
				music["bg1_E"].setVolume(0);
				music["bg1_F"].setVolume(100);
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

			//sf::CircleShape point(6);
			//point.setFillColor(sf::Color::Red);
			//point.setPosition(pad->sprite().getPosition());


			for (auto &e : balls) {
				e.move(elapsed_);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					e.changeSpeed(0, 10);
					//audio.play(0);
					std::cout << e.getSpeed().x << ", " << e.getSpeed().y << std::endl;
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					e.changeSpeed(0, -10);
					std::cout << e.getSpeed().x << std::endl;
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
			/*quad.draw(50);
			quad.clear();

			for (auto& e : bricks) {
			quad.insert(&e);
			}

			for (auto& e : balls) {
			//quad.insert(&e);
			}

			for (auto &e : balls) {
			returnObjects.clear();
			quad.retrive(returnObjects, e);
			for (auto &f : returnObjects) {
			collision direction = checkCollision(e, f);
			e.bounce(direction);
			infoScore.setString("Score: " + std::to_string(score));
			point.setPosition(e.sprite().getPosition());
			App.draw(f->sprite());
			}
			App.draw(e.sprite());

			}
			for (auto &e : bricks) {
				App.draw(e.sprite());
			}

			*/

			for (auto &e : bricks) {
				for (auto &f : balls) {
				collision direction = checkCollision(f, &e);
				f.bounce(direction);
				if(direction != collision::none) {
				App.draw(e.sprite());
				}
				infoScore.setString("Score: " + std::to_string(score));
				point.setPosition(f.sprite().getPosition());
				}
				App.draw(e.sprite());
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




			debug.setString(std::to_string((int)std::floor(1 / elapsed_.asSeconds())));
			//std::cout << elapsed_.asSeconds() << std::endl;
			App.draw(point);
			App.draw(pad->sprite());
			App.draw(debug);
			App.draw(infoScore);
			App.draw(point);			
			App.display();

			restartClock();
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

void game::applyBonus(bonusType bonus) {
	switch (bonus) {
	case bonusType::bonusBall:
		balls.emplace_back(ball(App, tx["ball"], { 200, 200 }, { pad->sprite().getPosition().x, pad->sprite().getPosition().y }));
		break;
	case bonusType::biggerBall:

		break;
	case bonusType::smallerBall:

		break;
	case bonusType::fasterBall:

		break;
	case bonusType::slowerBall:

		break;
	default:

		break;
	}
	audio.play("bonus");
}

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
		float rad = atan2(ballX - objectX, ballY - objectY);


		//float offsetX = 0;// objectX - ballX + ball.r();
		//float offsetY = ballY+ ball.r() - objectY;
		//std::cout << ballX << ", " << ballY<< " - " << offsetX << ", " << offsetY << std::endl;
		//ball.offset(offsetX, offsetY);

		//TODO: Calculate collision offset/distance

		if (object->type() == type::brick) {
			if (object->destroy()) {
				score += object->points();
			}
		}

		//TODO: Get bonus type from object
		if (ball.type() == type::bonus) {
			object->destroy();
			applyBonus(bonusType::bonusBall);
		}

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
};
