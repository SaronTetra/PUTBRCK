#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "cScreen.h"
#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "bonus.h"
#include "quadtree.h"
#include "level.h"
#include "particles.h"


#define M_PI 3.14159265358979323846


class game : public cScreen {
private:
	sf::RenderWindow &App;

	//std::map<std::string, sf::Texture> tx; //sprites are pointers to textures, texture must not be destroyed
	//std::map<std::string, sf::SoundBuffer> sfxBuffer; //similar to texture/sprite combo
	//std::map<std::string, sf::Sound> sfx; //similar to texture/sprite combo
	//std::map<std::string, sf::Music> music;  //plays directyl from a file, no buffer
	sf::Sprite sp_background;
	sf::Font font;
	sf::Text info;
	sf::Text infoScore;
	sf::Text infoCannon;

	sf::Time elapsed_;
	sf::Clock clock_;
	sf::Clock cooldownClock;

	std::vector<ball> balls;
	std::vector<ball> bullets;
	std::vector<ball> points;
	std::vector<brick> bricks;
	std::vector<bonus> bonuses;
	std::vector<particles> particles_;
	paddle * pad;
	sf::CircleShape point;

	level level_;
	//levelContainer levels;
	std::vector<level> levels;
	int currentLevel;

	int currentBullet;
	int currentPoint;
	int cannon;
	bool cannonFire;
	float laserScale;
	bool laserPlayed;

	int score;
	bool running;
	bool paused;

	bool toReset;
public:
	game(sf::RenderWindow &App);
	void nextLevel();
	int Run() override;

	sf::Time getElapsed();
	void restartClock();
	void applyBonus(bonusType bonus);
	void pause() { paused = true; }
	void resume() {
		elapsed_ = clock_.restart(); //elapsed is normal elapsed time (too big for pause)
		elapsed_ = clock_.restart(); //no time has passed (elapsed = 0)
		paused = false;
	}
	collision checkCollision(ball &ball, entity *object);
	void restart();
	void restart(std::string levelName);

	int getScore() { return score; }
};
