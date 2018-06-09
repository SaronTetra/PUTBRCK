#pragma once
#include <SFML/Graphics.hpp>
#include "resources.h"

float clamp(float x, float a, float b);

enum class collision { none, left, right, up, down };
enum class type { entity, ball, paddle, brick, bonus };
enum class bonusType{ bonusBall, fasterBall, slowerBall, biggerBall, smallerBall};

class entity {
protected:
	//int entityID;
	//sf::Texture texture_;
	sf::Sprite sprite_;

	sf::Vector2f position;

public:
	entity(const sf::Texture& tx_entity, const sf::Vector2f& position_);
	sf::Sprite& sprite() { return sprite_; }
	virtual type type() { return type::entity; };
	void setTexture(sf::Texture& tx_entity) {
		sprite_.setTexture(tx_entity);
	}
	virtual bool destroy();
	virtual int points();
	
	auto x() { return sprite_.getPosition().x; }
	auto y() { return sprite_.getPosition().y; }

	//auto grid() { return }

	auto size() { return sprite_.getLocalBounds(); }

	//int id() { return entityID; }
	bool toDelete = false;
};
