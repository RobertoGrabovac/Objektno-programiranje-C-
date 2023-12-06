#pragma once
#include <iostream>
#include "Window.h"
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "PacMan.h"

enum class GhostType {Clyde, Blinky, Pinky, Inky, None};

using namespace std;

class Ghost {
public:
	Ghost(GhostType);
	float distance(sf::Vector2f);
	void move(sf::Time);
	void draw(Window*);
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f);
	void setDirection(Direction);
	void setPreviousDirection(Direction);
	Direction getDirection();
	void setDesiredPosition(sf::Vector2f);
	Direction getPreviousDirection();
	float getSpeed();
	GhostType getType();
	void setSpeed(float);
	void setPossible(bool);
	bool tunnelApproached();
	void setAttackMode(bool);
	void setAlmostAttack(bool);
	void clockRestart();
	bool insideOfBox();
	void setInsideOfBox(bool);
	void reset();
	sf::Time elapsedTime();
private:
	GhostType type;
	sf::Vector2f currentPosition;
	sf::Vector2f desiredPosition;
	int currentSkin;
	sf::Texture skin;
	sf::Sprite skinSprite;
	Direction previousDirection;
	Direction direction;
	float speed;
	bool possible;
	bool box;
	bool attackMode;
	bool almostAttack;
	int whiteBlueSkin;
	sf::Clock clock;
	sf::Time time;
	sf::Time whiteSkinTime;
	void changeAnimation();
};

void Ghost::setAlmostAttack(bool f) {
	almostAttack = f;
}

void Ghost::setAttackMode(bool f) {
	attackMode = f;
}

Ghost::Ghost(GhostType t) : speed(100), direction(Direction::None), previousDirection(Direction::None), currentSkin(0), attackMode(true), almostAttack(false) {
	whiteBlueSkin = 0;
	type = t;
	skin.loadFromFile("all.png");
	skinSprite.setTexture(skin);
	switch (type) {
	case GhostType::Blinky:
		currentPosition = sf::Vector2f(10 * 32 + 0, 7 * 32 + 128);
		skinSprite.setTextureRect(sf::IntRect(6, 87, 16, 16));
		skinSprite.setScale(32.f / 16.f, 32.f / 16.f);
		skinSprite.setPosition(currentPosition);
		box = false;
		break;
	case GhostType::Pinky:
		currentPosition = sf::Vector2f(9 * 32 + 0, 9 * 32 + 128);
		skinSprite.setTextureRect(sf::IntRect(6, 106, 16, 16));
		skinSprite.setScale(32.f / 16.f, 32.f / 16.f);
		skinSprite.setPosition(currentPosition);
		box = true;
		direction = Direction::Right;
		desiredPosition = sf::Vector2f(10 * 32 + 0, 7 * 32 + 128);
		break;
	case GhostType::Clyde:
		currentPosition = sf::Vector2f(11 * 32 + 0, 9 * 32 + 128);
		skinSprite.setTextureRect(sf::IntRect(6, 146, 16, 16));
		skinSprite.setScale(32.f / 16.f, 32.f / 16.f);
		skinSprite.setPosition(currentPosition);
		box = true;
		direction = Direction::Left;
		desiredPosition = sf::Vector2f(10 * 32 + 0, 7 * 32 + 128);
		break;
	case GhostType::Inky:
		currentPosition = sf::Vector2f(10 * 32 + 0, 9 * 32 + 128);
		skinSprite.setTextureRect(sf::IntRect(6, 126, 16, 16));
		skinSprite.setScale(32.f / 16.f, 32.f / 16.f);
		skinSprite.setPosition(currentPosition);
		direction = Direction::Up;
		box = true;
		desiredPosition = sf::Vector2f(10 * 32 + 0, 7 * 32 + 128);
		break;
	}
}

void Ghost::reset() {
	switch (type)
	{
	case GhostType::Clyde:
		(*this).setPosition(sf::Vector2f(11 * 32 + 0, 9 * 32 + 128));
		(*this).setDirection(Direction::Left);
		(*this).setPreviousDirection(Direction::Left);
		(*this).setPossible(true);
		(*this).setDesiredPosition(sf::Vector2f(10 * 32 + 0, 7 * 32 + 128));
		(*this).setInsideOfBox(true);
		(*this).move(sf::microseconds(000));
		break;
	case GhostType::Blinky:
		(*this).setPosition(sf::Vector2f(10 * 32 + 0, 7 * 32 + 128));
		(*this).setDirection(Direction::Right);
		(*this).setPreviousDirection(Direction::None);
		(*this).setPossible(true);
		(*this).move(sf::microseconds(000));
		break;
	case GhostType::Pinky:
		(*this).setPosition(sf::Vector2f(9 * 32 + 0, 9 * 32 + 128));
		(*this).setDirection(Direction::Right);
		(*this).setPreviousDirection(Direction::Right);
		(*this).setPossible(true);
		(*this).setDesiredPosition(sf::Vector2f(10 * 32 + 0, 7 * 32 + 128));
		(*this).setInsideOfBox(true);
		(*this).move(sf::microseconds(000));
		break;
	case GhostType::Inky:
		(*this).setPosition(sf::Vector2f(10 * 32 + 0, 9 * 32 + 128));
		(*this).setDirection(Direction::Up);
		(*this).setPreviousDirection(Direction::Up);
		(*this).setPossible(true);
		(*this).setDesiredPosition(sf::Vector2f(10 * 32 + 0, 7 * 32 + 128));
		(*this).setInsideOfBox(true);
		(*this).move(sf::microseconds(000));
		break;
	default:
		break;
	}
}

