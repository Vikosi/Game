#include "Screen.h"
#include <conio.h>

const char* msgs[] = {
	"",
	"Failed GetStdHandle(): INVALID_HANDLE_VALUE",
	"Failed GetConsoleCursorInfo()",
	"Failed SetConsoleCursorInfo()",
	"Failed SetConsoleCursorPosition()"
};

const char* CSScreenException::what() {
	return msgs[err];
}

Screen::Screen() {
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsoleOutput == INVALID_HANDLE_VALUE)
		throw CSScreenException(1);    // "INVALID_HANDLE_VALUE"

	if (!GetConsoleCursorInfo(hConsoleOutput, &oldCursorInfo))
		throw CSScreenException(2);
	curCursorInfo.dwSize = oldCursorInfo.dwSize;
	curCursorInfo.bVisible = oldCursorInfo.bVisible;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);
	oldTextAttr = csbi.wAttributes;
}

Screen::~Screen() {
	SetConsoleCursorInfo(hConsoleOutput, &oldCursorInfo);
	SetConsoleTextAttribute(hConsoleOutput, oldTextAttr);
}

void Screen::cursor_show(bool visible) {
	curCursorInfo.bVisible = visible;
	if (!SetConsoleCursorInfo(hConsoleOutput, &curCursorInfo))
		throw CSScreenException(3);
}

void Screen::text_attr(WORD attr) {
	SetConsoleTextAttribute(hConsoleOutput, attr);
}

void Screen::pos(int x, int y, char ch) {
	COORD point;
	point.X = static_cast<SHORT>(x);
	point.Y = static_cast<SHORT>(y);
	if (!SetConsoleCursorPosition(hConsoleOutput, point))
		throw CSScreenException(4);
	if (ch > 0)
		_putch(ch);
}

void Screen::pos_str(int x, int y, const char* str) {
	pos(x, y);
	_cprintf("%s", str);
}

void Screen::cls() {
	system("cls");
}