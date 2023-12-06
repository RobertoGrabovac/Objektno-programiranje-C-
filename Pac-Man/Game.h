#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Window.h"
#include "PacMan.h"
#include "Ghosts.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <chrono>
#include<cstdlib>

using namespace std;

class Game {
public:
	friend class PacMan;
	Game();
	void processInput();
	void update();
	void render();
	bool isFinished();
	bool isPaused();
	void clockRestart();
	sf::Time elapsedTime();
	void saveHighScore();
private:
	Window gameWindow;
	PacMan pacman;
	Ghost blinky;
	Ghost pinky;
	Ghost clyde;
	Ghost inky;
	bool paused;
	char gameBoard[21][21];
	int blockSize;
	sf::Texture map;
	sf::Sprite mapSprite;
	sf::Texture allElements;
	sf::Sprite allSprite;
	sf::Sprite readySprite;
	sf::Sprite gameOverSprite;
	sf::Texture youWin;
	sf::Sprite youWinSprite;
	sf::Texture pause;
	sf::Sprite pauseSprite;
	sf::Texture resume;
	sf::Sprite resumeSprite;
	sf::SoundBuffer buffer1;
	sf::Sound powerPellet;
	sf::SoundBuffer buffer3;
	sf::Sound startSound;
	sf::SoundBuffer buffer4;
	sf::Sound gameOverSound;
	sf::SoundBuffer buffer5;
	sf::Sound eatGhost;
	sf::SoundBuffer buffer6;
	sf::Sound eatFruit;
	int playerScore;
	int highestScore;
	int dotValue;
	int dotsNumber;
	int difficulty;
	bool bonusSet;
	int bonusValue;
	bool ghostsMode;
	bool pacmanEaten;
	bool gameWon;
	bool gameStarted;
	int ghostsEaten;
	sf::Font font;
	sf::Text pScore;
	sf::Text hScore;
	sf::Clock clock;
	sf::Time time;
	sf::Time energyTime;
	sf::Vector2u mapOffset;
	sf::Vector2f pinkyDesiredPosition();
	sf::Vector2f clydeDesiredPosition();
	sf::Vector2f inkyDesiredPosition();
	void getHighestScore();
	void setGameBoard();
	bool outsideOfBox(Ghost&);
	bool checkWalls(sf::Time);
	bool checkWallsGhost(sf::Time, Ghost&);
	bool checkDirection(sf::Vector2f, Direction, Ghost&);
	bool checkDirectionPacman(sf::Time, Direction);
	bool checkPacmanLife();
	void checkEnergyDot();
	GhostType ghostEaten();
	void playerScoreUpdate();
	void ghostsReset();
	void drawLives();
	void drawDifficulty();
	void drawBonus();
	void drawScores();
	void drawReady();
	void drawBoard();
	void drawGameOver();
	void drawYouWin();
};

Game::Game() : gameWindow("Pac-man", sf::Vector2u(21*32, 21*32 + 192) ), blockSize(32), playerScore(0), dotValue(10), difficulty(1), bonusSet(false), bonusValue(30), paused(false), blinky(GhostType::Blinky), pinky(GhostType::Pinky), clyde(GhostType::Clyde), inky(GhostType::Inky), ghostsMode(true), pacmanEaten(false),
	dotsNumber(146), gameWon(false), ghostsEaten(0), gameStarted(false) {
	setGameBoard();
	sf::Vector2u windowSize = gameWindow.getSize();
	map.loadFromFile("MapTexture.png");
	mapSprite.setTexture(map);
	getHighestScore();

	allElements.loadFromFile("all.png");
	allSprite.setTexture(allElements);
	readySprite.setTexture(allElements);
	gameOverSprite.setTexture(allElements);

	youWin.loadFromFile("youWin.png");
	youWinSprite.setTexture(youWin);

	pause.loadFromFile("pause.png");
	pauseSprite.setTexture(pause);
	pauseSprite.setScale(32.0f / pauseSprite.getLocalBounds().width, 32.0f / pauseSprite.getLocalBounds().width);
	pauseSprite.setPosition(gameWindow.getSize().x / 2.f, 21 * 32 + 192 - 32 - 10);

	resume.loadFromFile("resume.png");
	resumeSprite.setTexture(resume);
	resumeSprite.setScale(61.0f / resumeSprite.getLocalBounds().width, 50.0f / resumeSprite.getLocalBounds().width);
	resumeSprite.setPosition(gameWindow.getSize().x / 2.f, 21 * 32 + 192 - 32 - 20);

	mapOffset = sf::Vector2u(0, 128);

	buffer1.loadFromFile("power_pellet.wav");
	powerPellet.setBuffer(buffer1);

	
	buffer3.loadFromFile("start-game.wav");
	startSound.setBuffer(buffer3);

	buffer4.loadFromFile("pacman-lose.wav");
	gameOverSound.setBuffer(buffer4);

	buffer5.loadFromFile("eat_ghost.wav");
	eatGhost.setBuffer(buffer5);

	buffer6.loadFromFile("eat_fruit.wav");
	eatFruit.setBuffer(buffer6);
}

void Game::processInput() {
	Direction direction = pacman.getDirection();
	pacman.setPreviousDirection(direction);

	if (!gameWindow.getPaused()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			direction = Direction::Up;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			direction = Direction::Down;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			direction = Direction::Left;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			direction = Direction::Right;
	}

	pacman.setDirection(direction);
	gameWindow.update();
	difficulty = gameWindow.getDifficulty();
}

bool Game::checkPacmanLife() {
	if (ghostsMode) { // ako ga duhovi mogu pojesti
		sf::FloatRect p(pacman.getPosition().x, pacman.getPosition().y, blockSize, blockSize);
		sf::FloatRect g1(blinky.getPosition().x, blinky.getPosition().y, blockSize, blockSize);
		sf::FloatRect g2(pinky.getPosition().x, pinky.getPosition().y, blockSize, blockSize);
		sf::FloatRect g3(clyde.getPosition().x, clyde.getPosition().y, blockSize, blockSize);
		sf::FloatRect g4(inky.getPosition().x, inky.getPosition().y, blockSize, blockSize);
		sf::FloatRect intersection;
		float pArea = p.width * p.height;
		if (p.intersects(g1, intersection)) {
			float intersectionArea = intersection.width * intersection.height;
			if (intersectionArea >= 0.8f * pArea) {
				pacman.reduceLives();
				return true;
			}
		}
		else if (p.intersects(g2, intersection)) {
			float intersectionArea = intersection.width * intersection.height;
			if (intersectionArea >= 0.8f * pArea) {
				pacman.reduceLives();
				return true;
			}
		}
		else if (p.intersects(g3, intersection)) {
			float intersectionArea = intersection.width * intersection.height;
			if (intersectionArea >= 0.8f * pArea) {
				pacman.reduceLives();
				return true;
			}
		}
		else if (p.intersects(g4, intersection)) {
			float intersectionArea = intersection.width * intersection.height;
			if (intersectionArea >= 0.8f * pArea) {
				pacman.reduceLives();
				return true;
			}
		}
	}
	return false;
}

