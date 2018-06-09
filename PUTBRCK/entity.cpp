#include "entity.h"
float clamp(float x, float a, float b) {
	if (x < a) { x = a; }
	if (x > b) { x = b; }
	return x;
}
entity::entity(const sf::Texture& tx_entity, const sf::Vector2f& position_) 
: position(position_){
	sprite_.setTexture(tx_entity);
	sprite_.setPosition(position_);
	position = position_;
}

bool entity::destroy() {
	return true;
}

int entity::points() {
	return 0;
}