void Ghost::move(sf::Time time1) {
	sf::Vector2f direct;
	float amount = time1.asSeconds() * speed;

	if (possible) {
		skinSprite.setPosition(currentPosition);
	}
	else {
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

		currentPosition += direct;
		skinSprite.setPosition(currentPosition);
	}
	changeAnimation();
}

void Ghost::changeAnimation() {
	if (direction != Direction::None && time.asSeconds() >= 0.1) {
		currentSkin = 1 - currentSkin;

		if (attackMode == false) {
			int a = 6 + 20 * currentSkin;
			if (almostAttack && whiteSkinTime.asSeconds() >= 0.4) {
				whiteBlueSkin = 1 - whiteBlueSkin;
				whiteSkinTime -= sf::seconds(0.4);
			}

			if (almostAttack == false)
				whiteBlueSkin = 0;

			skinSprite.setTextureRect(sf::IntRect(a + whiteBlueSkin * 40.f, 167, 16, 16));
			return;
		}

		int skinDirection;
		switch (direction)
		{
		case Direction::Up:
			skinDirection = 0;
			break;
		case Direction::Down:
			skinDirection = 1;
			break;
		case Direction::Left:
			skinDirection = 2;
			break;
		case Direction::Right:
			skinDirection = 3;
			break;
		}

		switch (type)
		{
		case GhostType::Clyde:
			skinSprite.setTextureRect(sf::IntRect(6 + 40 * skinDirection + 20 * currentSkin, 146, 16, 16));
			break;
		case GhostType::Blinky:
			skinSprite.setTextureRect(sf::IntRect(6 + 40 * skinDirection + 20 * currentSkin, 87, 16, 16));
			break;
		case GhostType::Pinky:
			skinSprite.setTextureRect(sf::IntRect(6 + 40 * skinDirection + 20 * currentSkin, 106, 16, 16));
			break;
		case GhostType::Inky:
			skinSprite.setTextureRect(sf::IntRect(6 + 40 * skinDirection + 20 * currentSkin, 126, 16, 16));
			break;
		default:
			break;
		}
		time -= sf::seconds(0.1);
	}
}

void Ghost::setSpeed(float f) {
	speed = f;
}

bool Ghost::tunnelApproached() {
	if ((currentPosition.x <= 4 * 32 && currentPosition.y == 416) || (currentPosition.x >= (640 - 4 * 32) && currentPosition.y == 416))
		return true;
	return false;
}

void Ghost::setPreviousDirection(Direction dir) {
	previousDirection = dir;
}

Direction Ghost::getDirection() {
	return direction;
}

void Ghost::setDesiredPosition(sf::Vector2f p) {
	desiredPosition = p;
}

void Ghost::setPossible(bool b) {
	possible = b;
}

void Ghost::setDirection(Direction dir) {
	direction = dir;
}

Direction Ghost::getPreviousDirection() {
	return previousDirection;
}

void Ghost::setPosition(sf::Vector2f pos) {
	currentPosition = pos;
}

float Ghost::getSpeed() {
	return speed;
}

sf::Vector2f Ghost::getPosition() {
	return currentPosition;
}

float Ghost::distance(sf::Vector2f possiblePosition) {
	return ((desiredPosition.x - possiblePosition.x) * (desiredPosition.x - possiblePosition.x) + (desiredPosition.y - possiblePosition.y) * (desiredPosition.y - possiblePosition.y));
}

void Ghost::clockRestart() {
	sf::Time t = clock.restart();
	time += t;
	if (almostAttack)
		whiteSkinTime += t;
}

sf::Time Ghost::elapsedTime() {
	return time;
}

void Ghost::draw(Window* p) {
	p->draw(skinSprite);
}

GhostType Ghost::getType() {
	return type;
}

bool Ghost::insideOfBox() {
	return box;
}

void Ghost::setInsideOfBox(bool b) {
	box = b;
}