GhostType Game::ghostEaten() {
	if (ghostsMode == false) { // ako PacMan moze pojesti duhove
		sf::FloatRect p(pacman.getPosition().x, pacman.getPosition().y, blockSize, blockSize);
		sf::FloatRect g1(blinky.getPosition().x, blinky.getPosition().y, blockSize, blockSize);
		sf::FloatRect g2(pinky.getPosition().x, pinky.getPosition().y, blockSize, blockSize);
		sf::FloatRect g3(clyde.getPosition().x, clyde.getPosition().y, blockSize, blockSize);
		sf::FloatRect g4(inky.getPosition().x, inky.getPosition().y, blockSize, blockSize);
		sf::FloatRect intersection;
		float pArea = p.width * p.height;
		if (p.intersects(g1, intersection)) {
			float intersectionArea = intersection.width * intersection.height;
			if (intersectionArea >= 0.8f * pArea) {
				cout << "Pojeo blinky" << endl;
				return GhostType::Blinky;
			}
		}
		else if (p.intersects(g2, intersection)) {
			float intersectionArea = intersection.width * intersection.height;
			if (intersectionArea >= 0.8f * pArea) {
				cout << "Pojeo pinky" << endl;
				return GhostType::Pinky;
			}
		}
		else if (p.intersects(g3, intersection)) {
			float intersectionArea = intersection.width * intersection.height;
			if (intersectionArea >= 0.8f * pArea) {
				cout << "Pojeo clyde" << endl;
				return GhostType::Clyde;
			}
		}
		else if (p.intersects(g4, intersection)) {
			float intersectionArea = intersection.width * intersection.height;
			if (intersectionArea >= 0.8f * pArea) {
				cout << "Pojeo inky" << endl;
				return GhostType::Inky;
			}
		}
	}

	return GhostType::None;
}

bool Game::checkDirection(sf::Vector2f direct, Direction direction, Ghost& ghost) {
	sf::Vector2f pos = ghost.getPosition();
	pos += direct;

	float a = pos.x / 32, b = pos.y / 32;
	sf::FloatRect q(pos.x, pos.y, 32, 32);

	sf::FloatRect p1(floor(a) * 32, floor(b) * 32, 32, 32);
	int x1 = floor(a), y1 = floor(b);
	sf::FloatRect p2(floor(a) * 32, ceil(b) * 32, 32, 32);
	int x2 = floor(a), y2 = ceil(b);
	sf::FloatRect p3(ceil(a) * 32, floor(b) * 32, 32, 32);
	int x3 = ceil(a), y3 = floor(b);
	sf::FloatRect p4(ceil(a) * 32, ceil(b) * 32, 32, 32);
	int x4 = ceil(a), y4 = ceil(b);


	if (ghost.getPreviousDirection() == Direction::Up || ghost.getPreviousDirection() == Direction::Down) {
		if (direction == Direction::Right && ( ( gameBoard[y3 - 4][x3] != '#' && ((floor(b) * 32 + 32 - pos.y) / 32.f) > 0.98f) || 
			( gameBoard[y4-4][x4] != '#' && ((pos.y + 32.f - ceil(b) * 32) / 32.f) > 0.98f) )) {
			return true;
		}
		else if (direction == Direction::Left && ( (gameBoard[y1 - 4][x1] != '#' && ((floor(b) * 32 + 32 - pos.y) / 32.f) > 0.98f)
			|| ( gameBoard[y2 - 4][x2] != '#' && ((pos.y + 32.f - ceil(b) * 32 ) / 32.f) > 0.98f) ) ) {
			return true;
		}
	}
	else if (ghost.getPreviousDirection() == Direction::Left || ghost.getPreviousDirection() == Direction::Right) {
		if (direction == Direction::Up && ((gameBoard[y1 - 4][x1] != '#' && ((floor(a) * 32 + 32 - pos.x) / 32.f) > 0.98f) 
			|| (gameBoard[y3 - 4][x3] != '#' && ((pos.x + 32.f - ceil(a) * 32) / 32.f) > 0.98f) ) ) {
			return true;
		}
		else if (direction == Direction::Down && ((gameBoard[y2 - 4][x2] != '#' && ((floor(a) * 32 + 32 - pos.x) / 32.f) > 0.98f) || 
			(gameBoard[y4 -4][x4] != '#' && ((pos.x + 32.f - ceil(a) * 32) / 32.f) > 0.98f) )) {
			return true;
		}
	}

	if ((q.intersects(p1) && gameBoard[y1 - 4][x1] == '#') || (q.intersects(p2) && gameBoard[y2 - 4][x2] == '#') || (q.intersects(p3) && gameBoard[y3 - 4][x3] == '#') || (q.intersects(p4) && gameBoard[y4 - 4][x4] == '#'))
	{
		return false;
	}

	return true;

}

