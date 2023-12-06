#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Window {
public:
	Window(); 
	Window(const std::string&, const sf::Vector2u&);
	void clear();
	void draw(sf::Drawable&);
	void display();
	void update();
	~Window();
	int getDifficulty() {
		return difficulty;
	}
	bool getPaused() {
		return paused;
	}
	bool getFinished() {
		return finished;
	}
	void setFinished() {
		finished = true;
	}
	sf::Vector2u getSize() {
		return size;
	}
private:
	void Create();
	void Destroy();
	void Init(const std::string&, const sf::Vector2u&);
	sf::RenderWindow window;
	sf::Vector2u size;
	std::string title;
	bool finished;
	bool paused;
	int difficulty = 1;
};

void Window::update() {
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			finished = true;
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Space)
				paused = !paused;
			else if (event.key.code == sf::Keyboard::Add) {
				difficulty++;
				if (difficulty > 3)
					difficulty = 3;
			}
			else if (event.key.code == sf::Keyboard::Subtract) {
				difficulty--;
				if (difficulty == 0)
					difficulty = 1;
			}
		}

	}
}

Window::Window(const std::string& n,
	const sf::Vector2u& v) {
	Init(n, v);
}

Window::Window() {
	Init("Prozor", sf::Vector2u(640, 480));
}

Window::~Window() {
	Destroy();
}

void Window::Init(const std::string& n,
	const sf::Vector2u& v) {
	title = n;
	size = v;
	finished = false;
	paused = false;
	//window.setFramerateLimit(100);
	Create();
}

void Window::Create() {
	window.create(sf::VideoMode(size.x,
		size.y, 32), title);
}

void Window::Destroy() {
	window.close();
}

void Window::clear() {
	window.clear(sf::Color::Black);
}

void Window::draw(sf::Drawable& d) {
	window.draw(d);
}

void Window::display() {
	window.display();
}