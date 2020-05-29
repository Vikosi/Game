#pragma warning(disable : 4996)
#ifndef __GAMEPROCESS_H__
#define __GAMEPROCESS_H__

#include <time.h>
#include <stdio.h>
#include <fstream>
#include <utility>

#include "Screen.h"
#include "Snake.h"
#include "Coord.h"

using namespace std;

class Game {
public:
	Game(Screen& _scr, int _width = 80, int _height = 24, int _latency = 100);
	void game_loop();           // �������� ���� ����
	bool once_more();           // ����� ������� � ���� ������ �� ������

private:
	enum Command { CMD_NOCOMMAND = 0, CMD_EXIT, CMD_LEFT, CMD_RIGHT, CMD_UP, CMD_DOWN };
	enum State { STATE_OK, STATE_EXIT, STATE_DIED };

	typedef pair<int, Command> CmdPair;

	int width, height;      // ������ � ������ �������� ����
	int latency;            // �������� ����� ���������� ������� � �������������
	Screen scr;            // ���������� ������������
	Snake snake;           // ������
	CmdPair cmd_table[5];   // ������� ������ ���������� �����

	void draw_field();      // ���������� �������� ����
	Coord make_food();     // ���������� ������� ��� ���
	Command get_command();  // ���� ������� � ����������
};

#endif 