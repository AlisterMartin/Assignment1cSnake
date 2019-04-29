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
#include <fstream>
#include <vector>

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

struct goBundle {
	bool gameOver;
	string message;
};


class Snake {
	/*This is the snake class, it uses a deque which is a double ended queue which allows
	me to add to the front of the snake and take from the back to allow it to move easier.*/
	deque<Item> snake;
public:
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
		if (snake.size() == target) //if the snake is at its target length then take the end part off 
		{
			snake.pop_back();
		}
		snake.front().symbol = BODY; //give the old head the symbol of a body
		snake.push_front({ snake.front().x + dx, snake.front().y + dy, SPOT }); //add a new head on the next coordinates

	}
	void placeSnake(char g[][SIZEX]) //render the snake
	{
		for (int i = 0; i < snake.size(); i++)
		{
			g[snake.at(i).y][snake.at(i).x] = snake.at(i).symbol; //place each part of the snake individually
		}
	}
	void setTo4(int& target) { //reset the snake to original size
		target = 4;
		while (snake.size() > target) {
			snake.pop_back(); //take the back item off until it is the original size
		}
	}
	Item getHead() // return the head of the snake
	{
		return snake.front();
	}
  void kill() // empty the snake vector
  {
    while (snake.size() > 0)
    {
      snake.pop_back();
    }
  }
  void readout(ofstream& o) // put all of the snake pieces into an file stream
  {
    for (int i = 0; i < snake.size(); i++)
    {
      o << snake.at(i).x << "," << snake.at(i).y << "," << (int)snake.at(i).symbol << ",";
    }
    o << endl;
  }
  void readin(const string& i) // construct a snake from a file stream
  {
    vector<int> vect;
    stringstream ss(i);
    int r;
    while (ss.peek() != '/n' && ss >> r)
    {
      vect.push_back(r);

      if (ss.peek() == ',')
        ss.ignore();
    }
    for (int c = 0; c < vect.size(); c++)
    {
      switch (c % 3) {
      case 0:
        snake.push_back({ vect.at(c), 0, 'x' });
        break;
      case 1:
        snake.at(c / 3).y = vect.at(c);
        break;
      case 2:
        snake.at(c / 3).symbol = vect.at(c);
        break;
      }

        
    }
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
	string name;                        //name of the player
	string highScore;
	int startTime;
	int currentTime;
	int countdownTimer;
	int countdownTime = 0;				//to be set by reading level files
	bool moved = false;
	bool timeInitilized = false;
	int mouseLevelCount(0);
	int currentLevel = 1;				//to be set by reading indiviual files
	Snake spot;							//Initialise new snake class
	//Item spot = { 0, 0, SPOT }; 		//spot's position and symbol
	Item mouse = { 0, 0, MOUSE };		//mouse position and symbol
	Item power = { 1, 1, TUNNEL };
	Item M1 = { 0, 0, WALL };	//mongooses - start as wall to hide till the need displaying
	Item M2 = { 0, 0, WALL };
	string stringScore, stringMouse, message("LET'S START...");	//current message to player
	goBundle go = { false, " " };	//contains messgae for game end and bool
	int score(0), mouseCount(0), target(4), delay(400), temp_delay(delay), inv(0), ptimer(0);
	bool cheat(false), speed(false), win(false);
	bool hasCheated(false);
	//initilize game
	name = getName();
	//initilize file if it does not exist
	if (!doesItExist(name)) {
		initilizeFile(name);
		highScore = "500";
	}
	else
	{
		highScore = getHighScore(name);
	}
	currentLevel = selectInitialLevel(name, hasCheated);

	while (currentLevel != 9 && !go.gameOver) {
		clrscr();
		//action...
		seed();								//seed the random number generator
		SetConsoleTitle("FoP 2018-19 - Task 1c - Game Skeleton");
		saveCheck(name) ? loadSave(spot, mouse, power, M1, M2, mouseLevelCount, currentLevel, countdownTime, name, score, mouseCount, countdownTimer, grid, maze, target): initialiseGame(grid, maze, spot, mouse, power, M1, M2, mouseLevelCount, currentLevel, countdownTime);	//initialise grid (incl. walls and spot)
		int key, temp_key;//current key selected by player
		makeString(score, mouseCount, stringScore, stringMouse, mouseLevelCount);
		renderGame(grid, message, stringScore, stringMouse, highScore, inv, name, countdownTimer, timeInitilized, startTime, countdownTime, go, currentLevel);//display game info, modified grid and messages
		key = toupper(getKeyPress());
		do {
			Sleep(delay);
			inv > 0 ? inv-- : inv = 0;
			ptimer > 0 ? ptimer-- : power.symbol = TUNNEL;
			if (kbhit())
				key = toupper(getKeyPress());//read in  selected key: arrow or letter command
			if (isArrowKey(key)) {
				moved = true;
				updateGame(grid, maze, spot, key, message, score, mouse, target, mouseCount, go, cheat, power, delay, inv, ptimer, M1, M2, countdownTimer);
				temp_key = key;
				showMessage(clBlack, clBlack, 40, 15, "                                                           ");
			}
			else if (key == 'c' || key == 'C') {
				hasCheated = true;
				key = temp_key;
				if (!cheat) {
					showMessage(clDarkYellow, clBlack, 40, 15, "Cheat Mode Enabled!");
					cout << "\a\a\a";
					cheat = true;
					spot.setTo4(target);
				}
				else {
					showMessage(clDarkYellow, clBlack, 40, 15, "Cheat Mode Disabled!");
					cheat = false;
				}
			}
			else if (key == 'Z') {
				key = temp_key;
				if (!speed) {
					temp_delay = delay;
					delay = 400;
					speed = true;
				}
				else {
					delay = temp_delay;
					speed = false;
				}
			}
      else if (key == 'O')
      {
        saveGame(spot, mouse, power, M1, M2, mouseLevelCount, currentLevel, countdownTime, name, score, mouseCount, countdownTimer, target);
        go.gameOver = true;
        go.message = "You saved the game";
      }
      else if (wantsToQuit(key))
      {
        go.gameOver = true;
        go.message = "You quit the game";
      }
			else
				message = "INVALID KEY!";  //set 'Invalid key' message
			if (mouseCount == mouseLevelCount) {
				win = true;
				go.gameOver = true;
				go.message = "YOU GOT ALL THE MICE! YOU WIN! Remaining time: " + to_string(countdownTimer);
			}
			makeString(score, mouseCount, stringScore, stringMouse, mouseLevelCount);
			if (moved && !timeInitilized) {
				startTime = getIntTime();
				timeInitilized = true;
			}
			renderGame(grid, message, stringScore, stringMouse, highScore, inv, name, countdownTimer, timeInitilized, startTime, countdownTime, go, currentLevel);//display game info, modified grid and messages
			if (countdownTimer < 0) {
				go.gameOver = true;
				go.message = "YOU RAN OUT OF TIME!";
			}
		} while (!go.gameOver);		//while user does not want to quit
		makeString(score, mouseCount, stringScore, stringMouse, mouseLevelCount);
		renderGame(grid, message, stringScore, stringMouse, highScore, inv, name, countdownTimer, timeInitilized, startTime, countdownTime, go, currentLevel);			//display game info, modified grid and messages

		//update file
		if (win) {
			if (currentLevel == 3) {
				go.message = "WELL DONE! You finished the game with score: " + tostring(score);
				if(!hasCheated)
				writeToFile(score, name, currentLevel);
				currentLevel = 9;
			}
			else
			{

				endProgram(go);
				currentLevel += 1;
				if (!hasCheated)
				writeToFile(score, name, currentLevel);
				currentLevel = selectLevel(name, hasCheated);
				mouseCount = 0;
        target = 4;
        spot.kill();
        M1.symbol = ' ';
        delay = 400;
				go.gameOver = false;
				win = false;
				renderGame(grid, message, stringScore, stringMouse, highScore, inv, name, countdownTimer, timeInitilized, startTime, countdownTime, go, currentLevel);
			}

			//reset all values
		}
		if (currentLevel == 9) {
			go.message = "Thank you for playing our game";
		}
	}
	endProgram(go);						//display final message
	return 0;
}

