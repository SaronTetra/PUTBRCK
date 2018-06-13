#pragma once
#include "cScreen.h"
#include "brick.h"
#include "bonus.h"
#include "game.h"
#include "level.h"


class menu : public cScreen {
private:
	sf::RenderWindow &App;
	game* thisGame;

	sf::Sprite sp_logo;
	sf::Sprite sp_bg;

	sf::Time elapsed_;
	sf::Clock clock_;
	sf::Font font_;

	std::vector<sf::Text> text_;

	bool running;

	bool mouseOverlap(sf::Text& e);

	std::vector<std::string> menuText;
public:
	menu(sf::RenderWindow &App, game* thisGame);
	int Run() override;
};

