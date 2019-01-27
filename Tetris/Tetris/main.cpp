#include <SFML/Graphics.hpp>
#include <time.h>

constexpr int HEIGHT = 20;
constexpr int WIDTH = 10;
constexpr int BLOCK_SIZE = 4;

int field[HEIGHT][WIDTH] = { 0 };

struct Point
{
	int x;
	int y;
} fixedBlock[BLOCK_SIZE], moveBlock[BLOCK_SIZE];;

int figures[7][4] =
{
	1,3,5,7,
	2,4,5,7,
	3,5,4,6,
	3,5,3,7,
	2,3,5,7,
	3,5,7,6,
	2,3,4,5,
};

bool check()
{
	for (int i = 0; i < BLOCK_SIZE; ++i)
	{
		if (fixedBlock[i].x < 0 || fixedBlock[i].x >= WIDTH || fixedBlock[i].y >= HEIGHT)
		{
			return false;
		}
		else if (field[fixedBlock[i].y][fixedBlock[i].x])
		{
			return false;
		}
	}

	return true;
}


int WinMain()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	sf::Color c(255, 0, 0, 100);
	shape.setFillColor(c);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}