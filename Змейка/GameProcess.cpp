#pragma warning(disable : 4996)
#include "GameProcess.h"
#include <iostream>
#include <cstring>
#include <conio.h>

// форматная строка для форматирования результата игры
const char* recordFormatStr = "%-15s  %9.4f  %4u  %7.2f  %s";

// очистка буфера клавиатуры
void clearkeys() {
	while (_kbhit())
		_getch();
}

// Конструктор
// _scr     - объект, осуществляющий вывод на консоль
// _width   - ширина игрового поля (x)
// _height  - высота игрового поля (y)
// _latency - задержка между изменением позиции в миллисекундах

Game::Game(Screen& _scr, int _width, int _height, int _latency) :
	width(_width), height(_height), latency(_latency), scr(_scr) {

	srand(static_cast<unsigned int>(time(NULL)));

	//duration_game = 0;
	//rating = 0.0;

	// инициализация таблицы команд управления игрой
	cmd_table[0] = CmdPair(27, CMD_EXIT);   // escape key
	cmd_table[1] = CmdPair('K', CMD_LEFT);  // стрелка влево
	cmd_table[2] = CmdPair('M', CMD_RIGHT); // стрелка вправо
	cmd_table[3] = CmdPair('H', CMD_UP);    // стрелка вверх
	cmd_table[4] = CmdPair('P', CMD_DOWN);  // стрелка вниз
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

// Координата еды вычисляется случайным образом.
// Ограничение: координата не должна попадать в тело змеи.
Coord Game::make_food() {
	Coord food;
	do {
		food.x = rand() % (width - 2) + 1;
		food.y = rand() % (height - 2) + 1;
	} while (snake.into(food));

	return food;
}

const char BORDER = 'Х';    // символ для рисования рамки игрового поля

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
	scr.pos_str(width / 2 - 8, height - 9, "Сыграем еще раз?");

	int ch = _getch();
	clearkeys();
	if (ch == 'N' || ch == 'n' || ch == 27)
		return false;
	return true;
}

const char FOOD = '$';      // символ для вывода еды

void Game::game_loop() {


	draw_field();           // нарисовать игровое поле

	snake.reset(Coord(width / 2, height / 2));     // установить змею: длина 2,
													// положение - в середине игрового поля,
													// направление - влево
	Command cmd = CMD_NOCOMMAND;
	State stt = STATE_OK;
	// delta  содержит приращение координат (dx, dy) для каждого перемещения змеи по полю
	Coord delta(-1, 0);                // начальное движение - влево
	Coord food = make_food();          // вычислить координаты еды
	scr.pos(food.x, food.y, FOOD);      // вывести еду на экран

	snake.draw(scr);                    // первичное рисование змеи


	do {

		if (_kbhit())                   // если в буфере клавиатуры есть информация,
			cmd = get_command();        // то принять команду

		// обработка команд
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

		Coord hd = snake.head();       // координата головы змеи
		hd += delta;                    // координата головы змеи после приращения (следующая позиция)
		// если голова змеи столкнулась с границей поля или со телом змеи, то змея умирает
		if (hd.x == 0 || hd.x == width - 1 || hd.y == 0 || hd.y == height - 1 || snake.into(hd))
			stt = STATE_DIED;

		if (stt == STATE_OK) {          // если змея ещё жива, то
			snake.move(delta, scr);     // сдвинуть змею на delta

			if (snake.head() == food) { // если координата головы змеи совпадает с координатой еды, то
				snake.grow(food, 3);    // увеличить длину змеи
				food = make_food();     // вычислить координаты новой еды
				scr.pos(food.x, food.y, FOOD); // вывести еду на экран
			}

			Sleep(latency);             // задержка перед следующим изменением позиции
		}

	} while (stt == STATE_OK);          // играем, пока змея жива

	scr.pos_str(width / 2 - 8, 10, "Игра окончена");
	clearkeys();
	_getch();
	clearkeys();
}