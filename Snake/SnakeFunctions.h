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

void updateGameData(const char g[][SIZEX], Snake& spot, const int key, string& mess, int& score, Item& mouse, int& target, int& mouseCount, bool& gameOver)
{ //move spot in required direction
	assert(isArrowKey(key));

	//reset message to blank
	mess = "";

	//calculate direction of movement for given key
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);

	//check new target position in grid and update game data (incl. spot coordinates) if move is possible
	switch (g[spot.getHead().y + dy][spot.getHead().x + dx])
	{			//...depending on what's on the target position in grid...
	case TUNNEL:		//can move
		spot.moveSnake(dx, dy, target);
		score++;
		break;
	case WALL:  		//hit a wall and stay there
	case BODY:							//TODO: this should end the game but it doesnt
		mess = "CANNOT GO THERE!";
		gameOver = true;
		break;
	case MOUSE:
		newMouse(mouse, g);
		target += 2;
		mouseCount++;
		score += 10;
		spot.moveSnake(dx, dy, target);
		break;
	}
}

void placeItem(char g[][SIZEX], const Item& item)
{ //place item at its new position in grid
	g[item.y][item.x] = item.symbol;
}

void placeMaze(char grid[][SIZEX], const char maze[][SIZEX])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], Snake& spot, const Item& mouse)
{ //update grid configuration after each move
	placeMaze(grid, maze);	//reset the empty maze configuration into grid
	//placeItem(grid, spot);	//set spot in grid
	spot.placeSnake(grid);
	placeItem(grid, mouse); //set mouse in grid

}

void updateGame(char grid[][SIZEX], const char maze[][SIZEX], Snake& spot, const int keyCode, string& mess, int& score, Item& mouse, int& target, int& mouseCount, bool& gameOver)
{ //update game
	updateGameData(grid, spot, keyCode, mess, score, mouse, target, mouseCount, gameOver);		//move spot in required direction
	updateGrid(grid, maze, spot, mouse);					//update grid information
}
void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Snake& spot, Item& mouse)
{ //initialise grid and place spot in middle
	setInitialMazeStructure(maze);		   //initialise maze
	spot.initSnake(maze);
	//setSpotInitialCoordinates(spot, maze); //sets spots posititon
	newMouse(mouse, maze);				   //sets mouses position
	updateGrid(grid, maze, spot, mouse);		   //prepare grid
}



int getKeyPress()
{ //get key or command selected by user
  //KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224) 		//ignore symbol following cursor key
		keyPressed = _getch();
	return keyPressed;
}


bool wantsToQuit(const int key)
{	//check if the user wants to quit (when key is 'Q' or 'q')

	return key == QUIT;
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(int x)
{	//convert an integer to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
string tostring(char x)
{	//convert a char to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message)
{	//display a string using specified colour at a given position 
	gotoxy(x, y);
	selectBackColour(backColour);
	selectTextColour(textColour);
	cout << message /*+ string(40 - message.length(), ' ')*/;
}
void renderGame(const char g[][SIZEX], const string& mess, const string& score, const string& mouseCount)
{ //display game title, messages, maze, spot and other items on screen
	//display game title
	showMessage(clDarkCyan, clMagenta, 0, 0, "___GAME___");
	showMessage(clWhite, clRed, 40, 0, "FoP Task 1c - " + getDate() + " - " + getTime());

	showMessage(clWhite, clRed, 40, 1, "CS2 - Group D - Alister, Liam, Manoj");
	//display menu options available
//TODO: Show other options availables when ready...
	showMessage(clRed, clYellow, 40, 3, "TO MOVE - USE KEYBOARD ARROWS ");
	showMessage(clRed, clYellow, 40, 4, "TO QUIT - ENTER 'Q'           ");

	//print auxiliary messages if any
	showMessage(clBlack, clWhite, 40, 8, mess);	//display current message

	showMessage(clBlack, clWhite, 40, 9, score);
	showMessage(clBlack, clWhite, 40, 10, mouseCount);

	//display grid contents
	paintGrid(g);
}

void makeString(const int& score, const int& mice, string& stringScore, string& stringMouse) {
	ostringstream sout;
	istringstream sin;
	sout << "Score:" << score;
	stringScore = sout.str();
	sout.str("");
	sout << mice << " mice collected out of 7";
	stringMouse = sout.str();
	sout.str("");
}

void endProgram(const bool& gameOver)
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	if (gameOver)
	{
		clrscr();
		showMessage(clRed, clYellow, 36, 10, "Game Over");
	}
	else
	{
		clrscr();
		showMessage(clRed, clYellow, 38, 10, "BYE!");
	}
	gotoxy(24, 12);
	system("pause");	//hold output screen until a keyboard key is hit
}