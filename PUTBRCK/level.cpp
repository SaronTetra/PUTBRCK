#include "level.h"
#include <iostream>

void level::readFromFile(std::string filePath) {
	bricks.clear();
	std::fstream file(filePath, std::ios::in | std::ios::binary);
	if (file) {
		brick newBrick;
		while (file.read(reinterpret_cast<char*>(&newBrick), sizeof(brick))) {
			bricks.push_back(newBrick);
			
		}
		std::cout << "Level loaded" << std::endl;
	}
	else {
		std::cerr << "Could not load level file" << std::endl;
	}
	file.close();
}

void level::writeToFile(std::string filePath) {
	std::fstream file(filePath, std::ios::out | std:: ios:: binary);
	if (file) {
		for (auto e : bricks) {
			file.write(reinterpret_cast<const char*>(&e), sizeof(e));
		}
		std::cout << "Level saved to file" << std::endl;
	}
	else {
		std::cerr << "Could not write to file" << std::endl;
	}
	file.close();
}


void level::saveBricks(std::vector<brick> newBricks) {
	bricks = std::move(newBricks);
	std::cout << "Bricks saved in memory" << std::endl;
}

void level::loadBricks(std::vector<brick>& brickList) {
	brickList = std::move(bricks);
	//brickList = bricks;
	std::cout << "Bricks loaded" << std::endl;
}
