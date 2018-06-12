#pragma once
#include <SFML/Graphics.hpp>
#include "resources.h"

float clamp(float x, float a, float b);

enum class collision { none, left, right, up, down, power};
enum class type { entity, ball, paddle, brick, bonus };
enum class bonusType{ none, bonusBall, fasterBall, slowerBall, powerBall};

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
	virtual bonusType bonusInside() { return bonusType::none; };
	
	auto x() { return sprite_.getPosition().x; }
	auto y() { return sprite_.getPosition().y; }

	//auto grid() { return }

	auto size() { return sprite_.getLocalBounds(); }


	auto x1() { return sprite_.getPosition().x - size().width/2; }
	auto x2() { return sprite_.getPosition().x + size().width / 2; }

	auto y1() { return sprite_.getPosition().y - size().height/2; }
	auto y2() { return sprite_.getPosition().y + size().height / 2; }
	bool toDelete = false;
};
