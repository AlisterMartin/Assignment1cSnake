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
#include <vector>
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
	int start = 0;
	vector<Item> items;
	vector<int> link;
	vector<int> free;
public: Snake()
	{

	}
public: void initSnake(const char maze[][SIZEX])
	{
		start = 0;
		do {
			items.at(start).y = random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
			items.at(start).x = random(SIZEX - 2);		 //horizontal coordinate in range [1..(SIZEX - 2)]
		} while (maze[items.at(start).y][items.at(start).x] == WALL);
		link.at(start) = 1;
		do {
			items.at(link.at(start)).y = random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]  NEEDS CHANGING
			items.at(link.at(start)).x = random(SIZEX - 2);		 //horizontal coordinate in range [1..(SIZEX - 2)] NEEDS CHANGING
		} while (maze[items.at(start).y][items.at(start).x] == WALL || (items.at(start).y == items.at(link.at(start)).y && items.at(start).x == items.at(link.at(start)).x));
	}
private: int getNode()
	{
		assert(!free.empty());
		int i = 0;
		while (free.at(i) == 0)
		{
			i++;
		}
		int r = free.at(i);
		free.at(i) = 0;
		return r;
	}
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	//function declarations (prototypes)
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], Item& spot, Item& mouse);
	void renderGame(const char g[][SIZEX], const string& mess);
	void updateGame(char g[][SIZEX], const char m[][SIZEX], Item& s, const int kc, string& mess, int& score, Item& mouse);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int k);
	int  getKeyPress();
	void endProgram();

	//local variable declarations 
	char grid[SIZEY][SIZEX];			//grid for display
	char maze[SIZEY][SIZEX];			//structure of the maze
	Item spot = { 0, 0, SPOT }; 		//spot's position and symbol
	Item mouse = {0, 0, MOUSE};			//mouse position and symbol
	string message("LET'S START...");	//current message to player
	//TODO: Display!!!!!!!!!!!!!!!!!!!!!!!!!!
	int score(0);

	//action...
	seed();								//seed the random number generator
	SetConsoleTitle("FoP 2018-19 - Task 1c - Game Skeleton");
	initialiseGame(grid, maze, spot, mouse);	//initialise grid (incl. walls and spot)
	int key;							//current key selected by player
	do {
		renderGame(grid, message);			//display game info, modified grid and messages
		
		key = toupper(getKeyPress()); 	//read in  selected key: arrow or letter command
		if (isArrowKey(key))
			updateGame(grid, maze, spot, key, message, score, mouse);
		else
			message = "INVALID KEY!";  //set 'Invalid key' message
	} while (!wantsToQuit(key));		//while user does not want to quit
	renderGame(grid, message);			//display game info, modified grid and messages
	endProgram();						//display final message
	return 0;
}


//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Item& spot, Item& mouse)
{ //initialise grid and place spot in middle
	void setInitialMazeStructure(char maze[][SIZEX]);
	void setSpotInitialCoordinates(Item& spot, const char maze[][SIZEX]);
	void updateGrid( char g[][SIZEX], const char m[][SIZEX], const Item& i, const Item& mouse);
	void newMouse(Item& mouse, const char maze[][SIZEX]);

	setInitialMazeStructure(maze);		   //initialise maze
	setSpotInitialCoordinates(spot, maze); //sets spots posititon
	newMouse(mouse, maze);				   //sets mouses position
	updateGrid(grid, maze, spot, mouse);		   //prepare grid
}

void setSpotInitialCoordinates(Item& spot, const char maze[][SIZEX])
{ //set spot coordinates inside the grid at random at beginning of game
	do{
	spot.y = random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
	spot.x = random(SIZEX - 2);		 //horizontal coordinate in range [1..(SIZEX - 2)]
	} while (maze[spot.y][spot.x] == WALL);
} 

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

//---------------------------------------------------------------------------
//----- Update Game
//---------------------------------------------------------------------------

void updateGame(char grid[][SIZEX], const char maze[][SIZEX], Item& spot, const int keyCode, string& mess, int& score, Item& mouse)
{ //update game
	void updateGameData(const char g[][SIZEX], Item& s, const int kc, string& m, int& score, Item& mouse);
	void updateGrid(char g[][SIZEX], const char maze[][SIZEX], const Item& s, const Item& mouse);
	updateGameData(grid, spot, keyCode, mess, score, mouse);		//move spot in required direction
	updateGrid(grid, maze, spot, mouse);					//update grid information
}
void updateGameData(const char g[][SIZEX], Item& spot, const int key, string& mess, int& score, Item& mouse)
{ //move spot in required direction
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy);
	void newMouse(Item& mouse, const char maze[][SIZEX]);
	assert (isArrowKey(key));

	//reset message to blank
	mess = "";

	//calculate direction of movement for given key
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);

	//check new target position in grid and update game data (incl. spot coordinates) if move is possible
	switch (g[spot.y + dy][spot.x + dx])
	{			//...depending on what's on the target position in grid...
	case TUNNEL:		//can move
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		break;
	case WALL:  		//hit a wall and stay there

		//cout << '\a';	//beep the alarm
		mess = "CANNOT GO THERE!";
		break;
	case MOUSE:
		newMouse(mouse, g);
		score += 10;
		break;
	}
}
void newMouse(Item& mouse, const char maze[][SIZEX]) {
	do {
		mouse.y = random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
		mouse.x = random(SIZEX - 2);		 //horizontal coordinate in range [1..(SIZEX - 2)]
	} while (maze[mouse.y][mouse.x] != TUNNEL);
}
void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const Item& spot, const Item& mouse)
{ //update grid configuration after each move
	void placeMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item& spot);

	placeMaze(grid, maze);	//reset the empty maze configuration into grid
	placeItem(grid, spot);	//set spot in grid
	placeItem(grid, mouse); //set mouse in grid

}

void placeMaze(char grid[][SIZEX], const char maze[][SIZEX])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void placeItem(char g[][SIZEX], const Item& item)
{ //place item at its new position in grid
	g[item.y][item.x] = item.symbol;
}
//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy)
{ //calculate direction indicated by key
	bool isArrowKey(const int k);
	assert (isArrowKey(key));
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

int getKeyPress()
{ //get key or command selected by user
  //KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224) 		//ignore symbol following cursor key
		keyPressed = _getch();
	return keyPressed;				 
}

bool isArrowKey(const int key)
{	//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')

	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
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
	cout << message + string(40 - message.length(), ' ');
}
void renderGame(const char g[][SIZEX], const string& mess)
{ //display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	string tostring(int x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	void paintGrid(const char g[][SIZEX]);
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

	//display grid contents
	paintGrid(g);
}

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

void endProgram()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	showMessage(clRed, clYellow, 40, 8, "BYE!");	
	system("pause");	//hold output screen until a keyboard key is hit
}
