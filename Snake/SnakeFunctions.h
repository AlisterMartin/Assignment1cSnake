#pragma once
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

//---------------------------------------------------------------------------
//----- define constants
//---------------------------------------------------------------------------

//defining the size of the grid
const int  SIZEX(15);    	//horizontal dimension
const int  SIZEY(13);		//vertical dimension
//defining symbols used for display of the grid and content
const char SPOT('@');   	//spot
const char BODY('o');		//snake body
const char TUNNEL(' ');    	//tunnel
const char WALL('#');    	//border
const char MOUSE('M');		//mouse
//defining the command letters to move the spot on the maze
const int  UP(72);			//up arrow
const int  DOWN(80); 		//down arrow
const int  RIGHT(77);		//right arrow
const int  LEFT(75);		//left arrow
//defining the other command letters
const char QUIT('Q');		//to end the game

struct Item {
	int x, y;
	char symbol;
};

class Snake {
	/*This is the snake class, it uses a deque which is a double ended queue which allows
	me to add to the front of the snake and take from the back to allow it to move easier.*/
	deque<Item> snake;
public:
	Snake()
	{

	}
	void initSnake(const char maze[][SIZEX]) //initialise snake
	{
		snake.push_front({ 0, 0, SPOT });
		do {
			snake.front().y = random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
			snake.front().x = random(SIZEX - 2);		 //horizontal coordinate in range [1..(SIZEX - 2)]
		} while (maze[snake.front().y][snake.front().x] == WALL);
	}
	void moveSnake(int dx, int dy, int target) //move the snake
	{
		snake.front().symbol = BODY;
		snake.push_front({ snake.front().x + dx, snake.front().y + dy, SPOT });
		if (snake.size() == target)
		{
			snake.pop_back();
		}
	}
	void placeSnake(char g[][SIZEX]) //render the snake
	{
		for (int i = 0; i < snake.size(); i++)
		{
			g[snake.at(i).y][snake.at(i).x] = snake.at(i).symbol;
		}
	}
	Item getHead()
	{
		return snake.front();
	}
};

void paintGrid(const char g[][SIZEX])
{ //display grid content on screen
	selectBackColour(clBlack);
	gotoxy(0, 2);

	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col) {
			if (g[row][col] != '@') {

				selectTextColour(clWhite);
			}
			else {
				selectTextColour(clGreen);
			}
			cout << g[row][col];	//output cell content
		}
		cout << endl;
	}
}

bool isArrowKey(const int key)
{	//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')

	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}

void newMouse(Item& mouse, const char maze[][SIZEX]) {
	do {
		mouse.y = random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
		mouse.x = random(SIZEX - 2);		 //horizontal coordinate in range [1..(SIZEX - 2)]
	} while (maze[mouse.y][mouse.x] != TUNNEL);
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void setInitialMazeStructure(char maze[][SIZEX])
{ //set the position of the walls in the maze
  //initialise maze configuration
	char initialMaze[SIZEY][SIZEX] 	//local array to store the maze structure
		= { { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };
	//with '#' for wall, ' ' for tunnel, etc. 
	//copy into maze structure with appropriate symbols
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (initialMaze[row][col])
			{
				//not a direct copy, in case the symbols used are changed
			case '#': maze[row][col] = WALL; break;
			case ' ': maze[row][col] = TUNNEL; break;
			}
}

/*void setSpotInitialCoordinates(Item& spot, const char maze[][SIZEX])     No longer needed as this is handled in snake class
{ //set spot coordinates inside the grid at random at beginning of game
	do {
		spot.y = random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
		spot.x = random(SIZEX - 2);		 //horizontal coordinate in range [1..(SIZEX - 2)]
	} while (maze[spot.y][spot.x] == WALL);
}*/

//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy)
{ //calculate direction indicated by key
	assert(isArrowKey(key));
	switch (key)	//...depending on the selected key...
	{
	case LEFT:  	//when LEFT arrow pressed...
		dx = -1;	//decrease the X coordinate
		dy = 0;
		break;
	case RIGHT: 	//when RIGHT arrow pressed...
		dx = +1;	//increase the X coordinate
		dy = 0;
		break;
	case UP:
		dx = 0;
		dy = -1;
		break;
	case DOWN:
		dx = 0;
		dy = +1;
		break;
	}
}

//---------------------------------------------------------------------------
//----- Update Game
//---------------------------------------------------------------------------

void updateGameData(const char g[][SIZEX], Snake& spot, const int key, string& mess, int& score, Item& mouse, int& target, int& mouseCount, bool& gameOver);

void placeItem(char g[][SIZEX], const Item& item);

void placeMaze(char grid[][SIZEX], const char maze[][SIZEX]);

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], Snake& spot, const Item& mouse);

void updateGame(char grid[][SIZEX], const char maze[][SIZEX], Snake& spot, const int keyCode, string& mess, int& score, Item& mouse, int& target, int& mouseCount, bool& gameOver);

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Snake& spot, Item& mouse);



int getKeyPress();


bool wantsToQuit(const int key);

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(int x);

string tostring(char x);

void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

void renderGame(const char g[][SIZEX], const string& mess, const string& score, const string& mouseCount);

void makeString(const int& score, const int& mice, string& stringScore, string& stringMouse);

void endProgram(const bool& gameOver, const bool& win);