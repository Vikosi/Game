#pragma warning(disable : 4996)
#include "GameProcess.h"
#include <iostream>
#include <cstring>
#include <conio.h>

// ��������� ������ ��� �������������� ���������� ����
const char* recordFormatStr = "%-15s  %9.4f  %4u  %7.2f  %s";

// ������� ������ ����������
void clearkeys() {
	while (_kbhit())
		_getch();
}

// �����������
// _scr     - ������, �������������� ����� �� �������
// _width   - ������ �������� ���� (x)
// _height  - ������ �������� ���� (y)
// _latency - �������� ����� ���������� ������� � �������������

Game::Game(Screen& _scr, int _width, int _height, int _latency) :
	width(_width), height(_height), latency(_latency), scr(_scr) {

	srand(static_cast<unsigned int>(time(NULL)));

	//duration_game = 0;
	//rating = 0.0;

	// ������������� ������� ������ ���������� �����
	cmd_table[0] = CmdPair(27, CMD_EXIT);   // escape key
	cmd_table[1] = CmdPair('K', CMD_LEFT);  // ������� �����
	cmd_table[2] = CmdPair('M', CMD_RIGHT); // ������� ������
	cmd_table[3] = CmdPair('H', CMD_UP);    // ������� �����
	cmd_table[4] = CmdPair('P', CMD_DOWN);  // ������� ����
}

Game::Command Game::get_command() {
	int ch;

	ch = _getch();
	if (ch == 0 || ch == 0xe0) {
		ch = _getch();
	}

	for (int i = 0; i < 5; i++) {
		if (cmd_table[i].first == ch) {
			return cmd_table[i].second;
		}
	}
	return CMD_NOCOMMAND;
}

// ���������� ��� ����������� ��������� �������.
// �����������: ���������� �� ������ �������� � ���� ����.
Coord Game::make_food() {
	Coord food;
	do {
		food.x = rand() % (width - 2) + 1;
		food.y = rand() % (height - 2) + 1;
	} while (snake.into(food));

	return food;
}

const char BORDER = '�';    // ������ ��� ��������� ����� �������� ����

void Game::draw_field() {

	scr.cls();

	for (int y = 0; y < height; y++) {
		if (y == 0 || y == height - 1) {
			for (int x = 0; x < width; x++)
				scr.pos(x, y, BORDER);
		}
		else {
			scr.pos(0, y, BORDER);
			scr.pos(width - 1, y, BORDER);
		}
	}
}

bool Game::once_more() {
	scr.pos_str(width / 2 - 8, height - 9, "������� ��� ���?");

	int ch = _getch();
	clearkeys();
	if (ch == 'N' || ch == 'n' || ch == 27)
		return false;
	return true;
}

const char FOOD = '$';      // ������ ��� ������ ���

void Game::game_loop() {


	draw_field();           // ���������� ������� ����

	snake.reset(Coord(width / 2, height / 2));     // ���������� ����: ����� 2,
													// ��������� - � �������� �������� ����,
													// ����������� - �����
	Command cmd = CMD_NOCOMMAND;
	State stt = STATE_OK;
	// delta  �������� ���������� ��������� (dx, dy) ��� ������� ����������� ���� �� ����
	Coord delta(-1, 0);                // ��������� �������� - �����
	Coord food = make_food();          // ��������� ���������� ���
	scr.pos(food.x, food.y, FOOD);      // ������� ��� �� �����

	snake.draw(scr);                    // ��������� ��������� ����


	do {

		if (_kbhit())                   // ���� � ������ ���������� ���� ����������,
			cmd = get_command();        // �� ������� �������

		// ��������� ������
		switch (cmd) {
		case CMD_LEFT:
			delta = Coord(-1, 0);
			break;
		case CMD_RIGHT:
			delta = Coord(1, 0);
			break;
		case CMD_UP:
			delta = Coord(0, -1);
			break;
		case CMD_DOWN:
			delta = Coord(0, 1);
			break;
		case CMD_EXIT:
			stt = STATE_EXIT;
		default:
			break;
		};

		Coord hd = snake.head();       // ���������� ������ ����
		hd += delta;                    // ���������� ������ ���� ����� ���������� (��������� �������)
		// ���� ������ ���� ����������� � �������� ���� ��� �� ����� ����, �� ���� �������
		if (hd.x == 0 || hd.x == width - 1 || hd.y == 0 || hd.y == height - 1 || snake.into(hd))
			stt = STATE_DIED;

		if (stt == STATE_OK) {          // ���� ���� ��� ����, ��
			snake.move(delta, scr);     // �������� ���� �� delta

			if (snake.head() == food) { // ���� ���������� ������ ���� ��������� � ����������� ���, ��
				snake.grow(food, 3);    // ��������� ����� ����
				food = make_food();     // ��������� ���������� ����� ���
				scr.pos(food.x, food.y, FOOD); // ������� ��� �� �����
			}

			Sleep(latency);             // �������� ����� ��������� ���������� �������
		}

	} while (stt == STATE_OK);          // ������, ���� ���� ����

	scr.pos_str(width / 2 - 8, 10, "���� ��������");
	clearkeys();
	_getch();
	clearkeys();
}