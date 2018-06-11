#pragma once
#include "cScreen.h"
#include "brick.h"
#include "bonus.h"
#include "game.h"
#include "level.h"

struct editorSettings {
	int gridSize;

	editorSettings() : gridSize(16) {}
};


struct brickMap {
	//std::vector<std::vector<brick>> pos;
	std::vector<brick> position;
};

class editor :	public cScreen {
private:
	sf::RenderWindow &App;
	game* thisGame;

	//std::map<std::string, sf::Texture> tx; //sprites are pointers to textures, texture must not be destroyed
	std::map<std::string, sf::SoundBuffer> sfxBuffer; //similar to texture/sprite combo
	std::map<std::string, sf::Sound> sfx; //similar to texture/sprite combo
	std::map<std::string, sf::Music> music;  //plays directyl from a file, no buffer
	sf::Sprite sp_background;
	sf::Font font;
	sf::Text debug;
	sf::Text infoScore;

	sf::Time elapsed_;
	sf::Clock clock_;

	std::vector<brick> bricks;
	std::vector<bonus> bonuses;

	sf::RectangleShape indicator;
	brick* selectedBrick;

	level level_;
	char InputBuf[256];

	int score;
	bool running;
	bool focused;
public:
	editor(sf::RenderWindow &App, game* thisGame);
	int Run() override;
};

