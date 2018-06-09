#include "quadtree.h"


quadtree::quadtree(sf::RenderWindow &App, int levels, bounds bounds_) : App(&App), levels_(levels), bounds_(bounds_) {
	//nodes_ = ;
	//debug.setSize(  );
	debug.setPosition(bounds_.x(), bounds_.y());
	debug.setSize({ bounds_.width() * 1.0f, bounds_.height() * 1.0f });
	debug.setFillColor(sf::Color::Red);
}

quadtree::~quadtree() {
}

void quadtree::clear() {
	objects_.clear();
	for (int i = 0; i < nodes_.size(); ++i) {
		if (nodes_[i] != nullptr) {
			nodes_[i]->clear();
			nodes_[i] = nullptr;
		}
	}

}

void quadtree::split() {
	int subWidth = bounds_.width() / 2;
	int subHeight = bounds_.height() / 2;
	int x = bounds_.x();
	int y = bounds_.y();	
	int x2 = bounds_.x2();
	int y2 = bounds_.y2();
	bounds b0 = { x + subWidth, y, x2, y + subHeight };
	bounds b1 = { x, y, x + subWidth, y + subWidth };
	bounds b2 = { x, y + subHeight, x + subWidth, y2 };
	bounds b3 = { x + subWidth, y + subHeight, x2, y2 };
	nodes_[0] = new quadtree(*App, levels_ + 1, b0 );
	nodes_[1] = new quadtree(*App, levels_ + 1, b1 );
	nodes_[2] = new quadtree(*App, levels_ + 1, b2 );
	nodes_[3] = new quadtree(*App, levels_ + 1, b3 );

}

void quadtree::fill() {
	if(levels_ < MAX_LEVELS) {
		split();
		for(auto& e:nodes_) {
			e->fill();
		}
	}
}

int quadtree::index(entity* object) {
	int index = -1;
	double verticalMidpoint = bounds_.x() + (bounds_.width() / 2);
	double horizontalMidpoint = bounds_.y() + (bounds_.height() / 2);

	bool topQuad = (object->y() < horizontalMidpoint && object->y() + object->size().height < horizontalMidpoint);

	bool bottomQuad = (object->y() > horizontalMidpoint);

	if(object->x() < verticalMidpoint && object->x() + object->size().width < verticalMidpoint) {
		if(topQuad) {
			index = 1;
		}
		else if(bottomQuad) {
			index = 2;
		}
	}
	else if( object->x() > verticalMidpoint) {
		if(topQuad) {
			index = 0;
		}
		else if (bottomQuad) {
			index = 3;
		}
	}

	return index;

}

void quadtree::insert(entity* object) {
	if(nodes_[0] != nullptr) {
		int index_ = index(object);

		if(index_ != -1) {
			nodes_[index_]->insert(object);
			return;
		}
	}
	objects_.push_back(object);

	if(objects_.size() > MAX_OBJECTS && levels_ < MAX_LEVELS) {
		if(nodes_[0] == nullptr) {
			split();
		}
		int i = 0;
		while(i < objects_.size()) {
			auto k = std::next(objects_.begin(), i);
			//int index_ = index(*k);
			int index_ = 0;
			if(index_ != -1) {
				nodes_[index_]->insert(object);
				objects_.erase(k);
			}
			else { i++; }
		}
	}
}

void quadtree::draw(int color) {
	debug.setPosition(bounds_.x(), bounds_.y());
	debug.setSize({ bounds_.width() * 1.0f, bounds_.height() * 1.0f });
	debug.setFillColor(sf::Color(color,0,0));
	App->draw(debug);
	if (nodes_[0] != nullptr) {
		for (auto& e : nodes_) {
			e->draw(color + 25);
		}
	}
}

void quadtree::retrive(std::list<entity*> &returnObjects, entity object) {
	if (nodes_[0] != nullptr) {
		int index_ = index(&object);
		if (index_ != -1) {
			nodes_[index_]->retrive(returnObjects, object);
		} else {
			for (int i = 0; i < nodes_.size(); ++i) {
				nodes_[i]->retrive(returnObjects, object);
			}
		}
}
		returnObjects.insert(returnObjects.end(), objects_.begin(), objects_.end());
	
}

