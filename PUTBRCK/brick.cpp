#include "brick.h"
#include <iostream>
#include "myrandom.h"

brick::brick(const sf::Texture& tx_brick, int health, int score, const sf::Vector2f& position_)
: entity(tx_brick ,position_), health(health), score(score) {
	sprite_.setOrigin(size().width / 2, size().height / 2);
	sprite_.setColor(sf::Color(255*randomInt(0,1), 255 * randomInt(0, 1), 255 * randomInt(0, 1)));
}

brick::~brick() {
}

bool brick::destroy() {
	health--;
	//sprite_.setColor(sf::Color::Red);
	if (health <= 0) {
		//std::cout << "Brick destroyed" << std::endl;
		//TODO: Implement proper destruction
		sprite_.setPosition(-1000, -1000);
		toDelete = true;
		audio.play("hit2");
		return true;
	}
	return false;
}

int brick::points() {
	return score;
}
