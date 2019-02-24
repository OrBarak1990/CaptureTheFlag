#pragma once

#include <iostream>
#include "Player.h"
#include "Tool.h"
using namespace std;

const int SIZE = 13;

class Player;
class Tool;
class Game {
	enum  { SEA, FR, FlgA, FlgB };
	Player *a, *b;
	char board[SIZE][SIZE];
	int score_a = 0, score_b = 0, current_size = SIZE;
public:
	Game();
	void menu();
	int getScoreA() const {
		return score_a;
	}
	int getScoreB() const{
		return score_b;
	}
	void printBoard() const;
	char getCell(int row, int col) const {
		return board[row][col];
	}

private:
	void resetGame();
	void checkOverLoads();
	int secondMenu(bool reverse_game);
	void playGame(bool reverse_game);
	int checkValidAndShapeDestination(char tool_type, int row_dest, int col_dest);
	int playTurn(Tool &tool, char direction);
	void updateSingleCell(int row, int col, int mode);
};
