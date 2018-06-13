#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "entity.h"


class ball : public entity{
protected:
	sf::RenderWindow &App;
	sf::Vector2i speed;

public:
	ball(const ball& other)
		: entity(other),
		  App(other.App),
		  speed(other.speed),
		  radius(other.radius),
		  toDelete(other.toDelete) {
	}

	ball(ball&& other)
		: entity(std::move(other)),
		  App(other.App),
		  speed(std::move(other.speed)),
		  radius(other.radius),
		  toDelete(other.toDelete) {
	}

	ball& operator=(const ball& other) {
		if (this == &other)
			return *this;
		entity::operator =(other);
		speed = other.speed;
		radius = other.radius;
		toDelete = other.toDelete;
		return *this;
	}

	ball& operator=(ball&& other) {
		if (this == &other)
			return *this;
		entity::operator =(std::move(other));
		speed = std::move(other.speed);
		radius = other.radius;
		toDelete = other.toDelete;
		return *this;
	}

protected:
	int radius;
public:
	ball(sf::RenderWindow &App, sf::Texture& tx, sf::Vector2i speed_ = { 200,200 }, sf::Vector2f position_ = {500,500});
	~ball();
	::type type() override { return type::ball; }
	float x() const { return position.x; }
	float y() const { return position.y; }
	int r() const { return radius; }

	void move(sf::Time dT);
	void move(float x, float y);
	void offset(float x, float y);
	bool checkCollision();
	virtual void bounce(collision direction);
	void setSpeed(int x, int y);
	void changeSpeed(int x, int y);
	sf::Vector2i getSpeed();

	bool toDelete;
	bool isPower;
	bool moving;
};
