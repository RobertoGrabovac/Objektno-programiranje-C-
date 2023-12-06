#include "Connect4.h"
#include <limits>

using namespace std;

Connect4::Connect4() : window(sf::VideoMode(640.f, 480.f), "Connect4"), button1("You vs. Friend", sf::Vector2f(200.f, 50.f)), button2("You vs. Computer", sf::Vector2f(200.f, 50.f)), backgroundColor(170, 196, 239), gridColor(55, 111, 197), current_player(1), current_game_status(-1) {
	button2.move("down", 100.0f);
	disk_position = 0;
	game_mode = 0;
	float ver_side = 480 / 7.f;
	radius = ver_side / 2;
	player_choice = sf::Vector2i(-1, -1);
	int i, j;
	for (i = 0; i < 6; i++)
		for (j = 0; j < 7; j++)
			grid[i][j] = 0;
}

Connect4::~Connect4() {
	cout << "Igra je gotova!" << endl;
}

void Connect4::createCurrentGrid(sf::CircleShape& circle) {
	int i, j;
	float ver_side = 480 / 7.f;
	float hor_side = 640 / 7.f;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) {
			sf::RectangleShape p(sf::Vector2f(hor_side, ver_side));
			p.setFillColor(gridColor);
			p.setPosition(j * hor_side, ver_side + i * ver_side);
			window.draw(p);
			sf::CircleShape q(radius - 6.f);
			q.setOrigin(radius, radius);
			q.setPosition(j * hor_side + hor_side / 2.0f + 4.f, ver_side + i * ver_side + ver_side / 2.0f + 6.0f);
			q.setOutlineThickness(-3.5f);
			q.setFillColor(backgroundColor);
			q.setOutlineColor(sf::Color::Black);

			if (grid[i][j] == 1) q.setFillColor(sf::Color::Yellow);
			else if (grid[i][j] == 2) q.setFillColor(sf::Color::Red);
			else q.setFillColor(backgroundColor);

			window.draw(q);
		}
	}
	window.draw(circle);
	window.display();

	return;
}

sf::CircleShape Connect4::showDisk(sf::Event event) {
	if (event.mouseMove.x) disk_position = (event.mouseMove.x / 640.f) * 7;
	else disk_position = (event.mouseButton.x / 640.f) * 7;
	sf::CircleShape q(radius - 6.f - 3.5f);
	q.setOrigin(radius, radius);
	float ver_side = 480 / 7.f;
	float hor_side = 640 / 7.f;
	q.setPosition(disk_position * hor_side + hor_side / 2.0f + 4.f, ver_side / 2.0f + 6.0f);
	if (current_player == 1)
		q.setFillColor(sf::Color::Yellow);
	else
		q.setFillColor(sf::Color::Red);

	return q;
}

void Connect4::animation(sf::Event event, int mouse_pos, int grid_pos) {
	for (int i = 0; i < grid_pos; i++) {
		grid[i][mouse_pos] = current_player;
		sf::CircleShape q = showDisk(event);
		if (current_player == 1)
			q.setFillColor(sf::Color::Red);
		else
			q.setFillColor(sf::Color::Yellow);

		if (game_mode == 2)
			q.setFillColor(sf::Color::Yellow);
		createCurrentGrid(q);
		sf::sleep(sf::milliseconds(35));
		grid[i][mouse_pos] = 0;
	}
}

void Connect4::updateGrid(sf::Event event) {
	//cout << player_choice.x << " " << player_choice.y << endl;
	animation(event, player_choice.x, player_choice.y);
	grid[player_choice.y][player_choice.x] = current_player;
	current_player = 3 - current_player;
	player_choice = sf::Vector2i(-1, -1);
}

sf::Event Connect4::processInput() {
	sf::Event event{};
	while (window.pollEvent(event)) {
		window.clear(backgroundColor);
		if (event.type == sf::Event::Closed)
			window.close();

		if (event.type == sf::Event::MouseMoved) {
			//sluzi za animaciju diska prije pritiska za ubacivanje u plocu
			player_disk = showDisk(event);

			//sluzi da se gumbovi istaknu kada se misem prijede preko njih
			sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			button1.contains(mousePosition) ? button1.setHover(bool(true)) : button1.setHover(bool(false));
			button2.contains(mousePosition) ? button2.setHover(bool(true)) : button2.setHover(bool(false));
		}

		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			if ((button1.isClicked() || button2.isClicked()) && current_game_status == -1 && game_mode) {
				int position = (event.mouseButton.x / 640.f) * 7, j;
				for (j = 5; j >= 0; j--)
					if (grid[j][position] == 0) {
						break;
					}
				if (j < 0) player_choice = sf::Vector2i(-1, -1);
				else player_choice = sf::Vector2i(position, j);
				return event;
			}
			//odabir vrste igre
			sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			if ((button1.isClicked() == false && button2.isClicked() == false) && button1.contains(mousePosition)) {
				button1.setClicked(bool(true));
				game_mode = 1;
			}
			else if ((button1.isClicked() == false && button2.isClicked() == false) && button2.contains(mousePosition)) {
				button2.setClicked(bool(true));
				game_mode = 2;
			}

		}
	}
	return event;
}

bool Connect4::check(int condition, int cur_row, int cur_col, int row_dir, int col_dir) {
	int player = grid[cur_row][cur_col];
	for (int count = 1; count < condition; count++) {
		cur_row += row_dir;
		cur_col += col_dir;
		if (cur_row >= 6 || cur_col >= 7 || cur_row < 0 || cur_col < 0 || player != grid[cur_row][cur_col])
			return false;
	}
	return true;
}