bool Game::checkWallsGhost(sf::Time time1, Ghost &ghost) {
	//ispod su pozicija vrata. U ovisnosti o kojem se duhu radi treba ovo vidjeti
	if (ghost.insideOfBox() == false)
		gameBoard[8][10] = '#';
	ghost.setPreviousDirection(ghost.getDirection());
	float amount = time1.asSeconds() * ghost.getSpeed();
	sf::Vector2f pos = ghost.getPosition();
	sf::Vector2f copyPos = pos;
	sf::Vector2f direct;
	sf::Vector2f min_direction = sf::Vector2f(0, 0);
	float min = 2000000;
	Direction direction;
	Direction new_direction = Direction::None;

	//sljedecih 6 if/else if vezani uz izlazak/ulazak iz/u tunel
	if (pos.x <= -32.f && ghost.getDirection() == Direction::Left) {
		ghost.setPosition(sf::Vector2f(672, 416));
		ghost.setDirection(Direction::Left);
		gameBoard[8][10] = 'D';
		return true;
	}
	else if (pos.x > 640 && pos.x <= 672 && ghost.getDirection() == Direction::Left) {
		ghost.setPosition(pos + sf::Vector2f(-amount, 0));
		gameBoard[8][10] = 'D';
		return true;
	}
	else if (pos.x > 673 && ghost.getDirection() == Direction::Right) {
		ghost.setPosition(sf::Vector2f(-32.f, 416));
		ghost.setDirection(Direction::Right);
		gameBoard[8][10] = 'D';
		return true;
	}
	else if (pos.x >= -32.f && pos.x <= 0.f && ghost.getDirection() == Direction::Right) {
		ghost.setPosition(pos + sf::Vector2f(amount, 0));
		gameBoard[8][10] = 'D';
		return true;
	}

	if (ghost.getPosition().x <= 1 && ghost.getPosition().y == 416 && ghost.getDirection() == Direction::Left) {
		ghost.setPosition(ghost.getPosition() + sf::Vector2f(-amount, 0));
		ghost.setDirection(Direction::Left);
		gameBoard[8][10] = 'D';
		return true;
	}
	else if (ghost.getPosition().x >= 639 && ghost.getPosition().y == 416 && ghost.getDirection() == Direction::Right) {
		ghost.setPosition(ghost.getPosition() + sf::Vector2f(amount, 0));
		ghost.setDirection(Direction::Right);
		gameBoard[8][10] = 'D';
		return true;
	}



	direct = sf::Vector2f(-amount, 0.f);
	if (ghost.distance(ghost.getPosition() + direct) <= min && ghost.getPreviousDirection() != Direction::Right && checkDirection(direct, Direction::Left, ghost)) {
		min_direction = direct;
		new_direction = Direction::Left;
		min = ghost.distance(ghost.getPosition() + direct);
	}
	
	direct = sf::Vector2f(amount, 0.f);
	if (ghost.distance(ghost.getPosition() + direct) <= min && ghost.getPreviousDirection() != Direction::Left && checkDirection(direct, Direction::Right, ghost)) { 
		min_direction = direct;
		new_direction = Direction::Right;
		min = ghost.distance(ghost.getPosition() + direct);
	}

	direct = sf::Vector2f(0.f, -amount);
	if (ghost.distance(ghost.getPosition() + direct) <= min && ghost.getPreviousDirection() != Direction::Down && checkDirection(direct, Direction::Up, ghost)) {
		min_direction = direct;
		new_direction = Direction::Up;
		min = ghost.distance(ghost.getPosition() + direct);
	}

	direct = sf::Vector2f(0.f, amount);
	if (ghost.distance(ghost.getPosition() + direct) <= min && ghost.getPreviousDirection() != Direction::Up && checkDirection(direct, Direction::Down, ghost)) { 
		min_direction = direct;
		new_direction = Direction::Down;
		min = ghost.distance(ghost.getPosition() + direct);
	}

	// duhovi nasumicno biraju skretanje ako bjeze
	if (ghostsMode == false) {
		int flags[4] = { 0, 0, 0, 0 };
		if (ghost.getPreviousDirection() != Direction::Up && checkDirection(sf::Vector2f(0.f, amount), Direction::Down, ghost))
			flags[0] = 1;
		else if (ghost.getPreviousDirection() != Direction::Down && checkDirection(sf::Vector2f(0.f, -amount), Direction::Up, ghost))
			flags[1] = 2;
		else if (ghost.getPreviousDirection() != Direction::Left && checkDirection(sf::Vector2f(amount, 0.f), Direction::Right, ghost))
			flags[2] = 3;
		else if (ghost.getPreviousDirection() != Direction::Right && checkDirection(sf::Vector2f(-amount, 0.f), Direction::Left, ghost))
			flags[3] = 4;

		int possible_number = 0, i;
		for (i = 0; i < 4; i++)
			if (flags[i]) possible_number++;

		srand((unsigned)std::time(NULL));
		int random = rand();

		if (possible_number) {
			int num = random % possible_number;
			num += 1;

			int chosen = 0;
			int j = 0;
			for (i = 0; i < 4; i++) {
				if (flags[i]) j++;
				if (j == num) {
					chosen = flags[i];
					break;
				}
			}

			switch (chosen)
			{
			case 1:
				new_direction = Direction::Down;
				min_direction = sf::Vector2f(0.f, amount);
				break;
			case 2:
				new_direction = Direction::Up;
				min_direction = sf::Vector2f(0.f, -amount);
				break;
			case 3:
				new_direction = Direction::Right;
				min_direction = sf::Vector2f(amount, 0.f);
				break;
			case 4:
				new_direction = Direction::Left;
				min_direction = sf::Vector2f(-amount, 0.f);
				break;
			}
		}

	}
	

	direction = new_direction;
	pos += min_direction;
	
	
	float a = pos.x / 32, b = pos.y / 32;
	sf::FloatRect q(pos.x, pos.y, 32, 32);

	sf::FloatRect p1(floor(a) * 32, floor(b) * 32, 32, 32);
	int x1 = floor(a), y1 = floor(b);
	sf::FloatRect p2(floor(a) * 32, ceil(b) * 32, 32, 32);
	int x2 = floor(a), y2 = ceil(b);
	sf::FloatRect p3(ceil(a) * 32, floor(b) * 32, 32, 32);
	int x3 = ceil(a), y3 = floor(b);
	sf::FloatRect p4(ceil(a) * 32, ceil(b) * 32, 32, 32);
	int x4 = ceil(a), y4 = ceil(b);

	if (ghost.insideOfBox() && ghost.getPreviousDirection() == Direction::Right) {
		if (gameBoard[y1 - 4][x1] != '#' && ((floor(a) * 32 + 32 - pos.x) / 32.f) > 0.95f) {
			ghost.setDirection(Direction::Up);
			ghost.setPosition(sf::Vector2f(x1 * 32, y1 * 32));
			return true;
		}
		else if (gameBoard[y3 - 4][x3] != '#' && ((((pos.x + 32) - ceil(a) * 32) / 32.f) > 0.95f)) {
			ghost.setDirection(Direction::Up);
			ghost.setPosition(sf::Vector2f(x3 * 32, y3 * 32));
			return true;
		}
	}

	if (ghost.getType() == GhostType::Inky && ghost.insideOfBox()) {
		ghost.setDirection(Direction::Up);
		ghost.setPosition(ghost.getPosition() + sf::Vector2f(0, -amount));
		return true;
	}

	// ovaj -4 ovisi o offsetu
	if (ghost.getPreviousDirection() == Direction::Up || ghost.getPreviousDirection() == Direction::Down) {
		sf::Vector2f ak = sf::Vector2f(pos.x + 3*min_direction.x, floor(b) * 32);
		if (direction == Direction::Right && gameBoard[y3 - 4][x3] != '#' && ((floor(b) * 32 + 32 - pos.y) / 32.f) > 0.9f) {
			ghost.setPosition(ak);
			ghost.setDirection(direction);
			gameBoard[8][10] = 'D';
			return true;
		}
		else if (direction == Direction::Left && gameBoard[y1 - 4][x1] != '#' && ((floor(b) * 32 + 32 - pos.y) / 32.f) > 0.9f) {
			ghost.setPosition(ak);
			ghost.setDirection(direction);
			gameBoard[8][10] = 'D';
			return true;
		}

		sf::Vector2f ak1 = sf::Vector2f(pos.x + 3 * min_direction.x, ceil(b) * 32);
		if (direction == Direction::Right && gameBoard[y4 - 4][x4] != '#' && ((pos.y + 32.f - ceil(b) * 32) / 32.f) > 0.9f) {
			ghost.setPosition(ak1);
			ghost.setDirection(direction);
			gameBoard[8][10] = 'D';
			return true;
		}
		else if (direction == Direction::Left && gameBoard[y2 - 4][x2] != '#' && ((pos.y + 32.f - ceil(b) * 32) / 32.f) > 0.9f) {
			ghost.setPosition(ak1);
			ghost.setDirection(direction);
			gameBoard[8][10] = 'D';
			return true;
		}


	}
	else if (ghost.getPreviousDirection() == Direction::Left || ghost.getPreviousDirection() == Direction::Right) {
		sf::Vector2f ak2 = sf::Vector2f(floor(a) * 32, pos.y + 3 * min_direction.y);
		if (direction == Direction::Up && gameBoard[y1 - 4][x1] != '#' && ((floor(a) * 32 + 32 - pos.x) / 32.f) > 0.9f) {
			ghost.setPosition(ak2);
			ghost.setDirection(direction);
			gameBoard[8][10] = 'D';
			return true;
		}
		else if (direction == Direction::Down && gameBoard[y2 - 4][x2] != '#' && ((floor(a) * 32 + 32 - pos.x) / 32.f) > 0.9f) {
			ghost.setPosition(ak2);
			ghost.setDirection(direction);
			gameBoard[8][10] = 'D';
			return true;
		}

		sf::Vector2f ak3 = sf::Vector2f(ceil(a) * 32, pos.y + 3 * min_direction.y);
		if (direction == Direction::Up && gameBoard[y3 - 4][x3] != '#' && ((pos.x + 32.f - ceil(a) * 32) / 32.f) > 0.9f) {
			ghost.setPosition(ak3);
			ghost.setDirection(direction);
			gameBoard[8][10] = 'D';
			return true;
		}
		else if (direction == Direction::Down && gameBoard[y4 - 4][x4] != '#' && ((pos.x + 32.f - ceil(a) * 32) / 32.f) > 0.9f) {
			ghost.setPosition(ak3);
			ghost.setDirection(direction);
			gameBoard[8][10] = 'D';
			return true;
		}
		
	}

	if ((q.intersects(p1) && gameBoard[y1 - 4][x1] == '#') || (q.intersects(p2) && gameBoard[y2 - 4][x2] == '#') || (q.intersects(p3) && gameBoard[y3 - 4][x3] == '#') || (q.intersects(p4) && gameBoard[y4 - 4][x4] == '#'))
	{
		if (direction == Direction::Right && ((q.intersects(p3) && gameBoard[y3 - 4][x3] == '#') || (q.intersects(p4) && gameBoard[y4 - 4][x4] == '#'))) {
			sf::Vector2f ak = sf::Vector2f(floor(a) * 32, pos.y);
			ghost.setPosition(ak);
		}
		else if (direction == Direction::Up && ((q.intersects(p1) && gameBoard[y1 - 4][x1] == '#') || (q.intersects(p3) && gameBoard[y3 - 4][x3] == '#'))) {
			sf::Vector2f ak = sf::Vector2f(pos.x, floor(b) * 32 + 32);
			ghost.setPosition(ak);
		}
		else if (direction == Direction::Down && ((q.intersects(p2) && gameBoard[y2 - 4][x2] == '#') || (q.intersects(p4) && gameBoard[y4 - 4][x4] == '#'))) {
			sf::Vector2f ak = sf::Vector2f(pos.x, ceil(b) * 32 - 32);
			ghost.setPosition(ak);
		}
		else if (direction == Direction::Left && ((q.intersects(p1) && gameBoard[y1 - 4][x1] == '#') || (q.intersects(p2) && gameBoard[y2 - 4][x2] == '#'))) {
			sf::Vector2f ak = sf::Vector2f(floor(a) * 32 + 32, pos.y);
			ghost.setPosition(ak);
		}

		gameBoard[8][10] = 'D';
		return false;
	}

	ghost.setPosition(pos);
	ghost.setDirection(direction);
	gameBoard[8][10] = 'D';
	return true;
}

