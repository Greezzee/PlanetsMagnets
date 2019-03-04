#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <time.h>
sf::Vector2i screenSize = sf::Vector2i(500, 500);

struct room
{
	sf::Vector2i size, pos;
};

std::vector<std::vector<bool>> generate(int diff) {
	std::vector<std::vector<bool>> out(100, std::vector<bool>(100, false));
	int numOfRooms = rand() % (2 + diff / 2) + 1 + diff / 2;
	std::vector<room> map(numOfRooms);
	for (int i = 0; i < numOfRooms; i++) {
		map[i].size.x = rand() % 40 + 5;
		map[i].size.y = rand() % 40 + 5;
		
		map[i].pos.x = rand() % (99 - map[i].size.x) + 1;
		map[i].pos.y = rand() % (99 - map[i].size.y) + 1;

		for (int i0 = map[i].pos.x; i0 < map[i].pos.x + map[i].size.x; i0++) for (int j0 = map[i].pos.y; j0 < map[i].pos.y + map[i].size.y; j0++) out[i0][j0] = true;
	}

	for (int i = 0; i < numOfRooms - 1; i++) {
		if (map[i].pos.x <= map[i + 1].pos.x && map[i].pos.x + map[i].size.x + 1 >= map[i + 1].pos.x) {
			if (map[i].pos.y <= map[i + 1].pos.y && map[i].pos.y + map[i].size.y + 1 >= map[i + 1].pos.y) continue;
			if (map[i].pos.y >= map[i + 1].pos.y && map[i].pos.y <= map[i + 1].pos.y + map[i + 1].size.y + 1) continue;
		}
		if (map[i].pos.x >= map[i + 1].pos.x && map[i].pos.x <= map[i + 1].pos.x + map[i + 1].size.x + 1) {
			if (map[i].pos.y <= map[i + 1].pos.y && map[i].pos.y + map[i].size.y + 1 >= map[i + 1].pos.y) continue;
			if (map[i].pos.y >= map[i + 1].pos.y && map[i].pos.y <= map[i + 1].pos.y + map[i + 1].size.y + 1) continue;
		}
		std::cout << "No collision!" << std::endl;



	}

	return out;
}

int main()
{
	srand(time(0));
	int difficulty = 10;

	std::vector<std::vector<bool>> field(100, std::vector<bool>(100, false));
	field = generate(difficulty);
	sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "SFMLworks");
	sf::RectangleShape square;
	square.setSize(sf::Vector2f(5, 5));
	square.setFillColor(sf::Color::White);
	sf::Clock clock;
	sf::Time timer;
	while (window.isOpen())
	{
		timer += clock.getElapsedTime();
		clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && timer.asMilliseconds() > 500) {
			std::system("CLS");
			field = generate(difficulty);
			timer = timer.Zero;
		}


		window.clear();
		for (int i = 0; i < 100; i++) for (int j = 0; j < 100; j++) if (field[i][j]) {
			square.setPosition(i * 5, j * 5);
			window.draw(square);
		}
		window.display();
	}

	return 0;
}