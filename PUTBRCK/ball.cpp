#include "ball.h"
#include <iostream>



ball::ball(sf::RenderWindow &App, sf::Texture& tx_ball, sf::Vector2i speed_, sf::Vector2f position_) 
: entity(tx_ball, position_), App(App) {

	radius = (size().height) / 2;
	radius -= 5;
	sprite_.setOrigin(radius, radius);
	speed = speed_;
	//std::cout << sp_ball.getOrigin().x << ", " << sp_ball.getOrigin().x << std::endl;
	//entityID++;
	toDelete = false;
}

ball::~ball() {
}

void ball::move(sf::Time dT) {
	sprite_.setPosition(
		x() + (speed.x * dT.asSeconds()),
		y() + (speed.y *dT.asSeconds())
	);
	position = sprite_.getPosition();
}

void ball::move(float x, float y) {
	sprite_.setPosition(x, y);
	position = sprite_.getPosition();
}

void ball::offset(float x, float y) {
	sf::Vector2f pos = sprite_.getPosition();
	sprite_.setPosition(pos.x + x, pos.y + y);
}

bool ball::checkCollision() {
	//right
	if (x() > PLAYAREA_X + PLAYAREA_WIDTH - radius && speed.x > 0) {
		speed.x = -speed.x;
		this->move(PLAYAREA_WIDTH + PLAYAREA_X - radius, y());
		return false;
	}
	//left
	if(x() < PLAYAREA_X + radius && speed.x < 0) {
		speed.x = -speed.x;
		this->move(PLAYAREA_X + radius, y());
		return false;
	}

	//bottom
	if (y() > PLAYAREA_Y + PLAYAREA_HEIGHT - radius && speed.y > 0){
		speed.y = -speed.y;
		this->move(x(), PLAYAREA_HEIGHT + PLAYAREA_Y - radius);
		audio.play("ballLost");		
		toDelete = true;
		return true;
	}
	//top
	if(y() < PLAYAREA_Y + radius && speed.y < 0) {
		speed.y = -speed.y;
		this->move(x(), PLAYAREA_Y + radius);
		return false;
	}
}

void ball::bounce(collision direction) {
	switch (direction) {
	case collision::up:
		speed.y = -speed.y;
		break;
	case collision::down:
		speed.y = -speed.y;
		break;
	case collision::left:
		speed.x = -speed.x;
		break;
	case collision::right:
		speed.x = -speed.x;
		break;
	}
	if(direction != collision::none) {
		audio.play("hit1");
	}
	
}

void ball::setSpeed(int x, int y) {
	speed.x = x;
	speed.y = y;
}

void ball::changeSpeed(int x, int y) {
	if (speed.x < 0) {
		speed.x += x;
		speed.x = clamp(speed.x, -600, -50);
	}
	else {
		speed.x -= x;
		speed.x = clamp(speed.x, 50, 600);
	}
	if (speed.y < 0) {
		speed.y += y;
		speed.y = clamp(speed.y, -600, -50);
	}
	else {
		speed.y -= y;
		speed.y = clamp(speed.y, 50, 600);
	}
	
}

sf::Vector2i ball::getSpeed() {
	return speed;
}