bool Game::checkDirectionPacman(sf::Time time1, Direction direction) {
	float amount = time1.asSeconds() * pacman.getSpeed();
	sf::Vector2f pos = pacman.getPosition();

	sf::Vector2f direct;

	switch (direction)
	{
	case Direction::None:
		direct = sf::Vector2f(0.0f, 0.0f);
		break;
	case Direction::Up:
		direct = sf::Vector2f(0.0f, -amount);
		break;
	case Direction::Down:
		direct = sf::Vector2f(0.0f, amount);
		break;
	case Direction::Left:
		direct = sf::Vector2f(-amount, 0.0f);
		break;
	case Direction::Right:
		direct = sf::Vector2f(amount, 0.0f);
		break;
	}

	pos += direct;

	float a = pos.x / 32, b = pos.y / 32;
	sf::FloatRect q(pos.x, pos.y, 32, 32);

	sf::FloatRect p1(floor(a) * 32, floor(b) * 32, 32, 32);
	int x1 = floor(a), y1 = floor(b);
	sf::FloatRect p2(floor(a) * 32, ceil(b) * 32, 32, 32);
	int x2 = floor(a), y2 = ceil(b);
	sf::FloatRect p3(ceil(a) * 32, floor(b) * 32, 32, 32);
	int x3 = ceil(a), y3 = floor(b);
	sf::FloatRect p4(ceil(a) * 32, ceil(b) * 32, 32, 32);
	int x4 = ceil(a), y4 = ceil(b);

	if (pacman.getPreviousDirection() == Direction::Up || pacman.getPreviousDirection() == Direction::Down || pacman.getPreviousDirection() == Direction::None) {
		if (direction == Direction::Right && ((gameBoard[y3 - 4][x3] != '#' && ((floor(b) * 32 + 32 - pos.y) / 32.f) > 0.8f) ||
			(gameBoard[y4 - 4][x4] != '#' && ((pos.y + 32.f - ceil(b) * 32) / 32.f) > 0.8f))) {
			return true;
		}
		else if (direction == Direction::Left && ((gameBoard[y1 - 4][x1] != '#' && ((floor(b) * 32 + 32 - pos.y) / 32.f) > 0.8f)
			|| (gameBoard[y2 - 4][x2] != '#' && ((pos.y + 32.f - ceil(b) * 32) / 32.f) > 0.8f))) {
			return true;
		}
	}
	else if (pacman.getPreviousDirection() == Direction::Left || pacman.getPreviousDirection() == Direction::Right || pacman.getPreviousDirection() == Direction::None) {
		if (direction == Direction::Up && ((gameBoard[y1 - 4][x1] != '#' && ((floor(a) * 32 + 32 - pos.x) / 32.f) > 0.8f)
			|| (gameBoard[y3 - 4][x3] != '#' && ((pos.x + 32.f - ceil(a) * 32) / 32.f) > 0.8f))) {
			return true;
		}
		else if (direction == Direction::Down && ((gameBoard[y2 - 4][x2] != '#' && ((floor(a) * 32 + 32 - pos.x) / 32.f) > 0.8f) ||
			(gameBoard[y4 - 4][x4] != '#' && ((pos.x + 32.f - ceil(a) * 32) / 32.f) > 0.8f))) {
			return true;
		}
	}

	if ((q.intersects(p1) && gameBoard[y1 - 4][x1] == '#') || (q.intersects(p2) && gameBoard[y2 - 4][x2] == '#') || (q.intersects(p3) && gameBoard[y3 - 4][x3] == '#') || (q.intersects(p4) && gameBoard[y4 - 4][x4] == '#'))
	{
		if (direction == Direction::Right && ((q.intersects(p3) && gameBoard[y3 - 4][x3] == '#') || (q.intersects(p4) && gameBoard[y4 - 4][x4] == '#'))) {
			sf::Vector2f ak = sf::Vector2f(floor(a) * 32, pos.y);
			pacman.setPosition(ak);
		}
		else if (direction == Direction::Up && ((q.intersects(p1) && gameBoard[y1 - 4][x1] == '#') || (q.intersects(p3) && gameBoard[y3 - 4][x3] == '#'))) {
			sf::Vector2f ak = sf::Vector2f(pos.x, floor(b) * 32 + 32);
			pacman.setPosition(ak);
		}
		else if (direction == Direction::Down && ((q.intersects(p2) && gameBoard[y2 - 4][x2] == '#') || (q.intersects(p4) && gameBoard[y4 - 4][x4] == '#'))) {
			sf::Vector2f ak = sf::Vector2f(pos.x, ceil(b) * 32 - 32);
			pacman.setPosition(ak);
		}
		else if (direction == Direction::Left && ((q.intersects(p1) && gameBoard[y1 - 4][x1] == '#') || (q.intersects(p2) && gameBoard[y2 - 4][x2] == '#'))) {
			sf::Vector2f ak = sf::Vector2f(floor(a) * 32 + 32, pos.y);
			pacman.setPosition(ak);
		}
		
		return false;
	}

	return true;
}

