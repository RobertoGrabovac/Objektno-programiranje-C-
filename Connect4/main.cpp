#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <tuple>
#include <limits>
#include "Connect4.h"

int main(void) {
	Connect4 igra;
	sf::Event event;
	while (igra.stillPlaying()) {
		event = igra.processInput();
		igra.updateGame(event);
	}
	
	return 0;
}