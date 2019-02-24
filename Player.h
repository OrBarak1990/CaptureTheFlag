#pragma once
#include <iostream>
#include "Tool.h"
#include "Game.h"
#include "Utils.h"

using namespace std;
const int TOOL_SIZE = 3;
class Tool;//forward decleration
class Player{
	string *name;
	Tool **tools;
	int current_tool_size;
public:
	Player(int start_line);
	~Player() { //assunimg we do the name with string
		delete name;
		if(this->getToolSize() > 0)
			this->deleteAllTools();
	}
	void setNewTools(int start_line);

	void checkOverLoads(int start_line);

	Tool &getTool(int index) const { //want to get it by ref
		return *tools[index];
	}

	int getToolSize() const {
		return current_tool_size;
	}

	void decreaseToolSize() {
		 current_tool_size--;
	}

	string getName() const {
		return *name;
	}

	Tool &pickTool(bool &escape);

	//Tool &pickTool(char sym);

	void setName();

	char pickDirection(Tool &tool, bool &escape);

	//char pickDirection(char sym);

	bool noOverLoadsWithPlayerTools(int row_dest, int col_dest);

	int manageBattle(Tool &tool, Player &other);//7/04, change return status: 0-no fight, 1-a won, 2-b won

	void draw() const;

	void erase() const;

	void deleteAllTools();

private:
	void draw(char c, int _x, int _y) const;
	void deleteTool(Tool &tool);
};
