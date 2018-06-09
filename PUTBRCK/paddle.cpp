#include "paddle.h"
#include <iostream>


paddle::paddle(sf::RenderWindow & App, sf::Texture & tx) : entity(tx, {100,100}), App(App) {
	sprite_.setOrigin(size().width / 2, size().height / 2);
}

paddle::~paddle() {
}

void paddle::move() {
	int paddlePos = sf::Mouse::getPosition(App).x - size().width;
	if (paddlePos < PLAYAREA_X + size().width / 2) { paddlePos = PLAYAREA_X + size().width / 2; }
	if (paddlePos > PLAYAREA_X + PLAYAREA_WIDTH - size().width/2) { paddlePos = PLAYAREA_X + PLAYAREA_WIDTH - size().width/2; }
	sprite_.setPosition(paddlePos, 900);
}
