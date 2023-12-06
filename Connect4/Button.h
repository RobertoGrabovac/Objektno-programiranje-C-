#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <tuple>
#include <limits>
#include <string>

class Button {
protected:
	sf::RectangleShape button;
	sf::Text buttonText;
	bool clicked;
	bool hover;
public:
	Button();
	Button(std::string, sf::Vector2f);
	void move(std::string, float);
	sf::RectangleShape get();
	bool isClicked();
	bool isHovered();
	void setClicked(bool);
	void setHover(bool);
	void setColor(sf::Color);
	void setFrame(float, sf::Color);
	void setText(std::string);
	void setFontSize(float);
	void draw(sf::RenderWindow &);
	bool contains(sf::Vector2f);
};

#endif 
