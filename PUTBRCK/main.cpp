#include <iostream>
#include <SFML/Graphics.hpp>
#include "screens.h"
#include "editor.h"
#include <imgui-SFML.h>
#include <imgui.h>
#include "credits.h"


int main(int argc, char** argv) {
	//Applications variables
	std::vector<cScreen*> Screens;
	int screen = 0;

	//Window creation
	sf::RenderWindow App(sf::VideoMode(1920, 1080, 32), "PUTBRCK", sf::Style::Titlebar | sf::Style::Close);
	//App.setVerticalSyncEnabled(true);
	App.setFramerateLimit(60);

	//Mouse cursor no more visible
	App.setMouseCursorVisible(false);
	tx.init();

	//Screens preparations
	game game(App);
	Screens.push_back(&game);

	editor editor(App, &game);
	Screens.push_back(&editor);

	credits credits(App, &game);
	Screens.push_back(&credits);


	screen = 0;
	//Main loop
	try {
		while (screen >= 0) {
			if (screen == 10) { 
				break;
			};
			screen = Screens[screen]->Run();
		}
	}	
	catch(...) {
		std::cout << "ERROR" << std::endl;
	}
	//system("PAUSE");
	return EXIT_SUCCESS;
}