void updateGameData(const char g[][SIZEX], Snake & spot, const int key, string & mess, int & score, Item & mouse, int & target, int & mouseCount, goBundle& go, const bool& cheat, Item& power, int& delay, int& inv, int& ptimer, Item& M1, Item& M2, const int& countdownTimer)
{ //move spot in required direction
	assert(isArrowKey(key));
	string cdTimer = to_string(countdownTimer);
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
		if (inv > 0) {
			
		}
		else {
			go.gameOver = true;
			go.message = "YOU HIT A WALL! YOU LOSE! Remaining time: " + cdTimer;
		}
		break;
	case MONGOOSE: 
		if (inv <= 0) {
			go.gameOver = true; //end the game
			go.message = "YOU GOT EATEN! YOU LOSE! Remaining time: " + cdTimer;
		}
		break;
	case BODY:
		if (inv <= 0) {
			go.gameOver = true; //end the game
			go.message = "YOU RAN INTO YOUR SELF! YOU LOSE! Remaining time: " + cdTimer;
		}
		break;
	case MOUSE:
		newMouse(mouse, g);
		mouseCount++;
		if (mouseCount % 2 == 0)
		{
			power.symbol = POWER;
			ptimer = 40;
			newMouse(power, g);	//place the power pill in the grid if 
		}
		if (!cheat)
		{
			target += 2;
		}
		score += 10;
		spot.moveSnake(dx, dy, target);
		mouseCount % 2 == 0 && delay !=100 ? delay -= 50 : delay; //speeds up snake every two mice 
		if (mouseCount == 3) {
				M1.symbol = MONGOOSE;
				newMouse(M1, g);
			}
		
		break;
	case POWER:
    inv = 20;
		spot.setTo4(target);
		spot.moveSnake(dx, dy, target);
    ptimer = 0;
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

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], Snake & spot, const Item & mouse, const Item & power, const Item& M1, const Item& M2)
{ //update grid configuration after each move
	placeMaze(grid, maze);	//reset the empty maze configuration into grid
	placeItem(grid, power);						
	//placeItem(grid, spot);	//set spot in grid
	spot.placeSnake(grid);
	placeItem(grid, mouse); //set mouse in grid
	placeItem(grid, M1);
	placeItem(grid, M2);
}

