#include "Player.h"
#include <iostream>
#include "Game.h"
#include "Tool.h"
#include "Utils.h"
#include "conio.h"
#include <string>
using namespace std;

Player::Player(int start_line)//simple constructor
{
	name = new string(start_line == 1 ? "A" : "B");//sets a default name based on the starting line
	setNewTools(start_line);
}

void Player::setNewTools(int start_line)//sets new tools for the player
{
	current_tool_size = TOOL_SIZE;
	tools = new Tool*[this->getToolSize()];
	for (int i = 0;i < this->getToolSize();i++) {
		if (start_line > 5)//start line tells us which player it is, and where should we place his tools
			tools[i] = new Tool((i + 7) + 48);
		else
			tools[i] = new Tool((i + 1) + 48);
		tools[i]->setLocation(start_line);
	}
	checkOverLoads(start_line);
}

void Player::checkOverLoads(int start_line)//checks for any overlapping tools
{
	bool had_overload;
	do
	{
		had_overload = false;
		for (int i = 0; i < TOOL_SIZE && !had_overload; i++) {
			for (int j = TOOL_SIZE - 1; j > i && !had_overload; j--) {
				if ((tools[i]->getRowLocation() == tools[j]->getRowLocation()) &&
					(tools[i]->getColLocation() == tools[j]->getColLocation())) {
					had_overload = true;
					tools[j]->setLocation(start_line);//overlap occured, replace the tool
				}
			}
		}
	} while (had_overload);

}

void Player::setName()//just a name setter
{
	
	name->erase();//erase since the name gets a default string A or B
	getline(cin, *name);
	
	
}

Tool & Player::pickTool(bool &escape)//gets the players input, and chooses the correct tool
{
	bool valid_choice = false;
	Tool *picked;
	char choice;
	do
	{
		
		choice = getch();
		if (choice == 27) {//escape character, go back and start secondary menu
			picked = nullptr;
			escape = true;
			valid_choice = true;
		}
		else {
			for (int i = 0; i < this->getToolSize() && !valid_choice; i++) {//loops over, to find the tool
				if (this->getTool(i).getType() == choice) {
					picked = &(this->getTool(i));
					valid_choice = true;
				}
			}
		}
	} while (!valid_choice);
	return *picked;
}


char Player::pickDirection(Tool &tool, bool &escape)//same as pickTool, but for the direction chosen
{
	bool valid_choice = true;
	char choice, direction;
	do
	{
		
		choice = getch();
		if (choice == 27) {
			direction = NULL;
			escape = true;
		}
		else {
			valid_choice = true;
			if ((tool.getType() - 48 < 4 && choice == 'd') || (tool.getType() - 48 > 4 && choice == 'l'))
				direction = 'R';
			else if ((tool.getType() - 48 < 4 && choice == 'a') || (tool.getType() - 48 > 4 && choice == 'j'))
				direction = 'L';
			else if ((tool.getType() - 48 < 4 && choice == 'w') || (tool.getType() - 48 > 4 && choice == 'i'))
				direction = 'U';
			else if ((tool.getType() - 48 < 4 && choice == 'x') || (tool.getType() - 48 > 4 && choice == 'm'))
				direction = 'D';
			else
				valid_choice = false;
		}
	} while (!valid_choice);
	return direction;//just to avoid warning
}



bool Player::noOverLoadsWithPlayerTools(int row_dest, int col_dest)
{
	for (int i = 0; i < this->getToolSize(); i++) {
		if (this->getTool(i).getRowLocation() == row_dest && this->getTool(i).getColLocation() == col_dest) {
			return false;
		}
	}
	return true;
}

int Player::manageBattle(Tool &tool, Player &other)//manages a battle between two tools over the same tile
{
	int win_player = 0;
	for (int i = 0; i < this->getToolSize(); i++) {
		if (this->getTool(i).getRowLocation() == tool.getRowLocation() && this->getTool(i).getColLocation() == tool.getColLocation()) {
			draw('*' , tool.getColLocation(), tool.getRowLocation());//* denotes that a battle has ensued
			_sleep(550);
			if (this->getTool(i).getType() - 48 < 4)
				win_player = this->getTool(i).fight(tool);//checks which tool has won
			else
				win_player = tool.fight(this->getTool(i));
			draw(' ', tool.getColLocation(), tool.getRowLocation());//deletes the tool from the tile
			if ((win_player == 1 && (this->getTool(i).getType() - 48 < 4)) || (win_player == 2 && (this->getTool(i).getType() - 48 > 4))) {
					other.deleteTool(tool);//deletes the tool from the aray
					this->getTool(i).draw();//draws the one that has won
				}
				else {
					this->deleteTool(this->getTool(i));
					tool.draw();
				}
		}
	}
	return win_player;
}

void Player::draw() const//just draws the player's tools over the board
{
	for (int i = 0; i < this->getToolSize(); i++) {
		draw(this->getTool(i).getType(), this->getTool(i).getColLocation(), this->getTool(i).getRowLocation());
	}
}

void Player::erase() const//deletes the players tools from the board
{
	for (int i = 0; i < TOOL_SIZE; i++)
		draw(' ', this->getTool(i).getColLocation(), this->getTool(i).getRowLocation());
}

void Player::deleteAllTools()//deletes any allocated tools
{
	for (int i = 0; i < this->getToolSize(); i++)
			delete  &(this->getTool(i));
	delete[]tools;
}

void Player::draw(char c, int _x, int _y) const//draws the given char over the given tile
{
	gotoxy((_x) * 5 + 2, _y * 2 + 1);//because of the way the board is drawn, some math is required to get the visual location
	cout << c;
	// make sure it gets to screen on time
	cout.flush();
}

void Player::deleteTool(Tool & tool)//frees the memory allocated to given tool
{
	if (this->getToolSize() - 1 > 0) {
		int j = 0;
		Tool **new_tools = new Tool*[this->getToolSize() - 1];
		for (int i = 0; i < this->getToolSize(); i++) {
			if (this->getTool(i).isEqual(tool)) {
				delete &tool;//destrutor of tool
			}
			else
				new_tools[j++] = &(this->getTool(i));
		}
		delete[]tools;
		tools = new_tools;
	}
	else {
		delete &tool;
	}
	this->decreaseToolSize();
}
