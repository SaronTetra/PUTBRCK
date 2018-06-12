#pragma once
#include <string>
#include <SFML/Audio.hpp>
#include <map>
#include <list>
#include <SFML/Graphics.hpp>
#include <fstream>

#define MAX_SOUNDS 10

#define PLAYAREA_WIDTH  1100
#define PLAYAREA_HEIGHT  850
#define PLAYAREA_X  410
#define PLAYAREA_Y  110

//
//
//------------------------------AUDIO--------------------------------
//
//

class Audio {

private:
	sf::SoundBuffer buffer;
	sf::Sound sound;
	std::string _src;
	//std::list
public:
	void init(std::string src) {        //Initialize audio
		_src = src;
		buffer.loadFromFile(src);
		sound.setBuffer(buffer);
	}
	void play() {
		sound.play();       // Play queued audio
	}
	void stop() {
		sound.stop();
	}
	void setVolume(int vol) {
		sound.setVolume(vol);
	}
	//void setVolume(), void setPitch() ....


};

class AudioContainer {
private:
	std::vector<Audio> sounds;
	std::map<std::string, Audio> audio;
	int soundCount = 0;
public:
	void loadAudio(std::string src, std::string name); // file path, map name
	void play(std::string name);
	AudioContainer() {
		sounds.resize(MAX_SOUNDS);
	}
};


//
//
//------------------------------TEXTURE--------------------------------
//
//

class TextureContainer {
private:
	std::map<std::string, sf::Texture> texture; 
public:
	void loadTexture(std::string src, std::string name); // file path, map name
	TextureContainer() {}

	void init();

	//friend auto operator[](std::string name, const TextureContainer& TC);
	sf::Texture& operator[](std::string name);
};


extern AudioContainer audio;
extern TextureContainer tx;
extern std::map<std::string, sf::Music> music;  //plays directyl from a file, no buffer

//TODO: Music manager
//TODO: Texture manager



//
//
//------------------------------LEVELS--------------------------------
//
//
