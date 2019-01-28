#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <random>
#include <time.h>

constexpr size_t HEIGHT = 20;
constexpr size_t WIDTH = 10;
constexpr size_t BLOCK_SIZE = 4;
constexpr float BLOCK_SPEED = 0.5f;

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
} moveBlock[BLOCK_SIZE], prevMoveBlock[BLOCK_SIZE];;

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
		if (moveBlock[i].x < 0 || moveBlock[i].x >= WIDTH || moveBlock[i].y >= HEIGHT) // 경계지점 체크
		{
			return false;
		}
		else if (field[moveBlock[i].y][moveBlock[i].x]) // 이미 놓여진 블록 체크
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
		eventFlag = 100;
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
	float delay = BLOCK_SPEED;

	int blockSelect = rand(mersenne);
	int blockColor = rand(mersenne);
	for (size_t i = 0; i < BLOCK_SIZE; ++i)
	{
		moveBlock[i].x = figures[blockSelect][i] % 2;
		moveBlock[i].y = figures[blockSelect][i] / 2;
	}

	while (true == window.isOpen())
	{
		timer += clock.restart().asSeconds();

		// 키보드 이벤트 감지
		sf::Event event;
		while (true == window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyPressed:
				dx = KeyboardEvent(event);
				if (0 == dx)
				{
					rotate = true;
				}
				else if (100 == dx)
				{
					dx = 0;
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
		if (true == sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			delay = 0.05f;
		}

		// 블럭 좌우 이동 및 경계 체크
		for (size_t i = 0; i < BLOCK_SIZE; ++i)
		{
			prevMoveBlock[i] = moveBlock[i];
			moveBlock[i].x += dx;
		}
		if (false == check())
		{
			for (size_t i = 0; i < BLOCK_SIZE; ++i)
			{
				moveBlock[i] = prevMoveBlock[i];
			}
		}

		// 회전 및 경계 체크
		if (true == rotate)
		{
			Point centerBlock = moveBlock[1]; // 회전을 시킬 중심축 위치
			for (size_t i = 0; i < BLOCK_SIZE; ++i)
			{ // 머리가 멍청해서 수학적 증명을 못하겠다.
				int x = moveBlock[i].y - centerBlock.y;
				int y = moveBlock[i].x - centerBlock.x;
				moveBlock[i].x = centerBlock.x - x;
				moveBlock[i].y = centerBlock.y + y;
			}

			if (false == check())
			{
				for (size_t i = 0; i < BLOCK_SIZE; ++i)
				{
					moveBlock[i] = prevMoveBlock[i];
				}
			}
		}

		//  블럭 아래로 이동 및 경계 체크
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++)
			{
				prevMoveBlock[i] = moveBlock[i];
				moveBlock[i].y += 1;
			}

			if (false == check())
			{
				for (int i = 0; i < 4; i++) // 아래쪽 끝에 닿았을때 혹은 블록에 닿았을때 그 정보를 기억해둔다.
				{
					field[prevMoveBlock[i].y][prevMoveBlock[i].x] = blockColor;
				}
				// 시작 지점으로 초기화
				blockColor = rand(mersenne);
				blockSelect = rand(mersenne);
				for (size_t i = 0; i < BLOCK_SIZE; ++i)
				{
					moveBlock[i].x = figures[blockSelect][i] % 2;
					moveBlock[i].y = figures[blockSelect][i] / 2;
				}
			}		

			timer = 0;
		}		

		// 블럭라인 체크 및 폭파
		int k = HEIGHT - 1;
		for (int i = k; i > 0; --i)
		{
			int count = 0;
			for (size_t j = 0; j < WIDTH; ++j)
			{
				if (field[i][j] > 0)
				{
					++count;
				}
				field[k][j] = field[i][j];
			}
			if (count < WIDTH)
			{
				--k;
			}
		}

		// 이동 정보값 상태 초기화
		dx = 0;
		rotate = false;
		delay = BLOCK_SPEED;
		
		// 그리기
		window.clear(sf::Color::White);
		window.draw(background);

		for (size_t i = 0; i < HEIGHT; ++i)
		{
			for (size_t j = 0; j < WIDTH; ++j)
			{
				if (field[i][j] == 0)
				{
					continue;
				}					
				block.setTextureRect(sf::IntRect(field[i][j] * 18, 0, 18, 18));
				block.setPosition(j * 18, i * 18);

				block.move(28, 31); 
				window.draw(block);
			}
		}
			

		for (size_t i = 0; i < BLOCK_SIZE; ++i)
		{
			block.setTextureRect(sf::IntRect(blockColor * 18, 0, 18, 18));
			block.setPosition(static_cast<float>(moveBlock[i].x * 18), static_cast<float>(moveBlock[i].y * 18));

			block.move(28, 31);
			window.draw(block);
		}

		//window.draw(block);
		//window.draw(background);
		//window.draw(frame);
		window.draw(frame);
		window.display(); // 이 함수가 호출이 되어야 최종적으로 그려진다.
		window.setFramerateLimit(60); // 프레임 설정
	}

	return 0;
}