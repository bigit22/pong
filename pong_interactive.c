#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <ncurses.h>
#include <unistd.h>

#define STR 80
#define ROW 25

// ball_direction: 
// 1 - up right 
// 2 - up left
// 3 - down right
// 4 - down left

void frame(char field[STR][ROW], int racket_y_1, int racket_y_2, int ball_x, int ball_y);
void render(char field[STR][ROW], int score_player_1, int score_player_2);
void ball_move(int* ball_x, int* ball_y, int ball_direction);
void ball_pounce(int* ball_x, int* ball_y, int* ball_direction, int* racket_y_1, int* racket_y_2, int* score_player_1, int* score_player_2);
void control(int* racket_y_1, int* racket_y_2);
void restart(int* score_player_1, int* score_player_2, int* ball_x, int* ball_y, int* racket_y_1, int* racket_y_2);

int main() {
	char field[STR][ROW];
	int racket_y_1 = ROW / 2, racket_y_2 = ROW / 2;
	int ball_x = STR / 2, ball_y = ROW / 2;
	int ball_direction = 4;
	int score_player_1 = 0, score_player_2 = 0;

	int input = 0;

	initscr();

	while (input != 1) {
		nodelay(stdscr, true);

		restart(&score_player_1, &score_player_2, &ball_x, &ball_y, &racket_y_1, &racket_y_2);
		frame(field, racket_y_1, racket_y_2, ball_x, ball_y);
		render(field, score_player_1, score_player_2);
		ball_move(&ball_x, &ball_y, ball_direction);
		ball_pounce(&ball_x, &ball_y, &ball_direction, &racket_y_1, &racket_y_2, &score_player_1, &score_player_2);
		control(&racket_y_1, &racket_y_2);
		
		clear();
		usleep(25 * 1000);
	}
	endwin();
	return 0;
}

void restart(int* score_player_1, int* score_player_2, int* ball_x, int* ball_y, int* racket_y_1, int* racket_y_2){
	if ((*score_player_1) == 21 || (*score_player_2) == 21) {
		(*score_player_1) = 0;
		(*score_player_2) = 0;
		(*ball_x) = STR / 2;
		(*ball_y) = ROW / 2;
		(*racket_y_1) = ROW / 2;
		(*racket_y_2) = ROW / 2;
	}
}

void control(int* racket_y_1, int* racket_y_2) {
	char last_char;
	last_char = getch();
	if (last_char == 'a' || last_char == 'A') {
		if ((*racket_y_1) > 2)
			(*racket_y_1)--;
	}
	else if (last_char == 'z' || last_char == 'Z') {
		if ((*racket_y_1) < ROW - 3)
			(*racket_y_1)++;
	}
	else if (last_char == 'k' || last_char == 'K') {
		if ((*racket_y_2)> 2)
			(*racket_y_2)--;
	}
	else if (last_char == 'm' || last_char == 'M') {
		if ((*racket_y_2) < ROW - 3)
			(*racket_y_2)++;
	}
}

void ball_pounce(int* ball_x, int* ball_y, int* ball_direction, int* racket_y_1, int* racket_y_2, int* score_player_1, int* score_player_2) {
	// upper
	if ((*ball_y) == 1 && (*ball_direction) == 1) {
		(*ball_direction) = 3;
	}
	else if ((*ball_y) == 1 && (*ball_direction) == 2) {
		(*ball_direction) = 4;
	}


	// bottom
	else if ((*ball_y) == ROW - 2 && (*ball_direction) == 3) {
		(*ball_direction) = 1;
	}
	else if ((*ball_y) == ROW - 2 && (*ball_direction) == 4) {
		(*ball_direction) = 2;
	}


	// right
	else if ((*ball_x) == STR - 1 && (*ball_direction) == 1) {
		(*ball_direction) = 2;
		(*ball_x) = STR / 2;
		(*ball_y) = ROW / 2;
		(*score_player_1)++;
	}
	else if ((*ball_x) == STR - 1 && (*ball_direction) == 3) {
		(*ball_direction) = 4;
		(*ball_x) = STR / 2;
		(*ball_y) = ROW / 2;
		(*score_player_1)++;
	}


	// left
	if ((*ball_x) == 1 && (*ball_direction) == 4) {
		(*ball_direction) = 3;
		(*ball_x) = STR / 2;
		(*ball_y) = ROW / 2;
		(*score_player_2)++;
	}
	else if ((*ball_x) == 1 && (*ball_direction) == 2) {
		(*ball_direction) = 1;
		(*ball_x) = STR / 2;
		(*ball_y) = ROW / 2;
		(*score_player_2)++;
	}


	// left racket
	else if (((*ball_x) == 2 && (*ball_direction) == 4) && ((*ball_y) == (*racket_y_1) || (*ball_y) == (*racket_y_1) - 1 || (*ball_y) == (*racket_y_1) + 1)) {
		(*ball_direction) = 3;
	}
	else if (((*ball_x) == 2 && (*ball_direction) == 2) && ((*ball_y) == (*racket_y_1) || (*ball_y) == (*racket_y_1) - 1 || (*ball_y) == (*racket_y_1) + 1)) {
		(*ball_direction) = 1;
	}


	// right racket
	else if (((*ball_x) == STR - 3 && (*ball_direction) == 3) && ((*ball_y) == (*racket_y_2) || (*ball_y) == (*racket_y_2) - 1 || (*ball_y) == (*racket_y_2) + 1)) {
		(*ball_direction) = 4;
	}
	else if (((*ball_x) == STR - 3 && (*ball_direction) == 1) && ((*ball_y) == (*racket_y_2) || (*ball_y) == (*racket_y_2) - 1 || (*ball_y) == (*racket_y_2) + 1)) {
		(*ball_direction) = 2;
	}
}

void ball_move(int* ball_x, int* ball_y, int ball_direction) {
	if (ball_direction == 1) {
		(*ball_x)++;
		(*ball_y)--;
	}
	else if (ball_direction == 2){
		(*ball_x)--;
		(*ball_y)--;
	}
	else if (ball_direction == 3){
		(*ball_x)++;
		(*ball_y)++;
	}
	else if (ball_direction == 4){
		(*ball_x)--;
		(*ball_y)++;
	}
}

void frame(char field[STR][ROW], int racket_y_1, int racket_y_2, int ball_x, int ball_y) {
	for (register int y = 0; y < ROW; y++) {
		for (register int x = 0; x < STR; x++){
			if (y == 0 || y == ROW - 1 || x == 0 || x == STR - 1){
				field[x][y] = '#';
			}
			else if (x == ball_x && y == ball_y) {
				field[x][y] = '*';
			}
			else if ((x == 1) && (racket_y_1 == y || racket_y_1 == y + 1 || racket_y_1 == y - 1)) {
				field[x][y] = '|';
			}
			else if ((x == STR - 2) && (racket_y_2 == y || racket_y_2 == y + 1 || racket_y_2 == y - 1)) {
				field[x][y] = '|';
			}
			else if (x == STR / 2) {
				field[x][y] = '|';
			}
			else {
				field[x][y] = ' ';
			}
		}
	}
}

void render(char field[STR][ROW], int score_player_1, int score_player_2) {
	for (register int y = 0; y < ROW; y++) {
		for (register int x = 0; x < STR; x++){
			printw("%c", field[x][y]);
		}
		printw("\n");
	}
	printw("                                       %d:%d                                      ", score_player_1, score_player_2);
}