bool Game::checkWalls(sf::Time time1) {
	float amount = time1.asSeconds() * pacman.getSpeed();
	sf::Vector2f pos = pacman.getPosition();

	//sljedeca cetiri if-a vezana uz to da se pacman pravilno vrati nakon sto je otisao kroz lijevi, odnosno desni tunel
	if (pos.x <= -32.f && pacman.getDirection() == Direction::Left) {
		pacman.setPosition(sf::Vector2f(672, 416));
		return true;
	} 
	else if (pos.x > 640 && pos.x <= 672 && pacman.getDirection() == Direction::Left) {
		pacman.setPosition(pos + sf::Vector2f(-amount, 0));
		return true;
	}
	else if (pos.x > 673 && pacman.getDirection() == Direction::Right) {
		pacman.setPosition(sf::Vector2f(-32.f, 416));
		return true;
	}
	else if (pos.x >= -32.f && pos.x < 0.f && pacman.getDirection() == Direction::Right) {
		pacman.setPosition(pos + sf::Vector2f(amount, 0));
		return true;
	}

	sf::Vector2f direct;
	Direction direction = pacman.getDirection();

	switch (direction)
	{
	case Direction::None:
		direct = sf::Vector2f(0.0f, 0.0f);
		break;
	case Direction::Up:
		direct = sf::Vector2f(0.0f, -amount);
		break;
	case Direction::Down:
		direct = sf::Vector2f(0.0f, amount);
		break;
	case Direction::Left:
		direct = sf::Vector2f(-amount, 0.0f);
		break;
	case Direction::Right:
		direct = sf::Vector2f(amount, 0.0f);
		break;
	}

	pos += direct;

	//sljedeca dva if-a vezana uz to da pacman moze kroz tunel doci s lijeve na desnu stranu i obratno
	if (pacman.getPosition().x <= 1 && pacman.getPosition().y == 416 && direction == Direction::Left) {
		pacman.setPosition(pos);
		pacman.setDirection(Direction::Left);
		return true;
	}
	else if (pacman.getPosition().x >= 639 && pacman.getPosition().y == 416 && direction == Direction::Right) {
		pacman.setPosition(pos);
		pacman.setDirection(Direction::Right);
		return true;
	}

	
	float a = pos.x / 32, b = pos.y / 32;
	sf::FloatRect q(pos.x, pos.y, 32, 32);

	sf::FloatRect p1(floor(a) * 32, floor(b) * 32, 32, 32);
	int x1 = floor(a), y1 = floor(b);
	sf::FloatRect p2(floor(a) * 32, ceil(b) * 32, 32, 32);
	int x2 = floor(a), y2 = ceil(b);
	sf::FloatRect p3(ceil(a) * 32, floor(b) * 32, 32, 32);
	int x3 = ceil(a), y3 = floor(b);
	sf::FloatRect p4(ceil(a) * 32, ceil(b) * 32, 32, 32);
	int x4 = ceil(a), y4 = ceil(b);
	
	// ovaj -4 ovisi o offsetu
	if (pacman.getPreviousDirection() == Direction::Up || pacman.getPreviousDirection() == Direction::Down) {
		sf::Vector2f ak = sf::Vector2f(pos.x, floor(b) * 32);
		if (direction == Direction::Right && gameBoard[y3 - 4][x3] != '#' && ((floor(b) * 32 + 32 - pos.y) / 32.f) > 0.9f) {
			pacman.setPosition(ak);
			return true;
		}
		else if (direction == Direction::Left && gameBoard[y1 - 4][x1] != '#' && ((floor(b) * 32 + 32 - pos.y) / 32.f) > 0.9f) {
			pacman.setPosition(ak);
			return true;
		}
	}
	else if (pacman.getPreviousDirection() == Direction::Left || pacman.getPreviousDirection() == Direction::Right) {
		sf::Vector2f ak2 = sf::Vector2f(floor(a) * 32, pos.y);
		if (direction == Direction::Up && gameBoard[y1 - 4][x1] != '#' && ((floor(a) * 32 + 32 - pos.x) / 32.f) > 0.9f) {
			pacman.setPosition(ak2);
			return true;
		}
		else if (direction == Direction::Down && gameBoard[y2 - 4][x2] != '#' && ((floor(a) * 32 + 32 - pos.x) / 32.f) > 0.9f) {
			pacman.setPosition(ak2);
			return true;
		}
	}

	if ((q.intersects(p1) && gameBoard[y1 - 4][x1] == '#') || (q.intersects(p2) && gameBoard[y2 - 4][x2] == '#') || (q.intersects(p3) && gameBoard[y3 - 4][x3] == '#') || (q.intersects(p4) && gameBoard[y4 - 4][x4] == '#'))
	{	
		if (direction == Direction::Right && ((q.intersects(p3) && gameBoard[y3 - 4][x3] == '#') || (q.intersects(p4) && gameBoard[y4 - 4][x4] == '#'))) {
			sf::Vector2f ak = sf::Vector2f(floor(a) * 32, pos.y);
			pacman.setPosition(ak);
		}
		else if (direction == Direction::Up && ((q.intersects(p1) && gameBoard[y1 - 4][x1] == '#') || (q.intersects(p3) && gameBoard[y3 - 4][x3] == '#'))) {
			sf::Vector2f ak = sf::Vector2f(pos.x, floor(b) * 32 + 32);
			pacman.setPosition(ak);
		}
		else if (direction == Direction::Down && ((q.intersects(p2) && gameBoard[y2 - 4][x2] == '#') || (q.intersects(p4) && gameBoard[y4 - 4][x4] == '#'))) {
			sf::Vector2f ak = sf::Vector2f(pos.x, ceil(b) * 32 - 32);
			pacman.setPosition(ak);
		}
		else if (direction == Direction::Left && ((q.intersects(p1) && gameBoard[y1 - 4][x1] == '#') || (q.intersects(p2) && gameBoard[y2 - 4][x2] == '#'))) {
			sf::Vector2f ak = sf::Vector2f(floor(a) * 32 + 32, pos.y);
			pacman.setPosition(ak);
		}

		if (pacman.getPreviousDirection() == Direction::Left && direction == Direction::Left || pacman.getPreviousDirection() == Direction::Right && direction == Direction::Right ||
			pacman.getPreviousDirection() == Direction::Up && direction == Direction::Up || pacman.getPreviousDirection() == Direction::Down && direction == Direction::Down)
			pacman.setDirection(Direction::None);
		else if (checkDirectionPacman(time1, pacman.getPreviousDirection())) pacman.setDirection(pacman.getPreviousDirection());
		else {
			pacman.setDirection(Direction::None);
		}

		return false;
	}
	
	pacman.setPosition(pos);
	return true;
}

