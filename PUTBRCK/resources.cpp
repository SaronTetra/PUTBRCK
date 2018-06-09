#include "resources.h"
#include <iostream>

void AudioContainer::loadAudio(std::string src, std::string name) {
	std::cout << "Loading: " << src << std::endl;
	audio[name].init(src);
	audio[name].setVolume(50);
}

void AudioContainer::play(std::string name) {
	//sounds.emplace_back(audio[name]);
	sounds.at(soundCount) = audio[name];
	sounds.at(soundCount).play();
	++soundCount;

	if(soundCount >= MAX_SOUNDS) {
		soundCount = 0;
	}
}



AudioContainer audio;
TextureContainer tx;

void TextureContainer::loadTexture(std::string src, std::string name) {
	std::cout << "Loading: " << src << std::endl;
	sf::Texture temp;
	temp.loadFromFile(src);
	texture[name] = temp;
}

sf::Texture& TextureContainer::operator[](std::string name) {
	return texture.at(name);
}

