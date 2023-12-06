#pragma once
#include <iostream>
#include "Window.h"
#include <SFML/Graphics.hpp>
#include "Game.h"

enum class Direction { None, Up, Down, Left, Right };

using namespace std;

class PacMan {
public:
	PacMan();
	void draw(Window*);
	void setPosition(sf::Vector2f);
	sf::Vector2f getPosition();
	void reduceLives();
	int getLives();
	void setDirection(Direction);
	Direction getDirection();
	void move(sf::Time);
	void clockRestart();
	sf::Time elapsedTime();
	void setPossible(bool);
	float getSpeed();
	void setSpeed(float);
	Direction getPreviousDirection();
	void setPreviousDirection(Direction);
private:
	Direction direction;
	Direction previousDirection;
	float speed;
	int livesNumber;
	sf::Vector2f position;
	sf::Texture skin;
	sf::Sprite skinSprite;
	int currentSkin;
	int skinDirection;
	bool possible;
	sf::Clock clock;
	sf::Time time;
	void changeAnimation();
};

void PacMan::reduceLives() {
	livesNumber--;
}

int PacMan::getLives() {
	return livesNumber;
}

void PacMan::setPreviousDirection(Direction dir) {
	previousDirection = dir;
}

Direction PacMan::getPreviousDirection() {
	return previousDirection;
}

void PacMan::setPosition(sf::Vector2f a) {
	position = a;
}

sf::Vector2f PacMan::getPosition() {
	return position;
}

float PacMan::getSpeed() {
	return speed;
}

void PacMan::setSpeed(float s) {
	speed = s;
}

void PacMan::setPossible(bool f) {
	possible = f;
}

PacMan::PacMan() {
	speed = 100;
	livesNumber = 3;
	direction = Direction::None;
	previousDirection = Direction::None;
	position = sf::Vector2f(10 * 32 + 0, 15 * 32 + 128);
	currentSkin = 0;
	skinDirection = 0;
	possible = true;

	skin.loadFromFile("pacman_skin.png");
	skinSprite.setTexture(skin);
	skinSprite.setTextureRect(sf::IntRect(17 * currentSkin, skinDirection, 16, 16));
	skinSprite.setScale(32.f / skinSprite.getLocalBounds().width, 32.f / skinSprite.getLocalBounds().height);
	skinSprite.setPosition(position);
}

void PacMan::changeAnimation() {
	if (direction == Direction::None)
		time = sf::seconds(0);
	else if (direction != Direction::None && time.asSeconds() >= 0.1) {
		currentSkin = currentSkin + 1;
		if (currentSkin == 6)
			currentSkin = 0;
		switch (direction)
		{
		case Direction::Up:
			skinDirection = 1;
			break;
		case Direction::Down:
			skinDirection = 3;
			break;
		case Direction::Left:
			skinDirection = 2;
			break;
		case Direction::Right:
			skinDirection = 0;
			break;
		}
		skinSprite.setTextureRect(sf::IntRect(16 * currentSkin, 16 * skinDirection, 16, 16));
		time -= sf::seconds(0.1);
	}
}

void PacMan::move(sf::Time time1) {
	sf::Vector2f direct;
	float amount = time1.asSeconds() * speed;

	if(possible) {
		changeAnimation();
		skinSprite.setPosition(position);
	}
	else if  (direction != Direction::None) {
		switch (getPreviousDirection())
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
		changeAnimation();
		position += direct;
		skinSprite.setPosition(position);
	}
}

void PacMan::setDirection(Direction dir) {
	direction = dir;
}

Direction PacMan::getDirection() {
	return direction;
}

void PacMan::draw(Window* p) {
	p->draw(skinSprite);
}

void PacMan::clockRestart() {
	time += clock.restart();
}

sf::Time PacMan::elapsedTime() {
	return time;
}
