#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "ball.h"


class bonus : public ball {
private:
	bonusType bonusType_;
public:
	
	bonus(sf::RenderWindow &App, sf::Texture& tx, bonusType type, sf::Vector2i speed_ = { 0,200 }, sf::Vector2f position_ = { 500,500 });
	~bonus();
	::type type() override { return type::bonus; }
	void bounce(collision direction) override;
	bool destroy() override;
	bonusType bonusInside() { return bonusType_; }
};
