/*
* C++ Final Project
* By Sid S. and Rohit J.
* Jalebi Disco Snake Game
*/

//necessary libraries for rand, time, and console control
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

//define namespace
using namespace std;

//tracks whether game is over or not and decides to exit loop
bool gameCondition;

//Border boardWidth and boardHeight(Game Board Border)
const int boardWidth = 30;
const int boardHeight = 20;

/*
* Random position of fruit(x, y) coordinate
* score variable tracks score by increments of 10(1 fruit = 10 pts)
*/
int xPos, yPos, jalebiXpos, jalebiYpos, score;
int tailXPos[100], tailYPos[100];
int tailLength;

/*
* Allows user to pick the snake head and tail(For personalization)
*/
char head, tail;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

/*
* Sets all the variables values to start the game
* Gives fruit random position on the board.
* Keeps the snake standing still when you begin playing(dir = STOP)
* gameCondition is false since we want the program to be running and the game to be online
*/
void Setup()
{
	gameCondition = false;
	dir = STOP;
	xPos = boardWidth / 2;
	yPos = boardHeight / 2;
	jalebiXpos = rand() % boardWidth;
	jalebiYpos = rand() % boardHeight;
	score = 0;
}

/*
* Method that changes console color(check for credits at the end of the program)
*/
void changeColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

/*
* Creates the board (height x width)
* The char(number) accesses the ASCII table character values which creates the border for the board
* Places the jalebis on random positions on the board
* Draws the head and tails (depends on how many jalebis the snake ate)
* Prints the score
*/
void Draw()
{
	//reduce glitchiness from past version sys(cls)
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 }); //system("clear");
	for (int i = 0; i < boardWidth + 2; i++)
		cout << char(254);
	cout << endl;

	for (int i = 0; i < boardHeight; i++)
	{
		for (int j = 0; j < boardWidth; j++)
		{
			if (j == 0)
				cout << char(219);
			if (i == yPos && j == xPos)
				cout << head;
			else if (i == jalebiYpos && j == jalebiXpos)
				cout << "@";
			else
			{
				bool move = false;
				for (int k = 0; k < tailLength; k++)
				{
					if (tailXPos[k] == j && tailYPos[k] == i)
					{
						cout << tail;
						move = true;
					}
				}
				if (!move)
					cout << " ";
			}


			if (j == boardWidth - 1)
				cout << char(219);
		}
		cout << endl;
	}

	for (int i = 0; i < boardWidth + 2; i++)
		cout << char(254);
	cout << endl;
	cout << "Score: " << score << endl;
}

/*
* Everytime the players eats a jalebi, we wanted to change the color of the board
* This function accesses the windows console application library, and makes random color values
*/
void randomColor()
{
	int color = rand() % 15 + 1;
	changeColor(color);
}

/*
* In order to check the movement, we had 8 case keys (including caps lock)
* Up, Down, Left, Right, and exit the game (x or X)
* The kbhit function sees whether the user pressed something on the keyboard
* The getch function then sees what character the user pressed
*/
void Input()
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			dir = LEFT;
			break;
		case 'A':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'D':
			dir = RIGHT;
			break;
		case 'w':
			dir = UP;
			break;
		case 'W':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'S':
			dir = DOWN;
			break;
		case 'x':
			gameCondition = true;
			break;
		case 'X':
			gameCondition = true;
			break;

		}
	}
}
/* 
* This function was one of the most important part of the game as it basically determined whether the rules were followed or not
* Since we decided to recreate the original snake game, we did not care if the snake went through the border
* We just needed to make sure that it returned from the other side (Goes into right border, comes out of the left)
* If the snake hit himself, we needed to end the game as it was the only way to lose
*/
void Logic()
{
	int prevX = tailXPos[0];
	int prevY = tailYPos[0];
	int prev2X, prev2Y;
	tailXPos[0] = xPos;
	tailYPos[0] = yPos;
	for (int i = 1; i < tailLength; i++)
	{
		prev2X = tailXPos[i];
		prev2Y = tailYPos[i];
		tailXPos[i] = prevX;
		tailYPos[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	switch (dir)
	{
	case LEFT:
		xPos--;
		break;
	case RIGHT:
		xPos++;
		break;
	case UP:
		yPos--;
		break;
	case DOWN:
		yPos++;
		break;
	default:
		break;
	}

	if (xPos >= boardWidth) 
		xPos = 0; 
	else if (xPos < 0) 
		xPos = boardWidth - 1;

	if (yPos >= boardHeight) 
		yPos = 0; 
	else if (yPos < 0) 
		yPos = boardHeight - 1;

	for (int i = 0; i < tailLength; i++)
		if (tailXPos[i] == xPos && tailYPos[i] == yPos) {
			gameCondition = true;
		}
//This is where the board changes its color evertime a jalebi is eaten
	if (xPos == jalebiXpos && yPos == jalebiYpos)
	{
		score += 10;
		randomColor();
		jalebiXpos = rand() % boardWidth;
		jalebiYpos = rand() % boardHeight;
		tailLength++;
	}
}

//Once the game is over, this end function prints out how many jalebis the user ate and how to exit the console application
void End()
{
	cout << "You collected " << (int)(score / 10) << " jalebis!\n\n" << endl;
	cout << "Press any key to exit!";
}

/*
* This method sets the initial color of the console(Green)
* It then explains all the rules of the game
*/
void Start()
{
	changeColor(2);
	cout << "Welcome to Snake Game! Basically, you are controlling a snake using WASD\n";
	cout << "Objective: The main objective of this game is to see how many jalebis you can collect with your cobra.\n";
	cout << "Rules: If you run into your own body, you lose. You can traverse through walls(like a real snake ofc)\n";
	cout << "W - Up" << endl << "A - Left" << endl << "S - Down" << endl << "D - Right" << endl;

}

/*
* The main function then puts all of these functions together
* It asks the user whether they want to customize their snake or not
* If yes, they can set what character they want for the head and tail
* If no, we give the default snake to them (square characters)
* Starts the game automatically once you have finished all the inputs
*/
int main()
{
	char choice;
	Start();
	cout << "Do you want to customize your snake?(Y or N): ";
	cin >> choice;
	if (choice == 'Y' || choice == 'y')
	{
		cout << "Enter what character you want for your head: " << endl;
		cin >> head;
		cout << "Enter what character you want for your tail: " << endl;
		cin >> tail;
	}
	else
	{
		head = char(223);
		tail = char(223);
	}

	if (_kbhit)
	{
		changeColor(15);
		system("CLS");
		Setup();
		while (!gameCondition)
		{
			Draw();
			Input();
			Logic();

			//sets a delay between each loop
			Sleep(10);
		}
		End();

	}
	return 0;

}
/*
* Credits
* StackOverflow: Color changing method
* GitHub programmers: Certain functions and methods to help us run the code smoothly
* GeekforGeeks: Helped us with direction methods
*/
