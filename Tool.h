#pragma once
#include <iostream>
#include "Game.h"
using namespace std;

class Tool
{
	enum { SEA, FR, FlgA, FlgB };
	int row, col;
	char type;
public:
	Tool();
	Tool(char _type);
	~Tool() { this->erase(); }
	 int getRowLocation() const {
		return row;
	}
	 int  getColLocation() const {
		return col;
	}
	void setRowLocation(int _row) {
		row = _row;
	}
	void setColLocation(int _col) {
		col = _col;
	}
	void setType(char _type) {
		type = _type;
	}
	char  getType() const {
		return type;
	}
	void setLocation(int start_line);
	void move(int _row, int _col);
	int fight(Tool &other);
	void draw() const;
	void erase() const;
	bool  isEqual(Tool   &other) const {
		if (this->getRowLocation() == other.getRowLocation() && this->getColLocation() == other.getColLocation())
			return true;
		return false;
	}
private:
	void draw(char c, int _x, int _y) const;
};
