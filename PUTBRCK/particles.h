#pragma once
#include "entity.h"

struct particle {
	sf::Sprite sp_part;
	float velX = 0;
	float velY = 0;
};

class particles : public entity{
	sf::RenderWindow& App;
	std::vector<particle> particles_;
	unsigned int amount;
	int speed;
	sf::Time lifeTime;
	int alpha;
public:
	particles(sf::RenderWindow &App, sf::Texture& tx_particle, unsigned int amount, sf::Color color);	
	~particles();
	void spawn(int x, int y);
	void move(sf::Time dT);
	std::vector<particle> getVector() { return particles_; }
};

