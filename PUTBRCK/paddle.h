#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "entity.h"

class paddle : public entity {
private:
	sf::RenderWindow &App;
public:
	paddle(sf::RenderWindow &App, sf::Texture& tx);
	~paddle();

	void move();
	::type type() override { return type::paddle; }
};





	 