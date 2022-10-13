#include <iostream>
#include <conio.h>
#include <vector>
#include <string>
#include <thread>
#include <windows.h>

using namespace std::chrono_literals;

struct Board {
	int width, height;
	std::vector< std::vector <char>> f; // fields - pola gry


	Board(int _width, int _heigth) :width(_width), height(_heigth),
	f(_heigth, std::vector<char>(_width, ' ')) {

	}
	void draw() {
		for (int y = 0; y < f.size(); y++) {
			for (int x = 0; x < f[y].size(); x++) {
				std::cout << f[y][x];
			}
			std::cout << '\n';
		}
	}
};

enum DIRECTION {
	RIGHT, DOWN, LEFT, UP
};

struct Player {
	int length;
	std::string name;
	int x, y; // 3, 2

	DIRECTION dir; // aktualny kierunek (direction)
	Player() :x(0), y(0), dir(RIGHT) { }

	void move() {
		if (dir == RIGHT) x++;
		if (dir == LEFT) x--;
		if (dir == DOWN) y++;
		if (dir == UP) y--;
	}
	void move(int wid, int hei, bool& isColiding) {
		switch (dir) {
		case RIGHT:
			if (++x >= wid) isColiding = true;
			break;
		case LEFT:
			if (--x < 0) isColiding = true;
			break;
		case DOWN:
			if (++y >= hei) isColiding = true;
			break;
		case UP:
			if (--y < 0) isColiding = true;
			break;
		}
	}
};

struct Game {
	Board b;
	Player pl[1];
	bool gameOver;

	Game() : b(70, 20), gameOver(false) {}


	void clearScreen(char fill = ' ') {
		COORD tl = { 0,0 };
		CONSOLE_SCREEN_BUFFER_INFO s;
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(console, &s);
		DWORD written, cells = s.dwSize.X * s.dwSize.Y;
		FillConsoleOutputCharacter(console, fill, cells, tl, &written);
		FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
		SetConsoleCursorPosition(console, tl);
	}

	void putPlayersOnBoard() {
		auto px = pl[0].x;
		auto py = pl[0].y;
		b.f[py][px] = '1';
	}
	void mainLoop() {
		while (!gameOver) {
			putPlayersOnBoard();
			b.draw();
			std::this_thread::sleep_for(200ms);
			clearScreen();
			//pl[0].move();
			pl[0].move(b.f.size(), b.f[0].size(), gameOver);
		}

	};

};
	
int main()
{
	Game game;

	std::thread inputThread([&game]() {
		int x;
		do {
			x = _getch();
			if (x == 's') game.pl[0].dir = DOWN;
			if (x == 'd') game.pl[0].dir = RIGHT;
			if (x == 'a') game.pl[0].dir = LEFT;
			if (x == 'w') game.pl[0].dir = UP;
		} while (x != 27);
		});

	game.mainLoop();
	inputThread.join();
}