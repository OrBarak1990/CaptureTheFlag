#include "Game.h"
#pragma once
#include "Game.h"
#include <iostream>
#include "Player.h"
#include "Tool.h"
#include "Utils.h"
#include <conio.h>
#include <string>

using namespace std;
Game::Game() :  board{
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', FlgA, ' ',' ' },
	{ ' ',' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' , ' ',' ' ,' ' },
	{ ' ',' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' },
	{ ' ',' ' , FR ,' ' ,' ' ,' ' ,' ' ,' ' ,' ', SEA ,' ' ,' ' ,' ' },
	{ ' ',' ' , FR ,' ' ,' ' ,' ' ,' ' ,' ' ,SEA, SEA ,' ' ,' ' ,' ' },
	{ ' ',' ' , FR , FR ,' ' ,' ' ,' ' , SEA,SEA, SEA ,' ' ,' ' ,' ' },
	{ FR , FR , FR , FR ,' ' ,' ' ,' ' ,' ' ,' ', SEA , SEA,' ' ,' ' },
	{ FR ,' ' ,' ' , FR ,' ' ,' ' ,' ' ,' ' ,' ', SEA , SEA, SEA,' ' },
	{ ' ',' ' ,' ' , FR ,' ' ,' ' ,' ' ,' ' ,' ', SEA , SEA,' ' ,' ' },
	{ ' ',' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ', SEA ,' ' ,' ' ,' ' },
	{ ' ',' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' },
	{ ' ',' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' },
	{ ' ',FlgB,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' },

} {
	a = new Player(1);//initialize the players
	b = new Player(9);
	this->checkOverLoads();//check for any overlappiing tools
}

void Game::menu()//our main menu
{
	bool had_one_game = false;//denotes if a game was already played since startup
	int choice;
	do
	{
		clearScreen();
		cout << "*************************************************************************\n";
		cout << "***                                                                   ***\n";
		cout << "***                                                                   ***\n";
		cout << "***                1.	Select names                                  ***\n";
		cout << "***                2.	Start game                                    ***\n";
		cout << "***                3.	Start reverse game                            ***\n";
		cout << "***                4.	Reset score                                   ***\n";
		cout << "***                9.	Quit                                          ***\n";
		cout << "***                                                                   ***\n";
		cout << "***                                                                   ***\n";
		cout << "*************************************************************************\n";
		
		gotoxy(0, 27);//sets cursor beneath the menu
		hideCursor();
		choice = getch()-'0';//-'0' gets us the number entered as an integer value and not ascii
		switch (choice)
		{
		case 1: {

			gotoxy(0, 27);
			cout << "player A:\n" ;
			a->setName();
			clearline(0, 28);
			gotoxy(0, 27);
			cout << "player B:\n" ;
			b->setName();
			break;
		}
		case 2: {
			if (had_one_game) {//if any game was already played we have some maintanence to perform
				this->resetGame();//resets the players and tools positions
				this->playGame(false);//start the game, false means it isnt a reverse game
			}
			else {
				this->playGame(false);//starts a reverse game
				had_one_game = true;
			}
			break;
		}
		case 3: {
			if (had_one_game) {//same as above
				this->resetGame();
				this->playGame(true);//sets reverse game
			}
			else {
				gotoxy(0, 15);//no reverse games are allowed until at least one round was had
				cout << "You need to play at least one regular game before selecting reverse game." ;
			}
			break;
		}
		case 4: {
			score_a = score_b = 0;//resets scores
			break;
		}
		case 9: {
			gotoxy(0, 15);
			delete this->a;
			delete this->b;
			cout << "Bye Bye\n" << endl;
			break;
		}
		default: {
			break;
		}
		}
	} while (choice != 9);
}

void Game::checkOverLoads()//checks the tools plascement
{
	bool had_overload;
	Tool *temp;
	do
	{
		had_overload = false;
		for (int i = 0; i < (a->getToolSize()); i++) {//get us a reference to each tool
			temp = &(a->getTool(i));
			if (board[temp->getRowLocation()][temp->getColLocation()] != ' ') {//checks if the cell is empty
				temp->setLocation(1);
				had_overload = true;
			}
		}
		if (had_overload)
			a->checkOverLoads(1);
	} while (had_overload);
	do
	{
		had_overload = false;//same as above
		for (int i = 0; i < (b->getToolSize()); i++) {
			temp = &(b->getTool(i));
			if (board[temp->getRowLocation()][temp->getColLocation()] != ' ') {
				temp->setLocation(9);
				had_overload = true;
			}
		}
		if (had_overload)
			b->checkOverLoads(9);
	} while (had_overload);
}

void Game::resetGame() {//need to check overloads but do not reset scores
	a->deleteAllTools();
	b->deleteAllTools();
	a->setNewTools(1);
	b->setNewTools(9);
	this->checkOverLoads();
}

int Game::secondMenu(bool reverse_game)//our secondary menu
{

	int choice;
	//the returns denote:  1 to return to the main menu or to exit the game, 0 otherwise
	clearScreen();
	gotoxy(0, 0);
	cout << "1. Resume.\n2. Restart Game.\n8. Main menu.\n9. Quit game.\n";
	while (true) {
		choice=getch()-'0';//gets integer value of the character entered
		switch (choice)
		{
		case 1://return to the  game
		{
			this->printBoard();//reprints the board
			return 0;
		}
		case 2://resets the gane players and tools, but not score
		{
			this->resetGame();
			this->printBoard();
			return 0;
		}
		case 8://tells playGame function to return to the main menu
		{
			return 1;
		}
		case 9:
		{
			ungetch('9');//put back 9 for exit from menue
			return 1;
		}
		}
	}

}


void Game::playGame(bool reverse_game)//this function manages the game
{
	bool end_game = false, escape;//end_game denotes a player has won, escape flags an escape hit
	int decision, battle_result;
	char direction;
	Tool *tool;
	int clock = 3;//games clock, manages which player's turn is it
	this->printBoard();
	while (!end_game) {//loop until a player has won
		
		do
		{
			escape = false;
			clearline(0, 28);
			tool = &((clock % 2 ? a : b)->pickTool(escape));//depending on the clock cycle, choose the correct player's tool
			if (escape) {//escape hit, go to secondary menu
				int flag=this->secondMenu(reverse_game);//cheks the menu's return value, and exits the game accordingly
				if (flag == 1)
					return;
			}
		} while (escape);
		

		do
		{
			escape = false;
			clearline(0, 28);
		
			direction = (clock % 2 ? a : b)->pickDirection(*tool, escape);//again according to the cycle, choose the player's direction to move
			if (escape)//same as above
			{
				int flag = this->secondMenu(reverse_game);
				if (flag == 1)
					return;
			}
		} while (escape);
		
		decision= playTurn(*tool, direction);//playe a single turn
		if (decision == 3) {//game is won by a player
			if (reverse_game) {
				clock % 2 ? score_b++ : score_a++;//for even clock cycles we add the points to player b instead of a and vice versa
				clearScreen();
				clock % 2 ? cout << b->getName() << " win!!!" << endl : cout << a->getName() << " win!!!" << endl;
			}
			else {
				clock % 2 ? score_a++ : score_b++;
				clearScreen();
				clock % 2 ? cout << a->getName() << " win!!!" << endl : cout << b->getName() << " win!!!" << endl;
			}
			_sleep(1000);
			end_game = true;
		}
		else if (decision == 1) {//means a tool has reched the opposite player's tool, a battle ensues!
			battle_result = (clock % 2 ? b : a)->manageBattle(*tool, ((clock % 2 ? *a : *b)));//gets the battle's result	
			if (b->getToolSize() == 0 || a->getToolSize() == 0) {
				if ((battle_result == 1 && reverse_game) || (battle_result == 2 && !reverse_game)) {//this if-else case deletes all player win tools
					score_b++;//for even clock cycles we add the points to player b instead of a and vice versa
					clearScreen();
					cout << b->getName() <<" win!!!" << endl;
				}
				else {
					score_a++;
					clearScreen();
					cout << a->getName() << " win!!!" << endl;
				}
				_sleep(1000);
				end_game = true;
			}
		}
		else
			continue;
		clock++;//advance the clock
	}
}

int Game::checkValidAndShapeDestination(char tool_type, int row_dest, int col_dest)//checks the legality of the player's move
{//0 = can't mpove,  case2 = flagA, case3 = flagb, case 1 = can move but hasnt won yet
	int res = -1;
	switch (tool_type - 48)
	{
	case 1:
	{
		if (board[row_dest][col_dest] == FR || board[row_dest][col_dest] == SEA || board[row_dest][col_dest] == FlgA)
			res = 0;//case 0 will stop, cant go there.
		else if (board[row_dest][col_dest] == FlgB) 
			res = 3;//reached the flag
		break;
	}
	case 2:
	{
		if(board[row_dest][col_dest] == FlgA)
			res = 0;//case 0 will stop, cant go there.
		else if (board[row_dest][col_dest] == FlgB)
			res = 3;//win
		break;
	}
	case 3:
	{
		if (board[row_dest][col_dest] == FR || board[row_dest][col_dest] == FlgA)
			res = 0;//case 0 will stop, cant go there.
		else if (board[row_dest][col_dest] == FlgB)
			res = 3;
		break;
	}
	case 7:
	{

		if (board[row_dest][col_dest] == FlgB)
			res = 0;//case 0 will stop, cant go there.
		else if (board[row_dest][col_dest] == FlgA)
			res = 3;
		break;
	}
	case 8:
	{
		if (board[row_dest][col_dest] == SEA || board[row_dest][col_dest] == FlgB)
			res = 0;//case 0 will stop, cant go there.
		else if (board[row_dest][col_dest] == FlgA)
			res = 3;
		break;
	}
	case 9:
	{
		if (board[row_dest][col_dest] == FR || board[row_dest][col_dest] == FlgB)
			res = 0;//case 0 will stop, cant go there.
		else if (board[row_dest][col_dest] == FlgA)
			res = 3;
		break;
	}
	default:
	{
		break;
	}
	}
	if (res == -1)
		return 1;
	else
		return res;
}
void Game::updateSingleCell(int row, int col, int mode) {//just an auxilary function that draws over a single board cell
	
	char c = getCell(row, col);
	gotoxy(col * 5 + 1, row * 2 + 1);
	switch (c) {//we need to check if the cell houses a special tile
	case SEA: {
		if (mode)//if mode is set, it means a tool has over a sea tile, s#s denotes a tool numbered # is on a sea tile
			cout << "s s";
		else
			cout << "SEA ";//otherwise a tool has moved away from the sea tile, so restore it
		break;
	}
	case FR: {
		if (mode)//as before, f#f denotes tool # is over a forest tile
			cout << "f f";
		else
			cout << "FR  ";//restores the tile
		break;
	}
	}
}
int Game::playTurn(Tool & tool, char direction)//plays a single turn
{//case 3 = victory,  case 1 = player move to destenation, case 0 =  can't move
	gotoxy(0, 20);
	int row_dest = -1, col_dest = -1;
	int original_row = tool.getRowLocation();//temporarily holds the previous location, so we can resotre the tile if needed
	int original_col = tool.getColLocation();
	int shape_of_destination_square;
	bool no_tools_player_overload;
	if (direction == 'R')//according to the input, updates the tools location
		col_dest = tool.getColLocation() + 1;
	else if (direction == 'L')
		col_dest = tool.getColLocation() - 1;
	else if (direction == 'U')
		row_dest = tool.getRowLocation() - 1;
	else if (direction == 'D')
		row_dest = tool.getRowLocation() + 1;
	if (row_dest == -1)
		row_dest = tool.getRowLocation();
	else
		col_dest = tool.getColLocation();

	if (row_dest >= SIZE || row_dest < 0 || col_dest >= SIZE || col_dest < 0)//prevents overflow, cannot go over the boards borders
		return 0;
	if (tool.getType() - 48 > 4)
		no_tools_player_overload = b->noOverLoadsWithPlayerTools(row_dest, col_dest);
	else
		no_tools_player_overload = a->noOverLoadsWithPlayerTools(row_dest, col_dest);
	shape_of_destination_square = checkValidAndShapeDestination(tool.getType(), row_dest, col_dest);
	if ((shape_of_destination_square > 0) && no_tools_player_overload) {
		updateSingleCell(row_dest,col_dest,1);
		tool.move(row_dest, col_dest);
		updateSingleCell(original_row, original_col, 0);
	}
	return shape_of_destination_square;
}

void Game::printBoard() const//just prints the board all nice and pretty
{
	clearScreen();
	gotoxy(0, 0);
	cout << "_________________________________________________________________\n";
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE;j++)
		{
			if (board[i][j] == FR)
				cout << "|FR  ";
			else if (board[i][j] == SEA)
				cout << "|SEA ";
			else if (board[i][j] == FlgA)
				cout << "|FlgA";
			else if (board[i][j] == FlgB)
				cout << "|FlgB";
			else if (board[i][j] == ' ')
				cout << "|    ";

			if (j == SIZE-1)
				cout << "|";
		}
		cout << "\n";
		cout << "_________________________________________________________________\n";
	}

	gotoxy(0, 27);//prints the scores beneath the board
	cout << a->getName() << "'s score: " << score_a << "               "<< b->getName() << "'s score: " << score_b;

	
	a->draw();//draws the tools
	b->draw();
}