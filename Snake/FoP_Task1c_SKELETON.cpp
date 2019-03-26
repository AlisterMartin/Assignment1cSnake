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
		if (snake.size() == target)
		{
			snake.pop_back();
		}
		snake.front().symbol = BODY;
		snake.push_front({ snake.front().x + dx, snake.front().y + dy, SPOT });

	}
	void placeSnake(char g[][SIZEX]) //render the snake
	{
		for (int i = 0; i < snake.size(); i++)
		{
			g[snake.at(i).y][snake.at(i).x] = snake.at(i).symbol;
		}
	}
	void setTo4(int& target) {
		target = 4;
		while (snake.size() != target) {
			snake.pop_back();
		}
	}
	Item getHead()
	{
		return snake.front();
	}
};
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
	//Item power = { }
	string stringScore, stringMouse, message("LET'S START...");	//current message to player
	int score(0), mouseCount(0), target(4);
	bool gameOver(false), win(false), cheat(false);

	//action...
	seed();								//seed the random number generator
	SetConsoleTitle("FoP 2018-19 - Task 1c - Game Skeleton");
	initialiseGame(grid, maze, spot, mouse);	//initialise grid (incl. walls and spot)
	int key;							//current key selected by player
	do {
		makeString(score, mouseCount, stringScore, stringMouse);
		renderGame(grid, message,stringScore,stringMouse);			//display game info, modified grid and messages
		key = toupper(getKeyPress()); 	//read in  selected key: arrow or letter command
		if (isArrowKey(key)) {
			updateGame(grid, maze, spot, key, message, score, mouse, target, mouseCount, gameOver, cheat);
			showMessage(clBlack, clBlack, 40, 15, "                                                           ");
		}
		else if (key == 'c'|| key =='C') {
			if (!cheat) {
				showMessage(clDarkYellow, clBlack, 40, 15, "Cheat Mode Enabled!");
				cheat = true;
				spot.setTo4(target);
			}
			else {
				showMessage(clDarkYellow, clBlack, 40, 15, "Cheat Mode Disabled!");
				cheat = false;
			}
		}
		else
			message = "INVALID KEY!";  //set 'Invalid key' message
		mouseCount == 7 ? win = true : win = false;
	} while (!wantsToQuit(key) && !gameOver && !win);		//while user does not want to quit
	makeString(score, mouseCount, stringScore, stringMouse);
	renderGame(grid, message, stringScore, stringMouse);			//display game info, modified grid and messages
	endProgram(gameOver, win);						//display final message
	return 0;
}

void updateGameData(const char g[][SIZEX], Snake & spot, const int key, string & mess, int & score, Item & mouse, int & target, int & mouseCount, bool & gameOver, const bool& cheat)
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
		//mouseCount % 2 == 0 ? newMouse();
		if(!cheat)
			target += 2;
		mouseCount++;
		score += 10;
		spot.moveSnake(dx, dy, target);
		break;
	}
}

void placeItem(char g[][SIZEX], const Item & item)
{ //place item at its new position in grid
	g[item.y][item.x] = item.symbol;
}

void placeMaze(char grid[][SIZEX], const char maze[][SIZEX])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], Snake & spot, const Item & mouse)
{ //update grid configuration after each move
	placeMaze(grid, maze);	//reset the empty maze configuration into grid
							//placeItem(grid, spot);	//set spot in grid
	spot.placeSnake(grid);
	placeItem(grid, mouse); //set mouse in grid

}

void updateGame(char grid[][SIZEX], const char maze[][SIZEX], Snake & spot, const int keyCode, string & mess, int & score, Item & mouse, int & target, int & mouseCount, bool & gameOver,const bool& cheat)
{ //update game
	updateGameData(grid, spot, keyCode, mess, score, mouse, target, mouseCount, gameOver, cheat);		//move spot in required direction
	updateGrid(grid, maze, spot, mouse);					//update grid information
}

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Snake & spot, Item & mouse)
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

void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string & message)
{	//display a string using specified colour at a given position 
	gotoxy(x, y);
	selectBackColour(backColour);
	selectTextColour(textColour);
	cout << message /*+ string(40 - message.length(), ' ')*/;
}

void renderGame(const char g[][SIZEX], const string & mess, const string & score, const string & mouseCount)
{ //display game title, messages, maze, spot and other items on screen
  //display game title
	showMessage(clDarkCyan, clMagenta, 0, 0, "___GAME___");
	showMessage(clWhite, clRed, 40, 0, "FoP Task 1c - " + getDate() + " - " + getTime());

	showMessage(clWhite, clRed, 40, 1, "CS2 - Group D - Alister, Liam, Manoj");
	//display menu options available
	//TODO: Show other options availables when ready...
	showMessage(clRed, clYellow, 40, 3, "TO MOVE - USE KEYBOARD ARROWS ");
	showMessage(clRed, clYellow, 40, 4, "TO QUIT - ENTER 'Q'           ");
	showMessage(clRed, clYellow, 40, 5, "TO CHEAT - Enter 'C");
	//print auxiliary messages if any
	showMessage(clBlack, clWhite, 40, 8, mess);	//display current message

	showMessage(clBlack, clWhite, 40, 9, score);
	showMessage(clBlack, clWhite, 40, 10, mouseCount);

	//display grid contents
	paintGrid(g);
}

void makeString(const int & score, const int & mice, string & stringScore, string & stringMouse) {
	ostringstream sout;
	istringstream sin;
	sout << "Score:" << score;
	stringScore = sout.str();
	sout.str("");
	sout << mice << " mice collected out of 7";
	stringMouse = sout.str();
	sout.str("");
}

void endProgram(const bool & gameOver, const bool & win)
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	if (gameOver)
	{
		clrscr();
		showMessage(clRed, clYellow, 36, 10, "Game Over");
	}
	else if (win)
	{
		clrscr();
		showMessage(clGreen, clBlue, 37, 10, "You Win");
	}
	else
	{
		clrscr();
		showMessage(clRed, clYellow, 38, 10, "BYE!");
	}
	gotoxy(24, 12);
	system("pause");	//hold output screen until a keyboard key is hit
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