void Game::playerScoreUpdate() {
	sf::Vector2f pos = pacman.getPosition();
	sf::FloatRect p(pos.x, pos.y, 32, 32);
	sf::FloatRect q(pos.x - 32, pos.y, 32, 32);
	float a = pos.x / 32, b = pos.y / 32;
	int x1 = floor(a), y1 = floor(b);
	int x2 = floor(a), y2 = ceil(b);
	int x3 = ceil(a), y3 = floor(b);
	int x4 = ceil(a), y4 = ceil(b);

	if (p.contains(x1 * 32 + 16, y1 * 32 + 16) && gameBoard[y1 - 4][x1] == '.') { //ovaj 4 ovisi o offsetu
		gameBoard[y1 - 4][x1] = ' ';
		playerScore += dotValue;
		dotsNumber--;
	}
	else if (p.contains(x2 * 32 + 16, y2 * 32 + 16) && gameBoard[y2 - 4][x2] == '.') {
		gameBoard[y2 - 4][x2] = ' ';
		playerScore += dotValue;
		dotsNumber--;
	}
	else if (p.contains(x3 * 32 + 16, y3 * 32 + 16) && gameBoard[y3 - 4][x3] == '.') {
		playerScore += dotValue;
		gameBoard[y3 - 4][x3] = ' ';
		dotsNumber--;
	}
	else if (p.contains(x4 * 32 + 16, y4 * 32 + 16 ) && gameBoard[y4 - 4][x4] == '.') {
		gameBoard[y4 - 4][x4] = ' ';
		playerScore += dotValue;
		dotsNumber--;
	}

	if ((146 - dotsNumber) > 0 && (146 - dotsNumber) % 30 == 0) 
		bonusSet = true;

	if ((p.contains(10 * 32, 11 * 32 + 128) || p.contains(11 * 32, 11 * 32 + 128)) && bonusSet) {
		bonusSet = false;
		playerScore += bonusValue;
		eatFruit.play();
	}

	if (playerScore > highestScore)
		highestScore = playerScore;
}

void Game::ghostsReset() {
	blinky.setPosition(sf::Vector2f(10 * 32 + 0, 7 * 32 + 128));
	blinky.setDirection(Direction::Right);
	blinky.setPreviousDirection(Direction::None);
	blinky.setPossible(true);
	blinky.move(sf::microseconds(000));

	pinky.setPosition(sf::Vector2f(9 * 32 + 0, 9 * 32 + 128));
	pinky.setDirection(Direction::None);
	pinky.setPreviousDirection(Direction::None);
	pinky.setPossible(true);
	pinky.setDesiredPosition(sf::Vector2f(10 * 32 + 0, 7 * 32 + 128));
	pinky.setInsideOfBox(true);
	pinky.move(sf::microseconds(000));

	clyde.setPosition(sf::Vector2f(11 * 32 + 0, 9 * 32 + 128));
	clyde.setDirection(Direction::None);
	clyde.setPreviousDirection(Direction::None);
	clyde.setPossible(true);
	clyde.setDesiredPosition(sf::Vector2f(10 * 32 + 0, 7 * 32 + 128));
	clyde.setInsideOfBox(true);
	clyde.move(sf::microseconds(000));

	inky.setPosition(sf::Vector2f(10 * 32 + 0, 9 * 32 + 128));
	inky.setDirection(Direction::None);
	inky.setPreviousDirection(Direction::None);
	inky.setPossible(true);
	inky.setDesiredPosition(sf::Vector2f(10 * 32 + 0, 7 * 32 + 128));
	inky.setInsideOfBox(true);
	inky.move(sf::microseconds(000));
}

