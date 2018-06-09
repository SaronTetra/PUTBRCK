#pragma once
#include "resources.h"
#include "brick.h"

class level {
private:
	std::string name;
	std::vector<brick> bricks;
public:
	void readFromFile(std::string filePath);
	void writeToFile(std::string filePath);
	void saveBricks(std::vector<brick> newBricks);
	void loadBricks(std::vector<brick>& brickList);
	//void setName(std::string name) { name_ = name; }
	//std::string name() { return name_; }
};

class levelContainer {
private:
	std::map<std::string, level> texture;
public:
	levelContainer();
	~levelContainer();
	void load(std::string fileName, std::string levelName);


	sf::Texture& operator[](std::string name);
};