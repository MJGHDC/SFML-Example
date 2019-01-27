#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <random>
#include <time.h>

constexpr int HEIGHT = 20;
constexpr int WIDTH = 10;
constexpr int BLOCK_SIZE = 4;

enum keyboardEvent
{
	Left = -1,
	Up = 0,
	Right = 1,
};

int field[HEIGHT][WIDTH] = { 0 };

struct Point
{
	int x;
	int y;
} moveBlock[BLOCK_SIZE], fixedBlock[BLOCK_SIZE];;

int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()
{
	for (int i = 0; i < BLOCK_SIZE; ++i)
	{
		if (moveBlock[i].x < 0 || moveBlock[i].x >= WIDTH || moveBlock[i].y >= HEIGHT)
		{
			return false;
		}
		else if (field[moveBlock[i].y][moveBlock[i].x])
		{
			return false;
		}
	}

	return true;
}

int KeyboardEvent(sf::Event event)
{
	int eventFlag = 0;
	switch (event.key.code)
	{
	case sf::Keyboard::Up:
		std::cout << "Up" << std::endl;
		eventFlag = keyboardEvent::Up;
		break;
	case sf::Keyboard::Left:
		std::cout << "Left" << std::endl;
		eventFlag = keyboardEvent::Left;
		break;
	case sf::Keyboard::Right:
		std::cout << "Right" << std::endl;
		eventFlag = keyboardEvent::Right;
		break;
	default:
		break;
	}

	return eventFlag;
}

bool testFlag = true;
int testCount = 0;

int main()
{
	sf::RenderWindow window(sf::VideoMode(320, 480), "The Game!");

	sf::Texture tilesImg, backgroundImg, frameImg;
	tilesImg.loadFromFile("images/tiles.png");
	backgroundImg.loadFromFile("images/background.png");
	frameImg.loadFromFile("images/frame.png");

	sf::Sprite block(tilesImg), background(backgroundImg), frame(frameImg);

	sf::Clock clock;

	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<> rand(0, 6);

	int dx = 0;
	bool rotate = false;
	float timer = 0.0f;
	float delay = 0.3f;

	int blockSelect = rand(mersenne);
	int blockColor = rand(mersenne);

	for (int i = 0; i < BLOCK_SIZE; ++i)
	{
		moveBlock[i].x = figures[blockSelect][i] % 2;
		moveBlock[i].y = figures[blockSelect][i] / 2;
	}

	while (window.isOpen())
	{
		timer += clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyPressed:
				//blockSelect = rand(mersenne);
				blockColor = rand(mersenne);
				dx = KeyboardEvent(event);
				if (0 == dx)
				{
					rotate = true;
				}

				break;

			case sf::Event::Closed:
				std::cout << timer << std::endl;
				window.close();
				break;

			default:
				break;
			}

		}


		for (int i = 0; i < BLOCK_SIZE; ++i)
		{
			fixedBlock[i] = moveBlock[i];
			moveBlock[i].x += dx;
		}
		dx = 0;
		rotate = false;
		// draw
		window.clear(sf::Color::White);
		//for (int i = 0; i < BLOCK_SIZE; ++i)
		//{
		//	moveBlock[i].x = figures[blockSelect][i] % 2;
		//	moveBlock[i].y = figures[blockSelect][i] / 2;
		//}
		
		for (int i = 0; i < BLOCK_SIZE; ++i)
		{
			block.setTextureRect(sf::IntRect(blockColor * 18, 0, 18, 18));
			block.setPosition(static_cast<float>(moveBlock[i].x * 18), static_cast<float>(moveBlock[i].y * 18));

			block.move(28, 31);
			window.draw(block);
		}

		//window.draw(block);
		//window.draw(background);
		//window.draw(frame);
		window.display();
	}

	return 0;
}