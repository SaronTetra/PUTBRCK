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
std::map<std::string, sf::Music> music;  //plays directyl from a file, no buffer

void TextureContainer::loadTexture(std::string src, std::string name) {
	std::cout << "Loading: " << src << std::endl;
	sf::Texture temp;
	temp.loadFromFile(src);
	texture[name] = temp;
}

void TextureContainer::init() {
	{
		//-------------------------------------------------
		//---------------------TEXTURES--------------------
		//-------------------------------------------------

		tx.loadTexture("assets/textures/background.png", "background");
		tx.loadTexture("assets/textures/logo.png", "logo");
		tx.loadTexture("assets/textures/ball.png", "ball");
		tx.loadTexture("assets/textures/paddle.png", "paddle");
		tx.loadTexture("assets/textures/brick.png", "brick");
		tx.loadTexture("assets/textures/powerBall.png", "powerBall");
		tx.loadTexture("assets/textures/power_bonus.png", "bonusPower");
		tx.loadTexture("assets/textures/faster_bonus.png", "bonusFaster");
		tx.loadTexture("assets/textures/slower_bonus.png", "bonusSlower");
		tx.loadTexture("assets/textures/ball_bonus.png", "bonusBall");

		music["bg1_A"].openFromFile("assets/music/PM_ATG_2_100BPM_A.ogg");
		music["bg1_B"].openFromFile("assets/music/PM_ATG_2_100BPM_B.ogg");
		music["bg1_C"].openFromFile("assets/music/PM_ATG_2_100BPM_C.ogg");
		music["bg1_D"].openFromFile("assets/music/PM_ATG_2_100BPM_D.ogg");
		music["bg1_E"].openFromFile("assets/music/PM_ATG_2_100BPM_E.ogg");
		music["bg1_F"].openFromFile("assets/music/PM_ATG_2_100BPM_F.ogg");
		music["credits"].openFromFile("assets/music/RetroSynthDream.ogg");

		music["bg1_A"].play();
		music["bg1_B"].play();
		music["bg1_C"].play();
		music["bg1_D"].play();
		music["bg1_E"].play();
		music["bg1_F"].play();

		music["bg1_A"].setLoop(true);
		music["bg1_B"].setLoop(true);
		music["bg1_C"].setLoop(true);
		music["bg1_D"].setLoop(true);
		music["bg1_E"].setLoop(true);
		music["bg1_F"].setLoop(true);

		music["bg1_A"].setVolume(0);
		music["bg1_B"].setVolume(0);
		music["bg1_C"].setVolume(0);
		music["bg1_D"].setVolume(0);
		music["bg1_E"].setVolume(0);
		music["bg1_F"].setVolume(0);

		audio.loadAudio("assets/sfx/PM_SA_IMPACT_33.ogg", "hit1");
		audio.loadAudio("assets/sfx/PM_SA_CINEMATIC_IMPACT_21.ogg", "hit2");
		audio.loadAudio("assets/sfx/PM_SA_CINEMATIC_IMPACT_32.ogg", "bonus");
		audio.loadAudio("assets/sfx/PM_SA_CINEMATIC_IMPACT_27.ogg", "ballLost");
	}
}

sf::Texture& TextureContainer::operator[](std::string name) {
	return texture.at(name);
}

