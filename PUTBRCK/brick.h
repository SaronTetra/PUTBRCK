#pragma once
#include <SFML/Graphics.hpp>

#include "entity.h"

class brick : public entity{
private:
	int health;
	int score;
	bonusType bonus_;
	sf::Color color_;

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
	bonusType bonus() override;
	void highlight(sf::Color color, bool on);
	void setHealth(int h) { health = h; }
	void setPoints(int s) { score = s; }
	int getHealth() { return health; }
	void setBonus(bonusType newBonus) { bonus_ = newBonus; }
	//TODO: Glow on hit (render twice);
};
