#ifndef CONNECT4_H
#define CONNECT4_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <tuple>
#include <limits>
#include <string>
#include "Button.h"

class Connect4 {
protected:
	sf::RenderWindow window;
	sf::Vector2u dimension;
	sf::CircleShape player_disk;
	Button button1, button2, button3;
	sf::Vector2i player_choice;
	int grid[6][7];
	int disk_position;
	int game_mode;
	int current_player;
	int current_game_status;
	float radius;
	sf::Color backgroundColor;
	sf::Color gridColor;
public:
	Connect4();
	sf::Event processInput();
	void updateGame(sf::Event);
	bool stillPlaying();
	~Connect4();
private:
	sf::CircleShape showDisk(sf::Event);
	void updateGrid(sf::Event);
	void animation(sf::Event, int, int);
	void createCurrentGrid(sf::CircleShape&);
	bool check(int, int, int, int, int);
	int gameStatus();
	int countDisks(int, int);
	int gridEvaluation();
	int minimax(int, int, int, bool);
	void cumputerPlays(sf::Event event);
	void finalResult();
};

#endif

