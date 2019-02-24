#include "Tool.h"
#include <ctime>

Tool::Tool()//default constructor
{
	row = col = -1;
}

Tool::Tool(char _type)//constructor based on tool type
{
	this->setType(_type);
	row = col = -1;
}

void Tool::setLocation(int start_line)//based on the starting line, sets the tools initial random location
{
	int check_row = this->getRowLocation(), check_col = this->getColLocation(), _row, _col;
	bool found = false;
	srand(time(NULL));
	while (!found)//loops until a plausible location was randomized
	{
		_row = rand() % 5 + start_line - 1;
		_col = rand() % 13;
		if (check_row != _row || check_col != _col)
			found = true;
	}
	this->setRowLocation(_row);
	this->setColLocation(_col);

}

void Tool::move(int _row, int _col)//moves a tool to the corresponding tile
{
		
	this->draw(this->getType(), _col, _row);//draws the new location
	
	_sleep(100);//jyst to make it look like its moving over
	this->erase();//deletes the previous

	
	this->setRowLocation(_row);//updates the location
	this->setColLocation(_col);
	_sleep(100);
	
}

int Tool::fight(Tool &other)//manages a battle between two tools, returns:  1for player a win, 2 for player b win
{
	//work only on player A
	if (((this->getType() - 48) == 1) && (row < 9  || col != 3)) {
		return 1;//win
	}
	else if ((this->getType() - 48) == 2 && (((other.getType() - 48) == 7 || (other.getType() - 48) == 8) && ((row >= 2 && row <= 3) || col == 10))) {
		return 1;//win
	}
	else if ((this->getType() - 48) == 3 && (row == 7 || col == 6)) {
		return 1;//win
	}
	return 2;//lose
}

void Tool::draw() const {//draws this tool over the board
	draw(this->getType(), this->getColLocation(), this->getRowLocation());
}

void Tool::erase() const//erases this tool from the board
{
	draw(' ', this->getColLocation(), this->getRowLocation());
}

void Tool::draw(char c, int _x, int _y) const//draws the given char over desired tile
{
	gotoxy((_x) * 5 + 2, _y * 2 + 1);
	
	cout << c;

}