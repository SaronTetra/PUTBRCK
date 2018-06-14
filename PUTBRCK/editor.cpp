#include "editor.h"
#include <iostream>
#include <imgui-SFML.h>
#include <imgui.h>

bool editor::mouseOverlap(entity & e) {
	float mousePosX = sf::Mouse::getPosition(App).x;
	float mousePosY = sf::Mouse::getPosition(App).y;
	float width2 = e.size().width / 2;
	float height2 = e.size().height / 2;
	float x1 = e.x() - width2;
	float x2 = e.x() + width2;
	float y1 = e.y() - height2;
	float y2 = e.y() + height2;
	return (mousePosX > x1 && mousePosX < x2 && mousePosY > y1 && mousePosY < y2);
}

editor::editor(sf::RenderWindow & App, game* thisGame) : App(App), thisGame(thisGame) {
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
}

sf::Event event;
int editor::Run() {
	ImGui::SFML::Init(App);
	auto io = ImGui::GetIO();

	//==========================================================================
	//VARIABLES
	//==========================================================================
	static int selected = -1;
	ImVec4 color(1.f, 1.f, 1.f,1.f);
	bool brushMode = false;
	int newHealth = 1;
	int newScore = 1;

	int pickedBonus;
	bonusType bonusToApply = bonusType::none;


	sf::RectangleShape outline;
	outline.setOutlineColor(sf::Color::Yellow);
	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineThickness(4);	


	sf::RectangleShape outlineSelected;
	outlineSelected.setOutlineColor(sf::Color::Green);
	outlineSelected.setFillColor(sf::Color::Transparent);
	outlineSelected.setOutlineThickness(4);

	//==========================================================================
	//MAIN LOOP
	//==========================================================================
	while (running) {
		float indicatorX = std::fmod(sf::Mouse::getPosition(App).x, indicator.getSize().x);
		float indicatorY = std::fmod(sf::Mouse::getPosition(App).y, indicator.getSize().y);
		float newBrickX = std::fmod(PLAYAREA_X, indicator.getSize().x) - indicator.getSize().x / 2 + 10 + sf::Mouse::getPosition(App).x - indicatorX;
		float newBrickY = std::fmod(PLAYAREA_Y, indicator.getSize().y) - indicator.getSize().y / 2 + 10 + sf::Mouse::getPosition(App).y - indicatorY;
		bool taken = false;
		focused = false;
		if (newBrickX < PLAYAREA_X) { taken = true; focused = true;	}
		if (newBrickY < PLAYAREA_Y) { taken = true; focused = true; }
		if (newBrickX > PLAYAREA_X + PLAYAREA_WIDTH) { taken = true; focused = true; }
		if (newBrickY > PLAYAREA_Y + PLAYAREA_HEIGHT - indicator.getSize().y * 4) { taken = true; focused = true; }
		indicator.setOrigin(indicator.getSize().x / 2, indicator.getSize().y / 2);
		indicator.setPosition({ newBrickX , newBrickY });
		indicator.setFillColor(sf::Color::Transparent);

		for (auto &e : bricks) {
			if (newBrickX == e.x() && newBrickY == e.y()) {
				taken = true;
			}
		}
		
		//==========================================================================
		//KEYBOARD INPUT
		//==========================================================================
		//Verifying events
		while (App.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);
			// Window closed
			if (event.type == sf::Event::Closed) {
				return (-1);
			}
			//Key pressed
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					return (3);
				case sf::Keyboard::Home:
					thisGame->resume();
					return 0;
				default:
					break;
				}
			}
		}


		//==========================================================================
		//GUI
		//==========================================================================
		ImGui::SFML::Update(App, elapsed_);
		//ImGui::ShowDemoWindow();
		ImGui::SetNextWindowPos({ PLAYAREA_X + PLAYAREA_WIDTH, 0 });
		ImGui::SetNextWindowSize({ App.getSize().x - PLAYAREA_X - PLAYAREA_WIDTH + 0.f, App.getSize().y + 0.f });
		ImGui::Begin("editor");
		ImVec2 elemSize = { ImGui::GetWindowSize().x, 50 };
		ImVec2 elemSizeHalf = { ImGui::GetWindowSize().x/2, 50 };
		std::string temp = InputBuf;		

		ImGui::Text("PUTBRCK EDITOR");
		ImGui::Text("Level name:");
		ImGui::CaptureKeyboardFromApp(true);
		ImGui::InputText("", InputBuf, 32);
		if (temp == "") {
			ImGui::TextColored({ 255,255,0,255 }, "Name can not be empty");
		}
		if (ImGui::Button("Save", elemSize)) {
			audio.play("click1");
			level_.saveBricks(bricks);
			if (temp != "") {
				level_.writeToFile("assets/levels/" + temp + ".txt");
			}
		}
		if (ImGui::Button("Load", elemSize)) {
			audio.play("click2");
			level_.readFromFile("assets/levels/" + temp + ".txt");
			level_.loadBricks(bricks);
			for (auto &e : bricks) {
				e.setTexture(tx["brick"]);
			}
		}

		//==========================================================================
		//TOOLS BUTTONS
		//==========================================================================
		if (ImGui::Selectable("Select", selected == 0)) { selected = 0; }
		if (ImGui::Selectable("Add/remove bricks", selected == 1)) { selected = 1; }
		if (ImGui::Selectable("Paint brush", selected == 2)) { selected = 2; }
		if (ImGui::Selectable("Paint bucket", selected == 3)) { selected = 3; }
		if (ImGui::Selectable("Properties painter", selected == 4)) { selected = 4; }

		ImGui::Columns(2);
		if (ImGui::Button("Clear", { (elemSize.x) / 2, elemSize.y })) {
			audio.play("click1");
			ImGui::OpenPopup("Clear?");
		}
		if (ImGui::BeginPopupModal("Clear?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			focused = true;
			ImGui::Text("Are you sure you want to clear all bricks?");
			ImGui::Separator();

			//static int dummy_i = 0;
			//ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");
			if (ImGui::Button("OK", ImVec2(120, 0))) {
				audio.play("click1");
				bricks.clear();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				audio.play("click1");
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::NextColumn();
		if(ImGui::Button("Fill", { (elemSize.x) / 2, elemSize.y })) {
			audio.play("click1");
			ImGui::OpenPopup("Fill?");
		}
		if (ImGui::BeginPopupModal("Fill?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			focused = true;
			ImGui::Text("Are you sure you want to fill whole screen?");
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0))) {
				audio.play("click1");
				bricks.clear();
				for (float i = 0; i < 18; ++i) {
					for (float j = 0; j < 15; ++j) {						
						bricks.emplace_back(brick(tx["brick"], 1, 100, { i * tx["brick"].getSize().x + PLAYAREA_X + tx["brick"].getSize().x / 2 + 10, j * tx["brick"].getSize().y + PLAYAREA_Y + tx["brick"].getSize().y / 2 + 10 }));
						bricks.back().setHealth(newHealth);
						bricks.back().sprite().setColor(color);
					}
				}
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				audio.play("click1");
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::Columns(1);

		//==========================================================================
		//TOOLS IMPLEMENTATION
		//==========================================================================
		if (!focused) {
			float mousePosX = sf::Mouse::getPosition(App).x;
			float mousePosY = sf::Mouse::getPosition(App).y;
			switch (selected) {
			case 0: //Select tool
				for (auto &e : bricks) {
					float width2 = e.size().width / 2;
					float height2 = e.size().height / 2;
					if (mouseOverlap(e)) {
						outline.setSize({ e.size().width, e.size().height });
						outline.setOrigin(width2, height2);
						outline.setPosition(e.x(), e.y());
						outline.setOutlineColor(sf::Color::Yellow);
						//e.highlight(sf::Color::Blue, true);
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
							selectedBrick = &e;
							newHealth = e.getHealth();
							newScore = e.points() / 100;
						}
					} else {
						//outline.setOutlineColor(sf::Color::Transparent);
						//e.highlight(sf::Color::Blue, false);
					}
					if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
						selectedBrick = nullptr;
					}
				}
				break;
			case 1: //add/remove tool
				if (taken) {
					indicator.setFillColor(sf::Color(255, 255, 255, 100));
				} else {
					indicator.setFillColor(sf::Color(100, 255, 100, 200));
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					for (auto &e : bricks) {
						if (newBrickX == e.x() && newBrickY == e.y()) {
							taken = true;
						}
					}
					if (!taken) {
						bricks.emplace_back(brick(tx["brick"], 1, 100, { newBrickX, newBrickY }));
						bricks.back().sprite().setColor(color);
						std::cout << "New brick added at: " << newBrickX << ", " << newBrickY << std::endl;
					}
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					for (auto &e : bricks) {							
						if (mouseOverlap(e)) {
						//if (newBrickX == e.x() && newBrickY == e.y()) {
							taken = true;
							bricks.erase(
								std::remove_if(bricks.begin(), bricks.end(),
									[&](brick &o) { return e.x() == o.x() && e.y() == o.y(); }),
								bricks.end());
							std::cout << "Brick deleted at: " << newBrickX << ", " << newBrickY << std::endl;

						}
					}
				}
				break;
			case 2: //paint brush
				for (auto &e : bricks) {
					if (mouseOverlap(e)) {
						float width2 = e.size().width / 2;
						float height2 = e.size().height / 2;
						outline.setSize({ e.size().width, e.size().height });
						outline.setOrigin(width2, height2);
						outline.setPosition(e.x(), e.y());
						outline.setOutlineColor(sf::Color::Yellow);
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {							
							e.sprite().setColor(color);							
						}
							
					}
				}
				break;
			case 3: //Paint bucket
				for (auto &e : bricks) {
					if (mouseOverlap(e)) {
						float width2 = e.size().width / 2;
						float height2 = e.size().height / 2;
						outline.setSize({ e.size().width, e.size().height });
						outline.setOrigin(width2, height2);
						outline.setPosition(e.x(), e.y());
						outline.setOutlineColor(sf::Color::Yellow);
						auto temp = e;
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
							//[&](brick &o) { return e.sprite().getColor() == o.sprite().getColor(); };
							for (auto &f : bricks) {
								if (temp.sprite().getColor() == f.sprite().getColor()) {
									f.sprite().setColor(color);
								}
							}
						}

					}
				}
				break;
			case 4: //properties brush
				for (auto &e : bricks) {
					if (mouseOverlap(e)) {
						float width2 = e.size().width / 2;
						float height2 = e.size().height / 2;
						outline.setSize({ e.size().width, e.size().height });
						outline.setOrigin(width2, height2);
						outline.setPosition(e.x(), e.y());
						outline.setOutlineColor(sf::Color::Yellow);
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
							e.setHealth(newHealth);
							e.setPoints(newScore * 100);
							e.setBonus(bonusToApply);
						}

					}
				}
			default:
				break;
			}
		}
			
		//==========================================================================
		//COLOR PICKER
		//==========================================================================
		ImGui::ColorPicker3("MyColor##4", (float*)&color, ImGuiColorEditFlags_PickerHueWheel);// , &ref_color_v.x);



		//==========================================================================
		//SELECTED BRICK
		//==========================================================================
		if (selectedBrick) {
			static ImVec4 ref_color_v = selectedBrick->sprite().getColor();
			ImGui::Columns(2);
			if (ImGui::Button("Set color", elemSizeHalf)) {
				audio.play("click1");
				selectedBrick->sprite().setColor(color);
			}
			ImGui::NextColumn();
			if (ImGui::Button("Remove", elemSizeHalf)) {
				audio.play("click1");
				bricks.erase(
					std::remove_if(bricks.begin(), bricks.end(),
						[&](brick &o) { return selectedBrick->x() == o.x() && selectedBrick->y() == o.y(); }),
					bricks.end());
				selectedBrick = nullptr;
			}
			ImGui::Columns(1);
			ImGui::SliderInt("Health slider", &newHealth, 1, 10);		
			ImGui::SliderInt("Score slider", &newScore, 0, 10);		
			ImGui::Combo("Bonus Type", &pickedBonus, "None\0Bonus Ball\0Faster Ball\0Slower Ball\0Power Ball\0");
			switch (pickedBonus) {
			case 0:
				bonusToApply = bonusType::none;
				break;;
			case 1:
				bonusToApply = bonusType::bonusBall;
				break;
			case 2:
				bonusToApply = bonusType::fasterBall;
				break;
			case 3:
				bonusToApply = bonusType::slowerBall;
				break;
			case 4:
				bonusToApply = bonusType::powerBall;
				break;
			default:
				bonusToApply = bonusType::none;
				break;
			}
			if (ImGui::Button("Apply", elemSizeHalf)) {
				audio.play("click1");
				selectedBrick->setHealth(newHealth);
				selectedBrick->setPoints(newScore * 100);
				selectedBrick->setBonus(bonusToApply);
			}
			
		}

		if(ImGui::Button("Play", elemSize)) {		
			audio.play("click1");
			ImGui::OpenPopup("Play?");
		}
		if (ImGui::BeginPopupModal("Play?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			focused = true;
			ImGui::Text("Ready to play?");
			ImGui::Separator();

			if (ImGui::Button("Yes", ImVec2(120, 0))) {
				audio.play("click2");
				thisGame->restart(InputBuf);				
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(120, 0))) {
				audio.play("click1");
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::GetIO().FontGlobalScale = 2.5f;
		ImGui::End();
		sp_background.setColor(sf::Color(255, 255, 255, 255));


		//==========================================================================
		//RENDER
		//==========================================================================
		//Clearing screen
		App.clear();
		//Drawing
		App.draw(sp_background);

		for (auto &e : bricks) {
			App.draw(e.sprite());
			sf::Text healthText;
			sf::Text scoreText;
			sf::Text bonusText;
			healthText.setFont(font);
			healthText.setCharacterSize(20);
			healthText.setPosition(e.x1(), e.y1());
			healthText.setFillColor(sf::Color::Green);
			healthText.setOutlineColor(sf::Color::Black);
			healthText.setOutlineThickness(1);
			healthText.setString(std::to_string(e.getHealth()));

			scoreText.setFont(font);
			scoreText.setCharacterSize(16);
			scoreText.setPosition(e.x1(), e.y());
			scoreText.setFillColor(sf::Color::Yellow);
			scoreText.setOutlineColor(sf::Color::Black);
			scoreText.setOutlineThickness(1);
			scoreText.setString(std::to_string(e.points()));

			bonusText.setFont(font);
			bonusText.setCharacterSize(16);
			bonusText.setPosition(e.x2() - 25, e.y());
			bonusText.setFillColor(sf::Color(255,255,0));
			bonusText.setOutlineColor(sf::Color::Black);
			bonusText.setOutlineThickness(1);
			std::string temp;
			switch(e.bonusInside()) {
			case bonusType::bonusBall:
				temp = "BB";
				break;
			case bonusType::fasterBall:
				temp = "FB";
				break;		
			case bonusType::powerBall:
				temp = "PB";
				break;
			case bonusType::slowerBall:
				temp = "SB";
				break;
			case bonusType::none:
				temp = "";
				break;
			default:
				temp = "";
				break;
			}
			bonusText.setString(temp);
			App.draw(healthText);
			App.draw(scoreText);
			App.draw(bonusText);
		}

		if (selectedBrick != nullptr) {
			outlineSelected.setSize({ selectedBrick->size().width, selectedBrick->size().height });
			outlineSelected.setOrigin(selectedBrick->size().width / 2, selectedBrick->size().height / 2);
			outlineSelected.setPosition(selectedBrick->x(), selectedBrick->y());

			App.draw(outlineSelected);
		}

		App.draw(outline);
		App.draw(indicator);
		App.draw(debug);
		App.draw(infoScore);
		ImGui::SFML::Render(App);
		App.display();
		//App.setMouseCursorVisible(true);
		//restartClock();
		
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}