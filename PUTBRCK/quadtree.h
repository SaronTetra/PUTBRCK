#pragma once
#include <list>
#include <array>
#include "entity.h"

class bounds {
private:
	int x_ = PLAYAREA_X;
	int y_ = PLAYAREA_Y;
	int x2_ = x_ + PLAYAREA_WIDTH;
	int y2_ = y_ + PLAYAREA_HEIGHT;
public:
	bounds(){}
	bounds(int x, int y, int x2, int y2) : x_(x), y_(y), x2_(x2), y2_(y2) {}

	int x() const {
		return x_;
	}

	int y() const {
		return y_;
	}

	int x2() const {
		return x2_;
	}

	int y2() const {
		return y2_;
	}

	int width() const {
		return x2() - x();
	}

	int height() const {
		return y2() - y();
	}
};

class quadtree {
private:
	sf::RenderWindow *App;
	int MAX_OBJECTS = 25;
	int MAX_LEVELS = 5;
	int levels_;
	std::list<entity*> objects_;
	bounds bounds_;
	std::array<quadtree*, 4> nodes_;
	sf::RectangleShape debug;
public:
	quadtree() {}
	quadtree(sf::RenderWindow &App, int levels, bounds bounds_);
	~quadtree();

	void clear();
	void split();
	void fill();
	int index(entity* object);
	void insert(entity* object);
	void draw(int color);

	void retrive(std::list<entity*>& returnObjects, entity object);
};

