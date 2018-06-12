#pragma once
#include "cScreen.h"
#include "brick.h"
#include "bonus.h"
#include "game.h"
#include "level.h"


class credits : public cScreen {
private:
	sf::RenderWindow &App;
	game* thisGame;

	sf::Sprite sp_logo;

	sf::Time elapsed_;
	sf::Clock clock_;
	sf::Font font_;

	std::vector<sf::Text> text_;

	bool running;

	bool mouseOverlap(entity& e);

	std::vector<std::string> creditsText;
	void loadCredits(std::string filePath);
public:
	credits(sf::RenderWindow &App, game* thisGame);
	int Run() override;
};