int Connect4::gameStatus() {
	int i, j, flag = 0;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) {
			if (grid[i][j] == 0) {
				flag = 1;
				continue;
			}
			//ako je nesto od donjeg ispunjeno, vraca igraca koji je pobijedio
			if (check(4, i, j, 1, 0))
				return grid[i][j];
			else if (check(4, i, j, 0, 1))
				return grid[i][j];
			else if (check(4, i, j, 1, 1))
				return grid[i][j];
			else if (check(4, i, j, 1, -1))
				return grid[i][j];

		}
	}
	if (flag == 0)
		return 0; //nerijeseno
	return -1; //nije gotovo
}

int Connect4::countDisks(int player, int diskNumber) {
	int i, j, number = 0;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) {
			if (grid[i][j] == player) {
				number += check(diskNumber, i, j, 1, 0);
				number += check(diskNumber, i, j, 0, 1);
				number += check(diskNumber, i, j, 1, 1);
				number += check(diskNumber, i, j, 1, -1);
			}
		}
	}
	return number;
}

int Connect4::gridEvaluation() {
	int three_disks_player = 2 * countDisks(1, 3), two_disks_player = countDisks(1, 2);  //tri spojena diska su vrijednija od dva spojena diska pa zato koef. 2
	int three_disks_computer = 2 * countDisks(2, 3), two_disks_computer = countDisks(2, 2);
	return three_disks_player + two_disks_player - (three_disks_computer + two_disks_computer);
}

int Connect4::minimax(int depth, int alpha, int beta, bool player) {
	int currentGameStatus = gameStatus();

	if (currentGameStatus != -1) {
		if (currentGameStatus == 1)
			return std::numeric_limits<int>::max();
		else if (currentGameStatus == 2)
			return -std::numeric_limits<int>::max();
		else
			return 0;
	}

	if (depth == 0)
		return gridEvaluation();

	if (player) {
		int i, j;
		int maxEval = -std::numeric_limits<int>::max(), eval;
		for (j = 0; j < 7; j++) {
			for (i = 5; i >= 0; i--)
				if (grid[i][j] == 0) break;
			if (i < 0) continue; //nema slobodne pozicije u tom retku
			grid[i][j] = 1;
			eval = minimax(depth - 1, alpha, beta, false);
			grid[i][j] = 0;
			maxEval = max(maxEval, eval);
			alpha = max(alpha, eval);
			if (beta <= alpha) break;
		}
		return maxEval;
	}
	else {
		int i, j;
		int minEval = std::numeric_limits<int>::max(), eval;
		for (j = 0; j < 7; j++) {
			for (i = 5; i >= 0; i--)
				if (grid[i][j] == 0) break;
			if (i < 0) continue;
			grid[i][j] = 2;
			eval = minimax(depth - 1, alpha, beta, true);
			grid[i][j] = 0;
			minEval = min(minEval, eval);
			beta = min(beta, eval);
			if (beta <= alpha) break;
		}
		return minEval;
	}
}

void Connect4::cumputerPlays(sf::Event event) {
	int i, j;
	int best_i = -1, best_j = -1;

	int minEval = std::numeric_limits<int>::max(), eval;
	for (j = 0; j < 7; j++) {
		for (i = 5; i >= 0; i--)
			if (grid[i][j] == 0) break;
		if (i < 0) continue;
		grid[i][j] = 2;
		eval = minimax(5, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), true);
		//cout << i << " " << j << " " << eval << endl;

		//pogledaj sto se dogodi jedan korak unaprijed. Ako odmah racunalo pobijedi, onda napravi taj potez. Inace se moze dogoditi da racunalo predvidi pobjedu ali nakon vise poteza, a onda ignorira taj jedan potez koji je dovoljan za pobjedu...
		int proba = minimax(0, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), true);
		if (proba == -std::numeric_limits<int>::max()) {
			minEval = proba;
			best_i = i;
			best_j = j;
			grid[i][j] = 0;
			break;
		}

		grid[i][j] = 0;
		if (eval < minEval) {
			minEval = eval;
			best_i = i;
			best_j = j;
		}
	}
	//cout << "odabir: " << best_i << " " << best_j << endl;
	animation(event, best_j, best_i);
	grid[best_i][best_j] = 2;
	current_player = 1;
}

void Connect4::finalResult() {
	Button button3("", sf::Vector2f(200.f, 40.f));
	button3.setFrame(0.0f, backgroundColor); button3.setFontSize(50);
	if (current_game_status == 0) button3.setText("Igra je nerijesena!");
	else (current_game_status == 1) ? button3.setText("Pobijedio je igrac 1!") : button3.setText("Pobijedio je igrac 2!");
	sf::sleep(sf::milliseconds(1500));
	window.clear(gridColor);
	button3.draw(window);
	window.display();
}

void Connect4::updateGame(sf::Event event) {
	if (button1.isClicked() == false && button2.isClicked() == false) {
		button1.isHovered() ? button1.setColor(sf::Color::Blue) : button1.setColor(gridColor);
		button2.isHovered() ? button2.setColor(sf::Color::Blue) : button2.setColor(gridColor);
		window.clear(backgroundColor);
		button1.draw(window);
		button2.draw(window);
		window.display();
	}

	if (!(event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseMoved))
		return;

	if (button1.isClicked() || button2.isClicked()) {
		window.clear(backgroundColor);
		if (current_game_status == -1) {
			//cout << player_choice.x << " " << player_choice.y << endl;
			if (player_choice != sf::Vector2i(-1, -1)) {
				updateGrid(event);
				if (game_mode == 2) cumputerPlays(event);
			}
			sf::CircleShape circle = showDisk(event);
			createCurrentGrid(circle); 
			current_game_status = gameStatus();

			if (current_game_status != -1)
				finalResult();
		}
	}
}

bool Connect4::stillPlaying() {
	return window.isOpen();
}