//---------------------------------------------------------------------------
//Program: Skeleton for Task 1c � group assignment
//Author: Pascale Vacher
//Last updated: 23 February 2018
//---------------------------------------------------------------------------

//Go to 'View > Task List' menu to open the 'Task List' pane listing the initial amendments needed to this program

//---------------------------------------------------------------------------
//----- include libraries
//---------------------------------------------------------------------------

//include standard libraries
#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>
#include <deque>
using namespace std;

//include our own libraries
#include "RandomUtils.h"    //for seed, random
#include "ConsoleUtils.h"	//for clrscr, gotoxy, etc.
#include "TimeUtils.h"		//for getSystemTime, timeToString, etc.
#include "SnakeFunctions.h" //for the various game functions used in the program

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	//local variable declarations 
	char grid[SIZEY][SIZEX];			//grid for display
	char maze[SIZEY][SIZEX];			//structure of the maze
	Snake spot;							//Initialise new snake class
	//Item spot = { 0, 0, SPOT }; 		//spot's position and symbol
	Item mouse = { 0, 0, MOUSE };			//mouse position and symbol
	string stringScore, stringMouse, message("LET'S START...");	//current message to player
	//TODO: Display!!!!!!!!!!!!!!!!!!!!!!!!!!
	int score(0), mouseCount(0), target(5);
	bool gameOver(false);

	//action...
	seed();								//seed the random number generator
	SetConsoleTitle("FoP 2018-19 - Task 1c - Game Skeleton");
	initialiseGame(grid, maze, spot, mouse);	//initialise grid (incl. walls and spot)
	int key;							//current key selected by player
	do {
		makeString(score, mouseCount, stringScore, stringMouse);
		renderGame(grid, message,stringScore,stringMouse);			//display game info, modified grid and messages
		key = toupper(getKeyPress()); 	//read in  selected key: arrow or letter command
		if (isArrowKey(key))
			updateGame(grid, maze, spot, key, message, score, mouse, target, mouseCount, gameOver);
		else
			message = "INVALID KEY!";  //set 'Invalid key' message
	} while (!wantsToQuit(key) && !gameOver);		//while user does not want to quit
	makeString(score, mouseCount, stringScore, stringMouse);
	renderGame(grid, message, stringScore, stringMouse);			//display game info, modified grid and messages
	endProgram(gameOver);						//display final message
	return 0;
}