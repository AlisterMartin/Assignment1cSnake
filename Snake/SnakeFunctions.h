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

struct Item;

class Snake;

void paintGrid(const char g[][SIZEX]);

bool isArrowKey(const int key);

void newMouse(Item& mouse, const char maze[][SIZEX]);

void setInitialMazeStructure(char maze[][SIZEX]);

//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------

void setKeyDirection(const int key, int& dx, int& dy);



//---------------------------------------------------------------------------
//----- Update Game
//---------------------------------------------------------------------------

void updateGameData(const char g[][SIZEX], Snake& spot, const int key, string& mess, int& score, Item& mouse, int& target, int& mouseCount, bool& gameOver, const bool& cheat);

void placeItem(char g[][SIZEX], const Item& item);

void placeMaze(char grid[][SIZEX], const char maze[][SIZEX]);

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], Snake& spot, const Item& mouse);

void updateGame(char grid[][SIZEX], const char maze[][SIZEX], Snake& spot, const int keyCode, string& mess, int& score, Item& mouse, int& target, int& mouseCount, bool& gameOver, const bool& cheat);

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