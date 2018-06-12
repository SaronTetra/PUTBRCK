#include "bonus.h"
#include "myrandom.h"

bonus::bonus(sf::RenderWindow & App, sf::Texture & tx_bonus, bonusType type, sf::Vector2i speed_, sf::Vector2f position_) 
: ball(App, tx_bonus, speed_, position_){
	radius = tx_bonus.getSize().x / 2;
	sprite_.setOrigin(radius, radius);
	speed = speed_;
	//std::cout << sp_ball.getOrigin().x << ", " << sp_ball.getOrigin().x << std::endl;
	bonusType_ = type;// bonusType(randomInt(0, 5));
}

bonus::~bonus() {
}

void bonus::bounce(collision direction) {
	if (direction != collision::none) {
		destroy();
	}
}

bool bonus::destroy() {
	//TODO: Proper destruction
	move(-100, -100);
	speed = { 0,0 };
	return true;
}
