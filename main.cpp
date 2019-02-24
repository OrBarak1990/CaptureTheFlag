#include <iostream>
#include "Player.h"
#include "Tool.h"
#include "Game.h"
#include "Utils.h"
#include "conio.h"
using namespace std;

int main() {
	//first print this awesome ascii art
	cout << 
		"  *****    *    ****  ***** *   * ****  ***** \n"
		"  *       * *   *   *   *   *   * *   * *____  \n"
		"  *      * * *  ****    *   *   * ***   *       \n"
		"  ***** *     * *       *    ***  *  *  *****    \n\n"
		"  ***** *   * *****    ***** *        *     ****           \n"
		"    *   *___* *___     *___  *       * *   * __            \n"
		"    *   *   * *        *     *      * * *  *   *          \n"
		"    *   *   * *****    *     ***** *     *  ***           \n"  ;
	_sleep(200);
	Game *game = new Game();
	
	game->menu();//start the game
	return 0;
}