void updateGame(char grid[][SIZEX], const char maze[][SIZEX], Snake & spot, const int keyCode, string & mess, int & score, Item & mouse, int & target, int & mouseCount, goBundle& go,const bool& cheat, Item& power, int& delay, int& inv, int& ptimer, Item& M1, Item& M2, const int& countdownTimer)
{ //update game
	moveMongoose(grid, M1, go, countdownTimer);	//moves mongees if needed
	//moveMongoose(grid, M2, go, countdownTimer);
	updateGameData(grid, spot, keyCode, mess, score, mouse, target, mouseCount, go, cheat, power, delay, inv, ptimer, M1, M2, countdownTimer);		//move spot in required direction
	updateGrid(grid, maze, spot, mouse, power, M1, M2);					//update grid information
}

void moveMongoose(const char g[][SIZEX],Item& M,goBundle& go, const int& countdownTimer) {
	if (M.symbol != TUNNEL) {
		string cdTimer = to_string(countdownTimer);
		int D1 = random(10);
		switch (D1) {
		case 1:
			if (g[M.y][M.x + 1] == TUNNEL)
				M.x++;
			else if (g[M.y][M.x + 1] == SPOT) {
				go.gameOver = true;
				go.message = "YOU GOT EATEN! GAME OVER! Remaining time: " + cdTimer;
				M.x++;
			}
			break;
		case 2:
			if (g[M.y][M.x - 1] == TUNNEL)
				M.x--;
			else if (g[M.y][M.x - 1] == SPOT) {
				go.gameOver = true;
				go.message = "YOU GOT EATEN! GAME OVER! Remaining time: " + cdTimer;
				M.x--;
			}
			break;
		case 3:
			if (g[M.y + 1][M.x] == TUNNEL)
				M.y++;
			else if (g[M.y + 1][M.x] == SPOT) {
				go.gameOver = true;
				go.message = "YOU GOT EATEN! GAME OVER! Remaining time: " + cdTimer;
				M.y++;
			}
			break;
		case 4:
			if (g[M.y - 1][M.x] == TUNNEL)
				M.y--;
			else if (g[M.y - 1][M.x] == SPOT) {
				go.gameOver = true;
				go.message = "YOU GOT EATEN! GAME OVER! Remaining time: " + cdTimer;
				M.y--;
			}
			break;
		case 5:
			break; //not move
		case 6:
			break; //not move
		case 7:
			break; //not move
		case 8:
			break; //not move
		case 9:
			break; //not move
		case 10:
			break; //not move
		}
	}
}

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Snake & spot, Item & mouse, Item & power, const Item& M1, const Item& M2, int& mouseLevelCount, const int& currentLevel, int& countdownTime)
{ //initialise grid and place spot in middle
	setInitialMazeStructure(maze, mouseLevelCount, currentLevel, countdownTime);		   //initialise maze
	spot.initSnake(maze);
	//setSpotInitialCoordinates(spot, maze); //sets spots posititon
	newMouse(mouse, maze);				   //sets mouses position
	updateGrid(grid, maze, spot, mouse, power, M1, M2);		   //prepare grid
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

void renderGame(const char g[][SIZEX], const string& mess, const string& score, const string& mouseCount, const string& highScore, const int& inv, const string& name, int& countdownTimer, const bool& timeInitilized, const int& startTime, const int& countdownTime, goBundle& go, const int& currentLevel)
{ //display game title, messages, maze, spot and other items on screen
	if (timeInitilized && !go.gameOver) {
		countdownTimer = (countdownTime - (getIntTime() - startTime));
	}
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
	showMessage(clBlack, clWhite, 40, 7, mess);	//display current message
	showMessage(clBlack, clWhite, 40, 8, "Player Name: " + name);
	showMessage(clBlack, clWhite, 40, 9, "Current Level: " + tostring(currentLevel));
	showMessage(clBlack, clWhite, 40, 10, "Current High Score: " + highScore);
	showMessage(clBlack, clWhite, 40, 11, "Countdown Timer: " + secondsToString(countdownTimer) + "                          ");
	showMessage(clBlack, clWhite, 40, 12, score);
	showMessage(clBlack, clWhite, 40, 13, mouseCount);
  inv > 0 ? showMessage(clBlack, clWhite, 40, 14, "Invincible") : showMessage(clBlack, clWhite, 40, 14, "                        ");


	//display grid contents
	paintGrid(g, inv);
}


//creates two strings 1 for showing score the other for showing the amount of mice collected
void makeString(const int & score, const int & mice, string& stringScore, string& stringMouse, const int& mouseLevelCount) {
	ostringstream sout;
	istringstream sin;
	sout << "Score:" << score;
	stringScore = sout.str();
	sout.str("");
	sout << mice << " mice collected out of " << to_string(mouseLevelCount);
	stringMouse = sout.str();
	sout.str("");
}

void endProgram(const goBundle& go)
{
	clrscr();
	showMessage(clRed, clYellow, 36, 10, go.message);
	gotoxy(36, 12);
	system("pause");	//hold output screen until a keyboard key is hit
	selectBackColour(clBlack);
}

int selectInitialLevel(const string& name, bool& hasCheated) {
	int maxLevel = getAvailableLevel(name);
	int input = -1;
	clrscr();
	switch (maxLevel) {
	case 1 :
		showMessage(clRed, clYellow, 36, 6, "To activate cheat mode, enter 6");
		showMessage(clRed, clYellow, 36, 10, "Welcome to the game, please press 1 to start");
		gotoxy(36, 11);
		cin >> input;
		while (input != 1 && input != 6) {
			showMessage(clRed, clYellow, 36, 9, "ERROR, please select the correct number");
			cin >> input;
		}
		break;
	case 2 :
		showMessage(clRed, clYellow, 36, 6, "To activate cheat mode, enter 6");
		showMessage(clRed, clYellow, 36, 9, "Welcome to the game, please select a level");
		showMessage(clRed, clYellow, 36, 10, "Press 1 to play level 1 again");
		showMessage(clRed, clYellow, 36, 11, "Press 2 to play level 2");
		gotoxy(36, 12);
		cin >> input;
		while (input != 1 && input != 2 && input != 6) {
			showMessage(clRed, clYellow, 36, 8, "ERROR, please select the correct number");
			cin >> input;
		}
		break;
	case 3 :
		showMessage(clRed, clYellow, 36, 6, "To activate cheat mode, enter 6");
		showMessage(clRed, clYellow, 36, 9, "Welcome to the game, please select a level");
		showMessage(clRed, clYellow, 36, 10, "Press 1 to play level 1 again");
		showMessage(clRed, clYellow, 36, 11, "Press 2 to play level 2 again");
		showMessage(clRed, clYellow, 36, 12, "Press 3 to play level 3");
		gotoxy(36, 13);
		cin >> input;
		while (input != 1 && input != 2 && input != 3 && input != 6) {
			showMessage(clRed, clYellow, 36, 8, "ERROR, please select the correct number");
			cin >> input;
		}
		break;
	}
	selectBackColour(clBlack);
	if (input == 6) {
		clrscr();
		hasCheated = true;
		showMessage(clRed, clYellow, 36, 9, "Welcome to cheat mode, please select a level");
		showMessage(clRed, clYellow, 36, 10, "Press 1 to play level 1");
		showMessage(clRed, clYellow, 36, 11, "Press 2 to play level 2");
		showMessage(clRed, clYellow, 36, 12, "Press 3 to play level 3");
		gotoxy(36, 13);
		cin >> input;
		while (input != 1 && input != 2 && input != 3) {
			showMessage(clRed, clYellow, 36, 8, "ERROR, please select the correct number");
			cin >> input;
		}
	}
	selectBackColour(clBlack);
	return input;
}

int selectLevel(const string& name, bool& hasCheated) {
	int maxLevel = getAvailableLevel(name);
	int input = -1;
	clrscr();
	switch(maxLevel) {
	case 2 : 
		showMessage(clRed, clYellow, 36, 6, "To activate cheat mode, enter 6");
		showMessage(clRed, clYellow, 36, 9, "Level 1 completed");
		showMessage(clRed, clYellow, 36, 10, "Press 1 to play level 1 again");
		showMessage(clRed, clYellow, 36, 11, "Press 2 to play level 2");
		showMessage(clRed, clYellow, 36, 12, "Press 9 to quit");
		gotoxy(36, 13);
		cin >> input;
		while (input != 1 && input != 2 && input != 9 && input != 6) {
			showMessage(clRed, clYellow, 36, 8, "ERROR, please select the correct number");
			cin >> input;
		}
		break;
	case 3 :
		showMessage(clRed, clYellow, 36, 6, "To activate cheat mode, enter 6");
		showMessage(clRed, clYellow, 36, 8, "Level 2 completed");
		showMessage(clRed, clYellow, 36, 9, "Press 1 to play level 1");
		showMessage(clRed, clYellow, 36, 10, "Press 2 to play level 2 again");
		showMessage(clRed, clYellow, 36, 11, "Press 3 to play level 3");
		showMessage(clRed, clYellow, 36, 12, "Press 9 to quit");
		gotoxy(36, 13);
		cin >> input;
		while (input != 1 && input != 2 && input!= 3 && input != 9 && input != 6) {
			showMessage(clRed, clYellow, 36, 7, "ERROR, please select the correct number");
			cin >> input;
		}
		break;
	}
	selectBackColour(clBlack);
	if (input == 6) {
		clrscr();
		hasCheated = true;
		showMessage(clRed, clYellow, 36, 9, "Welcome to cheat mode, please select a level");
		showMessage(clRed, clYellow, 36, 10, "Press 1 to play level 1");
		showMessage(clRed, clYellow, 36, 11, "Press 2 to play level 2");
		showMessage(clRed, clYellow, 36, 12, "Press 3 to play level 3");
		gotoxy(36, 13);
		cin >> input;
		while (input != 1 && input != 2 && input != 3) {
			showMessage(clRed, clYellow, 36, 8, "ERROR, please select the correct number");
			cin >> input;
		}
	}
	selectBackColour(clBlack);
	return input;
}

void paintGrid(const char g[][SIZEX], const int& inv)
{ //display grid content on screen
	selectBackColour(clBlack);
	gotoxy(0, 2);
	int colour = 1;
	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col) {
			if (g[row][col] == SPOT || g[row][col] == BODY) {
				inv > 0 ? selectTextColour(11) : selectTextColour(colour);
			}
			else {
				selectTextColour(colour);
			}
			cout << g[row][col];	//output cell content
			colour++;
			if (colour > 15)
				colour = 1;
		}
		cout << endl;
		colour++;
		if (colour > 15)
			colour = 1;
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

void setInitialMazeStructure(char maze[][SIZEX], int& mouseLevelCount, const int& currentLevel, int& countdownTime)
{ //set the position of the walls in the maze
	int initialMaze[SIZEY][SIZEX]; //local array to store the maze structure

	if (currentLevel == 1) {
		ifstream file{ "level1.maz" };
		for (int i = 0; i < SIZEY; i++) {
			for (int j = 0; j < SIZEX; j++) {
			file >> initialMaze[i][j];
			}
		}
		file >> mouseLevelCount;
		file >> countdownTime;
	}
	if (currentLevel == 2) {
		ifstream file{ "level2.maz" };
		for (int i = 0; i < SIZEY; i++) {
			for (int j = 0; j < SIZEX; j++) {
				file >> initialMaze[i][j];
			}
		}
		file >> mouseLevelCount;
		file >> countdownTime;
	}
	if (currentLevel == 3) {
		ifstream file{ "level3.maz" };
		for (int i = 0; i < SIZEY; i++) {
			for (int j = 0; j < SIZEX; j++) {
				file >> initialMaze[i][j];
			}
		}
		file >> mouseLevelCount;
		file >> countdownTime;
	}
  //initialise maze configuration
		/*= {
			{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };/**/
	//with '#' for wall, ' ' for tunnel, etc. 
	//copy into maze structure with appropriate symbols
	

	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (initialMaze[row][col])
			{
				//not a direct copy, in case the symbols used are changed
			case 1: maze[row][col] = WALL; break;
			case 0: maze[row][col] = TUNNEL; break;
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
	case UP:		//when UP arrow pressed
		dx = 0;
		dy = -1;
		break;
	case DOWN:		//when DOWN arrow pressed
		dx = 0;
		dy = +1;
		break;
	}
}

string HighScore(const string& name) {
	string highScore;
	string temp;
	ifstream output;
	string fileName = name + ".txt";
	output.open(fileName, ios::app);

	getline(output, temp);
	highScore = temp;
	output.close();

	return highScore;
}

void writeToFile(const int& score, const string& name, const int& level) {
	ifstream input;

	string currentHS;
	string fileName = name + ".txt";
	input.open(fileName, ios::in);
	string strScore = to_string(score);
	getline(input, currentHS);
	input.close();

	ofstream output;

	if (stoi(currentHS) < score) {
		output.open(fileName, ios::out);
		output << strScore << endl;
		output << level;
		output.close();
	}
	else
	{
		output.open(fileName, ios::out);
		output << currentHS << endl;
		output << level;
		output.close();
	}

	
}

string getHighScore(const string& name) {
	string score;
	ifstream input;
	string currentHS;
	string fileName = name + ".txt";
	input.open(fileName, ios::in);
	getline(input, currentHS);
	score = currentHS;
	input.close();

	return score;
}

int getAvailableLevel(const string& name) {
	ifstream input;
	string level;
	int intLevel;
	string fileName = name + ".txt";
	input.open(fileName, ios::in);
	getline(input, level);
	getline(input, level);
	intLevel = stoi(level);
	input.close();

	return intLevel;
}

void initilizeFile(const string& name) {
	ofstream output;
	string fileName = name + ".txt";
	output.open(fileName, ios::app);
	output << 0 << endl;
	output << 1;
	output.close();
}

inline bool doesItExist(const string& name) {
	ifstream input;
	string fileName = name + ".txt";
	input.open(fileName, ios::in);
	return !input.fail();
}

bool saveCheck(const string& name) {
  ifstream input;
  string fileName = name + ".save";
  input.open(fileName, ios::in);
  return !input.fail();
}

string getName() {
	string name;

	showMessage(clRed, clYellow, 22, 10, "Enter your name <MAX 20 Charcters>: ");
	gotoxy(40, 11);
	cin >> name;
	while (name.length() > 20)
	{
		cout << "Enter your name <MAX 20 Charcters>: ";
		cin >> name;
	}
	selectBackColour(clBlack);
	clrscr();

	return name;
}

void saveGame(Snake & spot, const Item & mouse, const Item & power, const Item& M1, const Item& M2, const int& mouseLevelCount, const int& currentLevel, const int& countdownTime, const string& name, const int& score, const int& mouseCount, const int& countdownTimer, const int& target) {
  ofstream out;
  out.open(name + ".save", ios::out);
  spot.readout(out);
  out << currentLevel << endl;
  out << score << endl;
  out << mouseCount << endl;
  out << countdownTimer << endl;
  out << mouse.x << endl;
  out << mouse.y << endl;
  out << mouse.symbol << endl;
  out << M1.x << endl;
  out << M1.y << endl;
  out << M1.symbol << endl;
  out << M2.x << endl;
  out << M2.y << endl;
  out << M2.symbol << endl;
  out << mouseLevelCount << endl;
  out << target << endl;
  out.close();
}


void loadSave(Snake & spot, Item & mouse, Item & power, Item& M1, Item& M2, int& mouseLevelCount, int& currentLevel, int& countdownTime, string& name, int& score, int& mouseCount, int& countdownTimer, char grid[][SIZEX], char maze[][SIZEX], int& target) {
  ifstream in;
  in.open(name + ".save", ios::in);
  string tmp_spot;
  string tmp_currentLevel;
  string tmp_score;
  string tmp_mouseCount;
  string tmp_countdownTimer;
  string tmp_mousex;
  string tmp_mousey;
  string tmp_mousesymbol;
  string tmp_M1x;
  string tmp_M1y;
  string tmp_M1symbol;
  string tmp_M2x;
  string tmp_M2y;
  string tmp_M2symbol;
  string tmp_mouseLevelCount;
  string tmp_target;

  getline(in, tmp_spot);
  spot.readin(tmp_spot);
  
  getline(in, tmp_currentLevel);
  getline(in, tmp_score);
  getline(in, tmp_mouseCount);
  getline(in, tmp_countdownTimer);
  getline(in, tmp_mousex);
  getline(in, tmp_mousey);
  getline(in, tmp_mousesymbol);
  getline(in, tmp_M1x);
  getline(in, tmp_M1y);
  getline(in, tmp_M1symbol);
  getline(in, tmp_M2x);
  getline(in, tmp_M2y);
  getline(in, tmp_M2symbol);
  getline(in, tmp_mouseLevelCount);
  getline(in, tmp_target);
  in.close();

  currentLevel = stoi(tmp_currentLevel);
  score = stoi(tmp_score);
  mouseCount = stoi(tmp_mouseCount);
  countdownTimer = stoi(tmp_countdownTimer);
  mouse.x = stoi(tmp_mousex);
  mouse.y = stoi(tmp_mousey);
  mouse.symbol = tmp_mousesymbol.at(0);
  M1.x = stoi(tmp_M1x);
  M1.y = stoi(tmp_M1y);
  M1.symbol = tmp_M1symbol.at(0);
  M2.x = stoi(tmp_M2x);
  M2.y = stoi(tmp_M2y);
  M2.symbol = tmp_M2symbol.at(0);
  mouseLevelCount = stoi(tmp_mouseLevelCount);
  target = stoi(tmp_target);

  setInitialMazeStructure(maze, mouseLevelCount, currentLevel, countdownTime);		   //initialise maze
  updateGrid(grid, maze, spot, mouse, power, M1, M2);		   //prepare grid
  remove((name + ".save").c_str());
}