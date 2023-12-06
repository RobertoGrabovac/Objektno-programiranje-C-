#include "Button.h"
#include <SFML/Graphics.hpp>

using namespace std;

Button::Button(std::string text, sf::Vector2f dimension) : button(dimension) {
	sf::Color gridColor(55, 111, 197);
	button.setFillColor(gridColor);
	button.setPosition(640 / 2.f - 200 / 2.f, 480 / 2.f - 50 / 2.f - 100.0f);
	button.setOutlineThickness(2.f);
	button.setOutlineColor(sf::Color::Black);
	clicked = false;
	hover = false;
	buttonText.setString(text);
	sf::Font font;
	if (!font.loadFromFile("VintageOne.ttf")) {
		cout << "Ne mogu ucitati font za pocetnu stranicu!" << endl;
		exit(1);
	}
	buttonText.setFont(font);
	buttonText.setCharacterSize(20);
	buttonText.setFillColor(sf::Color::White);
	buttonText.setPosition(button.getPosition().x + (button.getSize().x - buttonText.getGlobalBounds().width) / 2.f, button.getPosition().y + (button.getSize().y - buttonText.getGlobalBounds().height) / 2.f);
}

Button::Button() : button(sf::Vector2f(0.f,0.f)) {
	sf::Color gridColor(55, 111, 197);
	button.setFillColor(gridColor);
	button.setPosition(640 / 2.f - 200 / 2.f, 480 / 2.f - 50 / 2.f - 100.0f);
	button.setOutlineThickness(2.f);
	button.setOutlineColor(sf::Color::Black);
	clicked = false;
	hover = false;
	buttonText.setString("");
	sf::Font font;
	if (!font.loadFromFile("VintageOne.ttf")) {
		cout << "Ne mogu ucitati font za pocetnu stranicu!" << endl;
		exit(1);
	}
	buttonText.setFont(font);
	buttonText.setCharacterSize(20);
	buttonText.setFillColor(sf::Color::White);
	buttonText.setPosition(button.getPosition().x + (button.getSize().x - buttonText.getGlobalBounds().width) / 2.f, button.getPosition().y + (button.getSize().y - buttonText.getGlobalBounds().height) / 2.f);
}

void Button::move(std::string direction, float displacement) {
	sf::Vector2f displ(0.0f, 0.0f);
	if (direction == "up")
		displ = sf::Vector2f(0.0f, -displacement);
	else if (direction == "down")
		displ = sf::Vector2f(0.0f, displacement);
	else if (direction == "left")
		displ = sf::Vector2f(-displacement, 0.0f);
	else if (direction == "right")
		displ = sf::Vector2f(displacement, 0.0f);
	button.setPosition(button.getPosition() + displ);
	sf::Font font;
	if (!font.loadFromFile("VintageOne.ttf")) {
		cout << "Ne mogu ucitati font za pocetnu stranicu!" << endl;
		exit(1);
	}
	buttonText.setFont(font);
	buttonText.setPosition(button.getPosition().x + (button.getSize().x - buttonText.getGlobalBounds().width) / 2.f, button.getPosition().y + (button.getSize().y - buttonText.getGlobalBounds().height) / 2.f);
}

sf::RectangleShape Button::get() {
	return button;
}

bool Button::isClicked() {
	return clicked;
}

bool Button::isHovered() {
	return hover;
}

void Button::setClicked(bool a) {
	clicked = a;
}

void Button::setHover(bool a) {
	hover = a;
}

void Button::setColor(sf::Color col) {
	button.setFillColor(col);
}

void Button::setFrame(float thickness, sf::Color color) {
	button.setOutlineThickness(thickness);
	button.setOutlineColor(color);
}

void Button::setText(std::string text) {
	sf::Font font;
	if (!font.loadFromFile("VintageOne.ttf")) {
		cout << "Ne mogu ucitati font za pocetnu stranicu!" << endl;
		exit(1);
	}
	buttonText.setFont(font);
	buttonText.setString(text);
	buttonText.setPosition(button.getPosition().x + (button.getSize().x - buttonText.getGlobalBounds().width) / 2.f, button.getPosition().y + (button.getSize().y - buttonText.getGlobalBounds().height) / 2.f);
}

void Button::setFontSize(float a) {
	sf::Font font;
	if (!font.loadFromFile("VintageOne.ttf")) {
		cout << "Ne mogu ucitati font za pocetnu stranicu!" << endl;
		exit(1);
	}
	buttonText.setFont(font);
	buttonText.setCharacterSize(a);
	buttonText.setPosition(button.getPosition().x + (button.getSize().x - buttonText.getGlobalBounds().width) / 2.f, button.getPosition().y + (button.getSize().y - buttonText.getGlobalBounds().height) / 2.f);
}

void Button::draw(sf::RenderWindow& window) {
	window.draw(button);
	sf::Font font;
	if (!font.loadFromFile("VintageOne.ttf")) {
		cout << "Ne mogu ucitati font za pocetnu stranicu!" << endl;
		exit(1);
	}
	buttonText.setFont(font);
	window.draw(buttonText);
}

bool Button::contains(sf::Vector2f mousePosition) {
	return button.getGlobalBounds().contains(mousePosition);
}