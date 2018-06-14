#include "particles.h"
#include "myrandom.h"
#include <iostream>


particles::particles(sf::RenderWindow & App, sf::Texture & tx_particle, unsigned int amount, sf::Color color) 
	: entity(tx_particle, {0,0}), App(App), amount(amount){

	for(int i=0; i<amount; ++i) {
		sf::Sprite temp;
		temp.setTexture(tx_particle);
		temp.setPosition(-100, -100);
		temp.setColor(color);
		temp.rotate(randomInt(0, 360));
		particle temp2;
		temp2.sp_part = temp;
		particles_.push_back(temp2);
	}
	speed = 100;
	lifeTime = sf::seconds(1);
	alpha = 255;

}

particles::~particles() {
	particles_.clear();
}

void particles::spawn(int x, int y) {
	for(auto& e:particles_) {
		e.sp_part.setPosition(x, y);
		e.velX = 0.125 * randomInt(-8,8);
		e.velY = 0.125 * randomInt(-8,8);
	}
}

void particles::move(sf::Time dT) {
	lifeTime -= dT;
	if(lifeTime.asMilliseconds() < 0) {
		particles_.clear();
	}
	alpha -= 10;
	if (alpha <= 0) { alpha = 10; }
	for (auto& e : particles_) {
		float x = e.sp_part.getPosition().x;
		float y = e.sp_part.getPosition().y;			 
		
		e.sp_part.setPosition(x + e.velX * dT.asSeconds() * speed, y + e.velY * dT.asSeconds() * speed);
		int r = e.sp_part.getColor().r;
		int g = e.sp_part.getColor().g;
		int b = e.sp_part.getColor().b;
		e.sp_part.setColor(sf::Color(r, g, b, alpha + randomInt(0, 10)));
	}
}
