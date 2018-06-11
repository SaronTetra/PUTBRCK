#include "editor.h"
#include <iostream>
#include <imgui-SFML.h>
#include <imgui.h>

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
}

sf::Event event;
int editor::Run() {
	ImGui::SFML::Init(App);
	auto io = ImGui::GetIO();

	static int selected = -1;
	ImVec4 color(1.f, 1.f, 1.f,1.f);
	bool brushMode = false;

	sf::RectangleShape outline;
	outline.setOutlineColor(sf::Color::Yellow);
	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineThickness(4);	


	sf::RectangleShape outlineSelected;
	outlineSelected.setOutlineColor(sf::Color::Green);
	outlineSelected.setFillColor(sf::Color::Transparent);
	outlineSelected.setOutlineThickness(4);
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
					return (-1);
				case sf::Keyboard::Home:
					thisGame->resume();
					return 0;
				default:
					break;
				}
			}
		}

			ImGui::SFML::Update(App, elapsed_);
			ImGui::ShowDemoWindow();
			ImGui::SetNextWindowPos({ PLAYAREA_X + PLAYAREA_WIDTH, 0 });
			ImGui::SetNextWindowSize({ App.getSize().x - PLAYAREA_X - PLAYAREA_WIDTH + 0.f, App.getSize().y + 0.f });
			ImGui::Begin("editor");
			ImVec2 elemSize = { ImGui::GetWindowSize().x, 50 };
			std::string temp = InputBuf;

			ImGui::Text("PUTBRCK EDITOR");
			ImGui::Text("Level name:");
			ImGui::CaptureKeyboardFromApp(true);
			ImGui::InputText("", InputBuf, 32);
			if (temp == "") {
				ImGui::TextColored({ 255,255,0,255 }, "Name can not be empty");
			}
			if (ImGui::Button("Save", elemSize)) {
				level_.saveBricks(bricks);
				if (temp != "") {
					level_.writeToFile("assets/levels/" + temp + ".txt");
				}
			}
			if (ImGui::Button("Load", elemSize)) {
				level_.readFromFile("assets/levels/" + temp + ".txt");
				level_.loadBricks(bricks);
				for (auto &e : bricks) {
					e.setTexture(tx["brick"]);
				}
			}


			if (ImGui::Selectable("Select", selected == 0)) { selected = 0; }
			if (ImGui::Selectable("Add/remove bricks", selected == 1)) { selected = 1; }
			if (ImGui::Selectable("Tool 2", selected == 2)) { selected = 2; }
			if (ImGui::Selectable("Tool 3", selected == 3)) { selected = 3; }

			ImGui::Columns(2);
			if (ImGui::Button("Clear", { (elemSize.x) / 2, elemSize.y })) {
				ImGui::OpenPopup("Clear?");
			}
			if (ImGui::BeginPopupModal("Clear?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				focused = true;
				ImGui::Text("Are you sure you want to clear all bricks?");
				ImGui::Separator();

				//static int dummy_i = 0;
				//ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");
				if (ImGui::Button("OK", ImVec2(120, 0))) {
					bricks.clear();
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::NextColumn();
			ImGui::Button("Fill", { (elemSize.x) / 2, elemSize.y });
			ImGui::Columns(1);


			if (!focused) {
				float mousePosX = sf::Mouse::getPosition(App).x;
				float mousePosY = sf::Mouse::getPosition(App).y;
				switch (selected) {
				case 0:
					for (auto &e : bricks) {
						float width2 = e.size().width / 2;
						float height2 = e.size().height / 2;
						float x1 = e.x() - width2;
						float x2 = e.x() + width2;
						float y1 = e.y() - height2;
						float y2 = e.y() + height2;
						if (mousePosX > x1 && mousePosX < x2 && mousePosY > y1 && mousePosY < y2) {
							outline.setSize({ e.size().width, e.size().height });
							outline.setOrigin(width2, height2);
							outline.setPosition(e.x(), e.y());
							outline.setOutlineColor(sf::Color::Yellow);
							//e.highlight(sf::Color::Blue, true);
							if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
								selectedBrick = &e;
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
				case 1:
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
							std::cout << "New brick added at: " << newBrickX << ", " << newBrickY << std::endl;
						}
					}
					if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
						for (auto &e : bricks) {
							float width2 = e.size().width / 2;
							float height2 = e.size().height / 2;
							float x1 = e.x() - width2;
							float x2 = e.x() + width2;
							float y1 = e.y() - height2;
							float y2 = e.y() + height2;
							if (mousePosX > x1 && mousePosX < x2 && mousePosY > y1 && mousePosY < y2) {
							//if (newBrickX == e.x() && newBrickY == e.y()) {
								taken = true;
								bricks.erase(
									std::remove_if(bricks.begin(), bricks.end(),
										[&](brick &o) { return e.x() == o.x() && e.y() == o.y(); }),
									bricks.end());
								std::cout << "Brick deleted at: " << newBrickX << ", " << newBrickY << std::endl;

							}
						}
						/*if (taken) {
							//bricks.emplace_back(brick(tx["brick"], 1, 100, { newBrickX, newBrickY }));
							bricks.erase(
								std::remove_if(bricks.begin(), bricks.end(),
									[&](brick &o) { return newBrickX == o.x() && newBrickY == o.y(); }),
								bricks.end());
							std::cout << "Brick deleted at: " << newBrickX << ", " << newBrickY << std::endl;
						}*/
					}
					break;
				default:
					break;
				}
			}
			
			//color = ref_color_v; 0;

			ImGui::ColorPicker3("MyColor##4", (float*)&color, ImGuiColorEditFlags_PickerHueWheel);// , &ref_color_v.x);

			if (selectedBrick) {
			static ImVec4 ref_color_v = selectedBrick->sprite().getColor();			
			if (ImGui::Button("Set color", elemSize)) {
				selectedBrick->sprite().setColor(color);
			}
			ImGui::Checkbox("Brush mode", &brushMode);
			if (brushMode) {
				selectedBrick->sprite().setColor(color);
			}
			}

			ImGui::GetIO().FontGlobalScale = 2.5f;

			ImGui::End();
			sp_background.setColor(sf::Color(255, 255, 255, 255));

			//Clearing screen
			App.clear();
			//Drawing
			App.draw(sp_background);

			for (auto &e : bricks) {
				App.draw(e.sprite());
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