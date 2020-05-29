#include <iostream>
#include <stdio.h>
#include <conio.h>
#include "Screen.h"
#include "GameProcess.h"

int main()
{
	setlocale(LC_ALL, "Russian");

	try {
		Screen screen;
		screen.cursor_show(false);
		screen.text_attr((WORD)0x0a);
		screen.cls();
		Game game(screen, 80, 24, 120);

		do {
			game.game_loop();
		} while (game.once_more());

	}
	catch (CSScreenException& ex) {
		cerr << "*** " << ex.what() << endl;
	}

	return 0;
}