float vectorDistance(sf::Vector2f a, sf::Vector2f b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

sf::Vector2f Game::pinkyDesiredPosition() {
	if (pinky.insideOfBox())
		return sf::Vector2f(10 * 32 + 0, 7 * 32 + 128);
	
	sf::Vector2f desiredPosition = pacman.getPosition();
	Direction pacmanDir = pacman.getDirection();
	switch (pacmanDir)
	{
	case Direction::None:
		break;
	case Direction::Up:
		desiredPosition.x -= 4 * 32.f;
		desiredPosition.y -= 4 * 32.f;
		break;
	case Direction::Down:
		desiredPosition.y += 4 * 32.f;
		break;
	case Direction::Left:
		desiredPosition.x -= 4 * 32.f;
		break;
	case Direction::Right:
		desiredPosition.x += 4 * 32.f;
		break;
	default:
		break;
	}
	return desiredPosition;
}

sf::Vector2f Game::clydeDesiredPosition() {
	if (clyde.insideOfBox()) 
		return sf::Vector2f(10 * 32 + 0, 7 * 32 + 128);
	
	sf::Vector2f desiredPosition1 = pacman.getPosition();
	sf::Vector2f desiredPosition2 = sf::Vector2f(1 * 32, 25 * 32); 
	float pacman_clydeDistance = vectorDistance(desiredPosition1, clyde.getPosition());
	if (pacman_clydeDistance > 8 * blockSize)
		return desiredPosition1;

	return desiredPosition2;
}

sf::Vector2f Game::inkyDesiredPosition() {
	if (clyde.insideOfBox())
		return sf::Vector2f(10 * 32 + 0, 7 * 32 + 128);
	sf::Vector2f desiredPosition = pacman.getPosition();
	sf::Vector2f blinkyPosition = blinky.getPosition();
	Direction pacmanDir = pacman.getDirection();
	switch (pacmanDir)
	{
	case Direction::None:
		break;
	case Direction::Up:
		desiredPosition.x -= 2 * 32.f;
		desiredPosition.y -= 2 * 32.f;
		break;
	case Direction::Down:
		desiredPosition.y += 2 * 32.f;
		break;
	case Direction::Left:
		desiredPosition.x -= 2 * 32.f;
		break;
	case Direction::Right:
		desiredPosition.x += 2 * 32.f;
		break;
	default:
		break;
	}

	// sada jos na desiredPosition updateamo u ovisnosti o poziciji blinkyja

	float x_diff = blinkyPosition.x - desiredPosition.x;
	desiredPosition.x -= x_diff;
	float y_diff = blinkyPosition.y - desiredPosition.y;
	desiredPosition.y -= y_diff;
	
	return desiredPosition;
}

bool Game::outsideOfBox(Ghost& ghost) {
	sf::FloatRect aboveDoors(10 * 32 + 0, 7 * 32 + 128, 32, 32);
	sf::Vector2f pos = ghost.getPosition();
	sf::FloatRect ghostRectangle(pos.x, pos.y, 32, 32);
	sf::FloatRect intersection;
	if (aboveDoors.intersects(ghostRectangle, intersection)) {
		float area = 32 * 32.f;
		float intersectionArea = intersection.width * intersection.height;
		if (intersectionArea >= 0.8f * area) {
			GhostType type = ghost.getType();
			
			switch (type)
			{
			case GhostType::Clyde:
				ghost.setDirection(Direction::Left);
				break;
			case GhostType::Blinky:
				ghost.setDirection(Direction::Right);
				break;
			case GhostType::Pinky:
				ghost.setDirection(Direction::Right);
				break;
			case GhostType::Inky:
				ghost.setDirection(Direction::Left);
				break;
			default:
				break;
			}

			ghost.setPosition(sf::Vector2f(10 * 32 + 0, 7 * 32 + 128));
			ghost.setPossible(true);
			ghost.move(sf::milliseconds(2000));
			return true;
		}
	}
	return false;
}

void Game::checkEnergyDot() {
	sf::Vector2f pos = pacman.getPosition();
	sf::FloatRect p(pos.x, pos.y, 32, 32);
	sf::FloatRect q(pos.x - 32, pos.y, 32, 32);
	float a = pos.x / 32, b = pos.y / 32;
	int x1 = floor(a), y1 = floor(b);
	int x2 = floor(a), y2 = ceil(b);
	int x3 = ceil(a), y3 = floor(b);
	int x4 = ceil(a), y4 = ceil(b);

	bool flag = false;

	// ako pojede energetsku lopticu onda postavlja ghostMode = false, tj. duhovi vise ne mogu pojesti PacMan-a
	
	// unutar ovih if-ova ide promjena ponasanja duhova da bjeze od pacmana
	if (p.contains(x1 * 32 + 16, y1 * 32 + 16) && gameBoard[y1 - 4][x1] == 'o') {
		ghostsMode = false;
		energyTime = sf::seconds(0);
		gameBoard[y1 - 4][x1] = ' ';
		flag = true;
	}
	else if (p.contains(x2 * 32 + 16, y2 * 32 + 16) && gameBoard[y2 - 4][x2] == 'o') {
		ghostsMode = false;
		energyTime = sf::seconds(0);
		gameBoard[y2 - 4][x2] = ' ';
		flag = true;
	}
	else if (p.contains(x3 * 32 + 16, y3 * 32 + 16) && gameBoard[y3 - 4][x3] == 'o') {
		ghostsMode = false;
		energyTime = sf::seconds(0);
		gameBoard[y3 - 4][x3] = ' ';
		flag = true;
	}
	else if (p.contains(x4 * 32 + 16, y4 * 32 + 16) && gameBoard[y4 - 4][x4] == 'o') {
		ghostsMode = false;
		energyTime = sf::seconds(0);
		gameBoard[y4 - 4][x4] = ' ';
		flag = true;
	}

	// kada pojedemo energetsku loptu, onda mijenjamo skinove duhova
	if (flag) {
		blinky.setAttackMode(false);
		pinky.setAttackMode(false);
		inky.setAttackMode(false);
		clyde.setAttackMode(false);

		blinky.setAlmostAttack(false);
		pinky.setAlmostAttack(false);
		inky.setAlmostAttack(false);
		clyde.setAlmostAttack(false);
		powerPellet.setLoop(true);

		powerPellet.play();
	}
}

void Game::update() {
	if (gameStarted == false) return;

	sf::Time time1 = elapsedTime();

	if (energyTime >= sf::seconds(7) && energyTime < sf::seconds(10)) {
		blinky.setAlmostAttack(true);
		pinky.setAlmostAttack(true);
		inky.setAlmostAttack(true);
		clyde.setAlmostAttack(true);
	}

	if (energyTime >= sf::seconds(10)) { // jednom kada pacman pojede energetsku lopticu, onda ima 10 sekundi da pojede duhove. Jos treba dodati da se ponasanje duhova vrati na staro.
		ghostsMode = true;
		blinky.setAttackMode(true);
		pinky.setAttackMode(true);
		inky.setAttackMode(true);
		clyde.setAttackMode(true);

		blinky.setAlmostAttack(false);
		pinky.setAlmostAttack(false);
		inky.setAlmostAttack(false);
		clyde.setAlmostAttack(false);

		powerPellet.stop();
	}

	pacman.setPossible(checkWalls(time1));
	pacman.move(time1);

	blinky.setDesiredPosition(pacman.getPosition());
	blinky.setPossible(checkWallsGhost(time1, blinky));
	blinky.move(time1);

	//kretanje duhova. Na pocetku redom izlaze: inky, pinky, clyde
	inky.setDesiredPosition(inkyDesiredPosition());
	inky.setPossible(checkWallsGhost(time1, inky));
	inky.move(time1);
	if (inky.insideOfBox() && outsideOfBox(inky)) {
		inky.setInsideOfBox(false);
	}

	pinky.setDesiredPosition(pinkyDesiredPosition());
	//pricekaj da inky izadje
	if (inky.insideOfBox() == false || (inky.insideOfBox() == true && ghostsEaten)) {
		pinky.setPossible(checkWallsGhost(time1, pinky));
		pinky.move(time1);
		if (pinky.insideOfBox() && outsideOfBox(pinky)) {
			pinky.setInsideOfBox(false);
		}
	}

	clyde.setDesiredPosition(clydeDesiredPosition());
	// pricekaj da pinky izadje
	if (pinky.insideOfBox() == false || (pinky.insideOfBox() == true && ghostsEaten)) {
		clyde.setPossible(checkWallsGhost(time1, clyde));
		clyde.move(time1);
		if (clyde.insideOfBox() && outsideOfBox(clyde)) {
			clyde.setInsideOfBox(false);
		}
	}

	//provjera za svaki duh ako se koristi tunelom
	if (blinky.tunnelApproached())
		blinky.setSpeed(50.f + (difficulty - 1) * 20);
	else blinky.setSpeed(100.f + (difficulty - 1) * 20);
	if (pinky.tunnelApproached()) 
		pinky.setSpeed(50.f + (difficulty - 1) * 20);
	else pinky.setSpeed(100.f + (difficulty - 1) * 20);
	if (clyde.tunnelApproached()) 
		clyde.setSpeed(50.f + (difficulty - 1) * 20);
	else clyde.setSpeed(100.f + (difficulty - 1) * 20);
	if (inky.tunnelApproached()) 
		inky.setSpeed(50.f + (difficulty - 1) * 20);
	else inky.setSpeed(100.f + (difficulty - 1) * 20);

	pacman.setSpeed(100 + (difficulty - 1) * 20);


	playerScoreUpdate();
	checkEnergyDot();

	// unutar ovog if-a idu sve radnje koje se dogadjaju nakon sto se pojede duh
	GhostType deadGhost = ghostEaten();
	if (deadGhost != GhostType::None) {
		eatGhost.play();
		ghostsEaten++;
		playerScore += 50 + 20 * ghostsEaten;

		switch (deadGhost)
		{
		case GhostType::Clyde:
			clyde.reset();
			break;
		case GhostType::Blinky:
			blinky.reset();
			break;
		case GhostType::Pinky:
			pinky.reset();
			break;
		case GhostType::Inky:
			inky.reset();
			break;
		default:
			break;
		}
	}

	if (dotsNumber == 0) {
		gameWon = true;
		return;
	}

	if (checkPacmanLife()) {
		ghostsEaten = 0;
		pacman.setPosition(sf::Vector2f(10 * 32 + 0, 15 * 32 + 128));
		pacman.move(sf::microseconds(000));
		pacman.setDirection(Direction::None);
		pacmanEaten = true;
		ghostsReset();
	}
}

void Game::render() {
	gameWindow.clear();

	drawBoard();
	drawScores();
	drawLives();
	drawDifficulty();

	if (bonusSet && pacmanEaten == false && pacman.getLives() >= 0) drawBonus();

	if (this->isPaused()) gameWindow.draw(resumeSprite);
	else gameWindow.draw(pauseSprite);

	
	blinky.draw(&gameWindow);
	inky.draw(&gameWindow);
	pinky.draw(&gameWindow);
	clyde.draw(&gameWindow);
	pacman.draw(&gameWindow);

	if (gameWon) {
		powerPellet.stop();
		drawYouWin();
	}

	if (pacmanEaten && !gameWon) {
		if (pacman.getLives() >= 0)
			drawReady();
		else {
			drawGameOver();
			gameOverSound.play();
			gameWindow.setFinished();
		}
	}

	gameWindow.display();

	if (gameStarted == false) {
		startSound.play();
		sf::sleep(sf::milliseconds(4000));
		clock.restart();
		gameStarted = true;
	}

	if (pacmanEaten && !gameWon) {
		sf::sleep(sf::milliseconds(2000));
		clockRestart();
		pacmanEaten = false;
	}
	else if (gameWon) {
		sf::sleep(sf::milliseconds(5000));
		gameWindow.setFinished();
	}

}

bool Game::isPaused() {
	return gameWindow.getPaused();
}

bool Game::isFinished() {
	return gameWindow.getFinished();
}

void Game::getHighestScore() {
	std::ifstream dat;
	std::stringstream score;
	std::string line;
	dat.open("HighestScore.txt");
	getline(dat, line);
	score << line;
	score >> highestScore;
}

sf::Time Game::elapsedTime() {
	return time;
}

void Game::clockRestart() {
	time = clock.restart();
	pacman.clockRestart();
	blinky.clockRestart();
	pinky.clockRestart();
	clyde.clockRestart();
	inky.clockRestart();

	if (ghostsMode == false) {
		energyTime += time;
	}
}

void Game::setGameBoard() {
	std::ifstream dat;
	std::string line;
	dat.open("Map.txt");
	int i, j;
	for (i = 0; i < 21; i++) {
		getline(dat, line);
		j = 0;
		for (auto it : line) {
			gameBoard[i][j] = it;
			j++;
		}
	}
}

void Game::drawBoard() {
	sf::RectangleShape p(sf::Vector2f(blockSize, blockSize));
	p.setOutlineColor(sf::Color::White);
	int i, j;
	for (i = 0; i < 21; i++) {
		for (j = 0; j < 21; j++) {
			if (gameBoard[i][j] == '#') {
				bool left = 0, right = 0, up = 0, down = 0;

				if (i < 20 && gameBoard[i + 1][j] == '#')
					down = 1;

				if (j > 0) {
					if (gameBoard[i][j - 1] == '#')
						left = 1;
				}
				else
					left = 1;

				if (j < 20) {
					if (gameBoard[i][j + 1] == '#')
						right = 1;
				}
				else
					right = 1;

				if (i > 0 && gameBoard[i - 1][j] == '#')
					up = 1;

				mapSprite.setTextureRect(sf::IntRect(16 * (down + 2 * (left + 2 * (right + 2 * up))), 0, 16, 16));
				mapSprite.setPosition(j * blockSize + mapOffset.x, i * blockSize + mapOffset.y);
				mapSprite.setScale(32.f / 16, 32.f / 16);
				gameWindow.draw(mapSprite);
			}
			
			
			if (gameBoard[i][j] == '.') {
				mapSprite.setTextureRect(sf::IntRect(0, 16, 16, 16));
				mapSprite.setPosition(j * blockSize + mapOffset.x, i * blockSize + mapOffset.y);
				mapSprite.setScale(32.f / 16, 32.f / 16);
				gameWindow.draw(mapSprite);
			}
			else if (gameBoard[i][j] == 'D') {
				mapSprite.setTextureRect(sf::IntRect(32, 16, 16, 16));
				mapSprite.setPosition(j * blockSize + mapOffset.x, i * blockSize + mapOffset.y);
				mapSprite.setScale(32.f / 16, 32.f / 16);
				gameWindow.draw(mapSprite);
			}
			else if (gameBoard[i][j] == 'o') {
				mapSprite.setTextureRect(sf::IntRect(16, 16, 16, 16));
				mapSprite.setPosition(j * blockSize + mapOffset.x, i * blockSize + mapOffset.y);
				mapSprite.setScale(32.f / 16, 32.f / 16);
				gameWindow.draw(mapSprite);
			}
		}
	}
}

void Game::drawScores() {
	font.loadFromFile("emulogic.ttf");

	pScore.setFont(font);
	pScore.setString(std::to_string(playerScore));
	pScore.setCharacterSize(30);
	pScore.setFillColor(sf::Color::White);
	pScore.setPosition(sf::Vector2f(35.f, 75.f));
	gameWindow.draw(pScore);

	
	hScore.setFont(font);
	hScore.setString("HIGH SCORE\n");
	hScore.setCharacterSize(32);
	hScore.setFillColor(sf::Color::White);

	sf::FloatRect textRect = hScore.getLocalBounds();
	hScore.setOrigin(textRect.width / 2, textRect.height / 2);
	hScore.setPosition(sf::Vector2f(gameWindow.getSize().x / 2.f, 50.f));
	gameWindow.draw(hScore);

	hScore.setString(std::to_string(highestScore));
	hScore.setCharacterSize(32);
	hScore.setFillColor(sf::Color::White);
	textRect = hScore.getLocalBounds();
	hScore.setOrigin(textRect.width / 2, textRect.height / 2);
	hScore.setPosition(sf::Vector2f(gameWindow.getSize().x / 2.f, 25 + 50.f));
	gameWindow.draw(hScore);
}

void Game::drawBonus() {
	allSprite.setTextureRect(sf::IntRect(173, 166, 16, 16));
	allSprite.setScale(32.f / 16, 32.f / 16);
	allSprite.setPosition(10 * 32 + 0, 11 * 32 + 128);
	gameWindow.draw(allSprite);
}

void Game::drawDifficulty() {
	allSprite.setTextureRect(sf::IntRect(173, 166, 16, 16));
	int i;
	for (i = 0; i < difficulty; i++) {
		allSprite.setPosition(19 * 32 - i * 32, 21 * 32 + 140);
		allSprite.setScale(40.f / 16, 40.f / 16);
		gameWindow.draw(allSprite);
	}
}

void Game::drawLives() {
	allSprite.setTextureRect(sf::IntRect(6, 25, 16, 16));
	int i;
	for (i = 0; i < pacman.getLives(); i++) {
		allSprite.setPosition((i + 1) * 32, 21 * 32 + 140);
		allSprite.setScale(40.f / 16, 40.f / 16);
		gameWindow.draw(allSprite);
	}
}

void Game::drawReady() {
	readySprite.setTextureRect(sf::IntRect(205, 4, 49, 11));
	sf::FloatRect rect = readySprite.getLocalBounds();
	readySprite.setOrigin(rect.width / 2, rect.height / 2);
	readySprite.setScale(32.f / 16, 32.f / 16);
	readySprite.setPosition(sf::Vector2f(gameWindow.getSize().x / 2.f, 11 * 32 + 145));
	gameWindow.draw(readySprite);
}

void Game::drawGameOver() {
	gameOverSprite.setTextureRect(sf::IntRect(16, 195, 81, 9));
	sf::FloatRect rect = gameOverSprite.getLocalBounds();
	gameOverSprite.setOrigin(rect.width / 2, rect.height / 2);
	gameOverSprite.setScale(32.f / 16, 32.f / 16);
	gameOverSprite.setPosition(sf::Vector2f(gameWindow.getSize().x / 2.f, 11 * 32 + 145));
	gameWindow.draw(gameOverSprite);
}

void Game::drawYouWin() {
	youWinSprite.setTextureRect(sf::IntRect(90, 94, 213, 60));
	sf::FloatRect rect = youWinSprite.getLocalBounds();
	youWinSprite.setOrigin(rect.width / 2, rect.height / 2);
	youWinSprite.setScale(32.f / 16, 32.f / 16);
	youWinSprite.setPosition(sf::Vector2f(gameWindow.getSize().x / 2.f, 11 * 32 + 145));
	gameWindow.draw(youWinSprite);
}

void Game::saveHighScore() {
	std::ofstream file("HighestScore.txt");
	if (file.is_open()) {
		file << highestScore;
		file.close();
	}
	else std::cout << "Can't save high score!" << std::endl;
}