#pragma once
#include <SFML/Graphics.hpp>

#include "entity.h"

class brick : public entity{
private:
	int health;
	int score;
	//sf::bonus bonus;

public:
	brick() : entity(tx["brick"], {0,0}) {
		health = 0;
		score = 0;
	}
	brick(const sf::Texture& tx_brick, int health, int score, const sf::Vector2f& position_);
	~brick();
	::type type() override { return type::brick; }
	
	bool destroy() override;
	int points() override;
	//TODO: Glow on hit (render twice);
};
