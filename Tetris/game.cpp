#include <iostream>
#include <conio.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <Windows.h>
#include "leaderboard.h"
#include "menu.h"
#include "gameMenu.h"

using namespace std;

// get handle of the screen to change colors and sizes
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// colors code
const int WHITE = 7;
const int CYAN = 11;
const int DARK_BLUE = 1;
const int ORANGE = 6;
const int YELLOW = 14;
const int GREEN = 10;
const int PURPLE = 5;
const int RED = 4;
const int GRAY = 8;

struct GameSettings
{
	// set configuration to play
	string username;
	int width;
	int height;
	int level;

};

struct Location
{
	// coordination of blocks
	int i; // row
	int j; // column
};

struct Records {
	// info to save
	string user;
	int point;
	int time;
	int m; // rows
	int n; // columns
	int level;
};

/////////////////////////////////
// Functions
/////////////////////////////////

GameSettings getGameSettings();
void loading(); // load before starting game
void game(); // main func
void load(); // load game from previous save
void makeSolid(); // freeze blocks on the bottom
void makeSolidFor(bool& canShiftD); // check solidity while shifting down
void fall(); // fall down the block at once
void shiftR(); // shift to right
void shiftL(); // shift to left
void shiftD(); // move down the block
void turnR(); // rotate clockwise
void turnL(); // rotate anticlockwise
void insertShape(); // insert the new shape on board
void shapeRand(int** shape, Location& cm, int& index); // create a random shape
int getColor(int index); // returns the color code
void gameSave(); // save the current game
void printMainBoarder(); //
void printPoints();
void printNextShape();
void printLevel();
void fadeRow(); // fade a completed row
void makeShadow(); // show shadow of the blocks on bottom
void saveRecord(); // save info on exit
void printGameOver();
void restartGame();


int** board; // main game board
int** shape; // shape
int** newShape; // next shape
int** colors; // colors board
int shapeIndex; // number related to shape
int newShapeIndex; // number related to shape that will come
int rotateIndex = 0; // situation of shape for rotation
Location CM, newCM; // center of mass
int w, h, x;
int level;
string name;
bool remainInGame = true;
bool exitGame = false;
bool gameOverBool = false; // becomes true when gameover
int points = 0;
int rows = 0, rowsToShow;
int now = 0;
int duration = 0; // time in game
int lastGameTime = 0;
bool restart = false;

void loading() {
	printMainBoarder();
	// print blocks one by one
	setCursorLoc(6, 3 + h);
	for (int i = 0; i < h; i++) {
		setCursorLoc(6, 3 + h - i);
		for (int j = 0; j < w; j++) {
			cout << "\u2588" << "\u2588";
			Sleep(5);
		}
	}
	// fade blocks one by one
	setCursorLoc(6, 3 + h);
	for (int i = 0; i < h; i++) {
		setCursorLoc(6, 3 + h - i);
		for (int j = 0; j < w; j++) {
			cout << "  ";
			Sleep(5);
		}
	}
	PlaySound(TEXT("Start.wav"), NULL, SND_FILENAME | SND_ASYNC);
	Sleep(200);
}

int c = 0; // just one time initialize the seed

void game() {
	if (c == 0) {
		srand(static_cast<unsigned int>(time(nullptr))); // random seed
		c++;
	}
	int counter = 1;
	remainInGame = true;

	if (continueGame) {
		load();
		loading();
		continueGame = false;
	}
	
	else {
		GameSettings gs = getGameSettings();

		h = gs.height;
		w = gs.width;
		name = gs.username;
		level = gs.level;
		points = 0;
		rows = 0;
		rowsToShow = 0;
		lastGameTime = 0;

		// loading
		loading();

		// main board
		board = new int* [h + 1];
		for (int i = 0; i <= h; i++) {
			board[i] = new int[w];
			for (int j = 0; j < w; j++) {
				board[i][j] = 0;
			}
		}
		// fill the last row with solid blocks
		for (int j = 0; j < w; j++) {
			board[h][j] = 2;
		}

		// colors board
		colors = new int* [h];
		for (int i = 0; i < h; i++) {
			colors[i] = new int[w];
			for (int j = 0; j < w; j++) {
				colors[i][j] = 7;
			}
		}

		// shape
		shape = new int* [4];
		for (int i = 0; i < 4; i++) {
			shape[i] = new int[4];
			for (int j = 0; j < 4; j++)
				shape[i][j] = 0;
		}

		// next shape
		newShape = new int* [4];
		for (int i = 0; i < 4; i++) {
			newShape[i] = new int[4];
			for (int j = 0; j < 4; j++)
				newShape[i][j] = 0;
		}

		x = (w - 4) / 2;
		// create the first and second shapes
		shapeRand(shape, CM, shapeIndex);
		shapeRand(newShape, newCM, newShapeIndex);
		insertShape();
	}
	// boxes on the right
	printPoints();
	printNextShape();
	printLevel();

	int start = time(NULL);

	while (remainInGame) {
		makeShadow();
		// print all the blocks on the board
		setCursorLoc(6, 4);
		for (int i = 0; i < h; i++) {
			setCursorLoc(6, 4 + i);
			for (int j = 0; j < w; j++) {
				if (board[i][j] == 1) {
					SetConsoleTextAttribute(hConsole, getColor(shapeIndex));
					cout << "\u2588\u2588";
				}
				else if (board[i][j] == 2) {
					SetConsoleTextAttribute(hConsole, getColor(colors[i][j]));
					cout << "\u2588\u2588";
				}
				else if (board[i][j] == 3) {
					SetConsoleTextAttribute(hConsole, GRAY);
					cout << "\u2588\u2588";
					board[i][j] = 0;
				}
				else {
					cout << "  ";
				}
			}
		}
		// set time every loop
		duration = time(NULL) - start;
		// happens to be used only when started from a loaded game
		now = duration + lastGameTime;
		
		SetConsoleTextAttribute(hConsole, WHITE);
		setCursorLoc(11 + w * 2, 4);
		cout << "Time: ";
		setCursorLoc(11 + w * 2, 5);
		if (now < 60)
			cout << now;
		else
			cout << now / 60 << ":" << now % 60;
		setCursorLoc(11 + w * 2, 6);
		cout << "Points: ";
		setCursorLoc(11 + w * 2, 7);
		cout << points;

		// get keys and do something
		if (_kbhit()) {
			char choice = _getch();
			if (choice == 's' || choice == 'S') {
				shiftD();
			}
			else if (choice == 'a' || choice == 'A') {
				shiftL();
			}
			else if (choice == 'd' || choice == 'D') {
				shiftR();
			}
			else if (choice == 'q' || choice == 'Q') {
				turnL();
			}
			else if (choice == 'e' || choice == 'E') {
				turnR();
			}
			else if (choice == ' ') {
				fall();
			}
			else if (choice == 27) {
				SetConsoleTextAttribute(hConsole, WHITE);
				setCursorLoc(0, 0);
				system("cls");
				PlaySound(TEXT("click2.wav"), NULL, SND_FILENAME | SND_ASYNC);
				pauseMenu();
				lastGameTime += now;
				start = time(NULL);
				if (restart) {
					restartGame();
					restart = false;
				}
			}
		}

		// shift down if possible
		bool canShiftD = true;
		if (counter % 10 == 0) {
			makeSolidFor(canShiftD);
			if (canShiftD)
				shiftD();
			counter = 0;
		}

		counter++;
		Sleep(60 - 4 * level); // sleeps based on level
	}
	
	system("cls");
	setCursorLoc(0, 0);
	PlaySound(TEXT("main.wav"), NULL, SND_FILENAME | SND_ASYNC);

	if (!gameOverBool) {
		gameSave();

		for (int i = 0; i < h + 1; i++) {
			delete[] board[i];
		}
		delete[] board;
		for (int i = 0; i < h; i++) {
			delete[] colors[i];
		}
		delete[] colors;
		for (int i = 0; i < 4; i++) {
			delete[] shape[i];
		}
		delete[] shape;
		for (int i = 0; i < 4; i++) {
			delete[] newShape[i];
		}
		delete[] newShape;

		if (exitGame)
			exit(1);
	}
	else {

		for (int i = 0; i < h + 1; i++) {
			delete[] board[i];
		}
		delete[] board;
		for (int i = 0; i < h; i++) {
			delete[] colors[i];
		}
		delete[] colors;
		for (int i = 0; i < 4; i++) {
			delete[] shape[i];
		}
		delete[] shape;
		for (int i = 0; i < 4; i++) {
			delete[] newShape[i];
		}
		delete[] newShape;

		printGameOver();
	}
}

// insert new shape and checks if shifting down is possible anymore
void makeSolidFor(bool& canShiftD) {
	for (int i = h-1; i >= 0; i--) {
		for (int j = 0; j < w; j++) {
			if (board[i+1][j] == 2 && board[i][j] == 1) {
				makeSolid();
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++)
						shape[i][j] = newShape[i][j]; // replace the old shape with new one
				}
				// shape's center of mass
				CM.i = newCM.i;
				CM.j = newCM.j;
				shapeIndex = newShapeIndex;
				insertShape();
				shapeRand(newShape, newCM, newShapeIndex);
				canShiftD = false;
				rotateIndex = 0;
				if (gameOverBool)
					printNextShape();
				return;
			}
		}
	}
}

GameSettings getGameSettings() {
	GameSettings gs;

	system("cls");
	printBorder();

	printGameLogo();

	// username
	do {
		setCursorLoc(4, 8);
		for (int i = 0; i < 50; i++)
			cout << " ";
		setCursorLoc(4, 8);
		cout << ">> Please Enter Your Name: ";
		getline(cin, gs.username);
	} while (gs.username.length() == 0);

	// width
	do {
		setCursorLoc(4, 10);
		for (int i = 0; i < 50; i++)
			cout << " ";
		setCursorLoc(4, 10);
		cout << ">> Enter Game Width: ";
		cin >> gs.width;
	} while (gs.width < 5 || gs.width > 20);

	//height
	do {
		setCursorLoc(4, 12);
		for (int i = 0; i < 50; i++)
			cout << " ";
		setCursorLoc(4, 12);
		cout << ">> Enter Game Height: ";
		cin >> gs.height;
	} while (gs.height < 5 || gs.height > 20);

	// level
	do {
		setCursorLoc(4, 14);
		for (int i = 0; i < 50; i++)
			cout << " ";
		setCursorLoc(4, 14);
		cout << ">> Enter Game Level(1-10): ";
		cin >> gs.level;
	} while (gs.level < 1 || gs.level > 10);

	return gs;
}

void fall() {
	bool shiftable = true;
	// shift down while possible
	while (shiftable) {
		shiftD();

		for (int i = h - 1; i >= 0; i--) {
			for (int j = 0; j < w; j++) {
				if (board[i][j] == 1 && board[i + 1][j] == 2) {
					shiftable = false;
					break;
				}
			}
		}
	}
	// play sound when it reaches down
	PlaySound(TEXT("Fall.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void shiftR() {
	bool shiftable = true;
	// checks if shifting is possible
	for (int i = 0; i < h; i++) {
		for (int j = w - 1; j >= 0; j--) {
			if (board[i][j] == 1) {
				if (board[i][j + 1] == 2 || j + 1 >= w) {
					shiftable = false;
				}
			}
		}
	}

	if (shiftable) {
		for (int i = 0; i < h; i++) {
			for (int j = w - 1; j >= 0; j--) {
				if (board[i][j] == 1) {
					swap(board[i][j], board[i][j + 1]);
				}
			}
		}
		CM.j++;
	}
}

void shiftL() {
	bool shiftable = true;
	// chekcs if shifting is possible
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (board[i][j] == 1) {
				if (board[i][j - 1] == 2 || j - 1 < 0) {
					shiftable = false;
				}
			}
		}
	}

	if (shiftable) {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				if (board[i][j] == 1) {
					swap(board[i][j], board[i][j - 1]);
				}
			}
		}
		CM.j--;
	}
}

void shiftD() {
	// won't let the shape break into parts
	for (int i = h - 1; i >= 0; i--) {
		for (int j = 0; j < w; j++) {
			if (board[i][j] == 1 && board[i + 1][j] == 2) {
				return;
			}
		}
	}

	for (int i = h - 1; i >= 0; i--) {
		for (int j = 0; j < w; j++) {
			if (board[i][j] == 1 && board[i + 1][j] != 2) {
				swap(board[i][j], board[i + 1][j]);
			}
		}
	}
	CM.i++;

	points += level;
}


void turnR() {
	// square won't rotate
	if (shapeIndex == 1)
		return;
	// makes a copy of main board to check
	int** temp = new int* [h + 1];
	for (int i = 0; i <= h; i++) {
		temp[i] = new int[w];
		for (int j = 0; j < w; j++)
			temp[i][j] = board[i][j];
	}


	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (temp[i][j] == 1) {
				temp[i][j] = 0;
			}
		}
	}

	if (shapeIndex == 0) {
		switch (rotateIndex)
		{
		case 0:
			if (CM.i - 1 < 0 || CM.i + 2 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 2][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 2][CM.j] = 1;
			rotateIndex++;
			break;
		case 1:
			if (CM.j - 2 < 0 || CM.j + 1 >= w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i][CM.j - 2] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i][CM.j - 2] = 1;
			rotateIndex++;
			break;
		case 2:
			if (CM.i - 2 < 0 || CM.i + 1 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 2][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 2][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			rotateIndex++;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.j + 2 >= w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j + 2] == 2 || temp[CM.i][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j + 2] = 1;
			temp[CM.i][CM.j - 1] = 1;
			rotateIndex = 0;
			break;
		}
	}

	else if (shapeIndex == 2) {
		switch (rotateIndex)
		{
		case 0:
			if (CM.i - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i - 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			rotateIndex++;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex++;
			break;
		case 2:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 >= w || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 >= w || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j - 1] == 2 || temp[CM.i][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			temp[CM.i][CM.j + 1] = 1;
			rotateIndex = 0;
			break;
		}
	}

	else if (shapeIndex == 3) {
		switch (rotateIndex)
		{
		case 0:
			if (CM.i - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex++;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i][CM.j + 1] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 2:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.i + 1 >= h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 >= w || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			rotateIndex = 0;
			break;
		}
	}

	else if (shapeIndex == 4) {
		switch (rotateIndex)
		{
		case 0:
			if (CM.i - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j + 1] == 2 || temp[CM.i + 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			rotateIndex++;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i][CM.j - 1] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex++;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.i + 1 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 3:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.j + 1 >= w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			rotateIndex = 0;
			break;
		}
	}

	else if (shapeIndex == 5) {
		switch (rotateIndex)
		{
		case 0:
			if (CM.i - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex++;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.i + 1 >= h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 3:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.j + 1 >= w || temp[CM.i][CM.j - 1] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i - 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			rotateIndex = 0;
			break;
		}
	}

	else if (shapeIndex == 6) {
		switch (rotateIndex)
		{
		case 0:
			if (CM.i - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			rotateIndex++;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			rotateIndex++;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.i + 1 >= h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 3:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.j + 1 >= w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j] = 1;
			rotateIndex = 0;
			break;
		}
	}

	for (int i = 0; i <= h; i++) {
		for (int j = 0; j < w; j++)
			board[i][j] = temp[i][j];
	}
	
	// delete copied board
	for (int i = 0; i < h; i++)
		delete[] temp[i];
	delete[] temp;
	PlaySound(TEXT("Fall.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void turnL() {
	// square won't rotate
	if (shapeIndex == 1)
		return;
	// makes a copy of main board
	int** temp = new int* [h + 1];
	for (int i = 0; i <= h; i++) {
		temp[i] = new int[w];
		for (int j = 0; j < w; j++)
			temp[i][j] = board[i][j];
	}

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (temp[i][j] == 1) {
				temp[i][j] = 0;
			}
		}
	}

	if (shapeIndex == 0) {
		switch (rotateIndex)
		{
		case 0:
			if (CM.i - 2 < 0 || CM.i + 1 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 2][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 2][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			rotateIndex = 3;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.j + 2 >= w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j + 2] == 2 || temp[CM.i][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j + 2] = 1;
			temp[CM.i][CM.j - 1] = 1;
			rotateIndex--;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.i + 2 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 2][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 2][CM.j] = 1;
			rotateIndex--;
			break;
		case 3:
			if (CM.j - 2 < 0 || CM.j + 1 >= w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i][CM.j - 2] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i][CM.j - 2] = 1;
			rotateIndex--;
			break;
		}
	}

	else if (shapeIndex == 2) {
		switch (rotateIndex)
		{
		case 0:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.i + 1 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex = 3;
			break;
		case 1:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.j + 1 >= w || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j - 1] == 2 || temp[CM.i][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			temp[CM.i][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j + 1 >= w || temp[CM.i + 1][CM.j] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i - 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex--;
			break;
		}
	}

	else if (shapeIndex == 3) {
		switch (rotateIndex)
		{
		case 0:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.i + 1 >= h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			rotateIndex = 3;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 >= w || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i][CM.j + 1] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex--;
			break;
		}
	}

	else if (shapeIndex == 4) {
		switch (rotateIndex)
		{
		case 0:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.i + 1 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex = 3;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 >= w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			rotateIndex--;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j + 1] == 2 || temp[CM.i + 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			rotateIndex--;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i][CM.j - 1] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex--;
			break;
		}
	}

	else if (shapeIndex == 5) {
		switch (rotateIndex)
		{
		case 0:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.i + 1 >= h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			rotateIndex = 3;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 >= w || temp[CM.i][CM.j - 1] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i - 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex--;
			break;
		}
	}

	else if (shapeIndex == 6) {
		switch (rotateIndex)
		{
		case 0:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.i + 1 >= h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			rotateIndex = 3;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 >= w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j] = 1;
			rotateIndex--;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.j + 1 >= w || CM.i + 1 >= h || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			rotateIndex--;
			break;
		}
	}

	for (int i = 0; i <= h; i++) {
		for (int j = 0; j < w; j++)
			board[i][j] = temp[i][j];
	}

	// delete the copied board
	for (int i = 0; i < h; i++)
		delete[] temp[i];
	delete[] temp;

	PlaySound(TEXT("Fall.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void makeSolid() {
	// turns every 1 to 2
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (board[i][j] == 1) {
				board[i][j] = 2;
				colors[i][j] = shapeIndex;
			}
		}
	}
	// checks if row is completed
	fadeRow();
}

void insertShape() {
	// checks if game is over
	// if it can't insert the shape game will be over
	int J = 0;
	for (int i = 0; i < 2; i++) {
		for (int j = x; j < x + 4; j++) {
			if (shape[i][J] == 1 && board[i][j] == 2) {
				saveRecord();
				remainInGame = false;
				gameOverBool = true;
				return;
			}
			J++;
		}
		J = 0;
	}
	// iterates through the shape array and put ones into the main board
	J = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = x; j < x + 4; j++) {
			if (shape[i][J] == 1)
				board[i][j] = 1;
			J++;
		}
		J = 0;
	}
}

void shapeRand(int** shape, Location& cm, int& index) {
	// reset to 0
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			shape[i][j] = 0;
	}
	// choose a random shape
	int randNum = rand() % 7;
	index = randNum;
	switch (randNum)
	{
	case 0: // Line
		for (int i = 0; i < 4; i++)
			shape[0][i] = 1;
		cm.i = 0;
		cm.j = x + 1;
		break;
	case 1: // Square
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				shape[i][j] = 1;
		cm.i = 0;
		cm.j = x + 1;
		break;
	case 2: // z
		shape[0][0] = 1;
		shape[0][1] = 1;
		shape[1][1] = 1;
		shape[1][2] = 1;
		cm.i = 1;
		cm.j = x + 1;
		break;
	case 3: // reverse z
		shape[1][0] = 1;
		shape[1][1] = 1;
		shape[0][1] = 1;
		shape[0][2] = 1;
		cm.i = 1;
		cm.j = x + 1;
		break;
	case 4: // L
		shape[0][0] = 1;
		shape[1][0] = 1;
		shape[1][1] = 1;
		shape[1][2] = 1;
		cm.i = 1;
		cm.j = x + 1;
		break;
	case 5: // reverse L
		shape[1][0] = 1;
		shape[1][1] = 1;
		shape[1][2] = 1;
		shape[0][2] = 1;
		cm.i = 1;
		cm.j = x + 1;
		break;
	case 6: // rake
		shape[1][0] = 1;
		shape[1][1] = 1;
		shape[0][1] = 1;
		shape[1][2] = 1;
		cm.i = 1;
		cm.j = x + 1;
		break;
	}
}

int getColor(int index) {
	// choose the color of blocks based on shape's index
	switch (index)
	{
	case 0:
		return CYAN;

	case 1:
		return YELLOW;

	case 2:
		return RED;

	case 3:
		return GREEN;

	case 4:
		return DARK_BLUE;

	case 5:
		return ORANGE;

	case 6:
		return PURPLE;

	default:
		return GRAY;
	}
}

void gameSave() {
	ofstream save("save.txt", ios::out);
	save << name << endl << points << endl << now << endl << w << endl << h << endl << level << endl;
	save << shapeIndex << endl << rotateIndex << endl << CM.i << endl << CM.j << endl;
	for (int i = 0; i <= h; i++) {
		for (int j = 0; j < w; j++) {
			save << board[i][j];
		}
		save << endl;
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			save << colors[i][j];
		}
		save << endl;
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			save << shape[i][j];
		save << endl;
	}
	save.close();
}

void load() {
	ifstream loadGame("save.txt", ios::in);

	// name
	getline(loadGame, name);

	// scores
	string stringPoints;
	getline(loadGame, stringPoints);
	points = stoi(stringPoints);

	// last game time
	string stringLGT;
	getline(loadGame, stringLGT);
	lastGameTime = stoi(stringLGT);

	// game board width
	string stringW;
	getline(loadGame, stringW);
	w = stoi(stringW);

	// game board height
	string stringH;
	getline(loadGame, stringH);
	h = stoi(stringH);

	// game level
	string stringLevel;
	getline(loadGame, stringLevel);
	level = stoi(stringLevel);

	// shape index
	string stringShapeIndex;
	getline(loadGame, stringShapeIndex);
	shapeIndex = stoi(stringShapeIndex);

	// rotate index
	string stringRotateIndex;
	getline(loadGame, stringRotateIndex);
	rotateIndex = stoi(stringRotateIndex);

	// i of CM
	string stringCMI;
	getline(loadGame, stringCMI);
	CM.i = stoi(stringCMI);

	// j of CM
	string stringCMJ;
	getline(loadGame, stringCMJ);
	CM.j = stoi(stringCMJ);

	// game board
	board = new int* [h + 1];
	for (int i = 0; i <= h; i++) {
		board[i] = new int[w];
		for (int j = 0; j < w; j++) {
			board[i][j] = 0;
		}
	}

	// turn board rows into integer
	string* boardRows = new string[h + 1];
	for (int i = 0; i <= h; i++) {
		getline(loadGame, boardRows[i]);
		for (int j = 0; j < w; j++) {
			board[i][j] = boardRows[i][j] - '0';
		}
	}

	// game board colors
	colors = new int* [h];
	for (int i = 0; i < h; i++) {
		colors[i] = new int[w];
		for (int j = 0; j < w; j++) {
			colors[i][j] = 7;
		}
	}

	// color rows into integer
	string* colorRows = new string[h];
	for (int i = 0; i < h; i++) {
		getline(loadGame, colorRows[i]);
		for (int j = 0; j < w; j++) {
			colors[i][j] = colorRows[i][j] - '0';
		}
	}

	// shape
	shape = new int* [4];
	for (int i = 0; i < 4; i++) {
		shape[i] = new int[4];
		for (int j = 0; j < 4; j++)
			shape[i][j] = 0;
	}

	// shape rows into integer
	string* shapeRows = new string[4];
	for (int i = 0; i < 4; i++) {
		getline(loadGame, shapeRows[i]);
		for (int j = 0; j < 4; j++) {
			shape[i][j] = shapeRows[i][j] - '0';
		}
	}

	loadGame.close();

	// next shape
	newShape = new int* [4];
	for (int i = 0; i < 4; i++) {
		newShape[i] = new int[4];
		for (int j = 0; j < 4; j++)
			newShape[i][j] = 0;
	}

	x = (w - 4) / 2; // find the middle of the board
	shapeRand(newShape, newCM, newShapeIndex);
}

void printMainBoarder() {
	SetConsoleTextAttribute(hConsole, WHITE);

	system("cls");

	setCursorLoc(5, 3);
	cout << "\u2554";
	for (int i = 0; i < w * 2; i++) {
		cout << "\u2550";
	}
	cout << "\u2557";

	for (int i = 0; i < h; i++) {
		setCursorLoc(5, 4 + i);
		cout << "\u2551";
		setCursorLoc(6 + w * 2, 4 + i);
		cout << "\u2551";
	}

	setCursorLoc(5, 4 + h);
	cout << "\u255A";
	for (int i = 0; i < w * 2; i++) {
		cout << "\u2550";
	}
	cout << "\u255D";
}

void printPoints() {
	SetConsoleTextAttribute(hConsole, WHITE);

	setCursorLoc(9 + w * 2, 3);
	cout << "\u2554";
	for (int i = 0; i < 10; i++) {
		cout << "\u2550";
	}
	cout << "\u2557";

	for (int i = 0; i < 4; i++) {
		setCursorLoc(9 + w * 2, 4 + i);
		cout << "\u2551";
		setCursorLoc(20 + w * 2, 4 + i);
		cout << "\u2551";
	}

	setCursorLoc(9 + w * 2, 8);
	cout << "\u255A";
	for (int i = 0; i < 10; i++) {
		cout << "\u2550";
	}
	cout << "\u255D";
}

void printNextShape() {
	SetConsoleTextAttribute(hConsole, WHITE);

	setCursorLoc(9 + w * 2, 10);
	cout << "\u2554";
	for (int i = 0; i < 10; i++) {
		cout << "\u2550";
	}
	cout << "\u2557";

	for (int i = 0; i < 4; i++) {
		setCursorLoc(9 + w * 2, 11 + i);
		cout << "\u2551";
		setCursorLoc(20 + w * 2, 11 + i);
		cout << "\u2551";
	}

	setCursorLoc(9 + w * 2, 15);
	cout << "\u255A";
	for (int i = 0; i < 10; i++) {
		cout << "\u2550";
	}
	cout << "\u255D";

	SetConsoleTextAttribute(hConsole, WHITE);
	setCursorLoc(11 + w * 2, 11);
	cout << "Next: ";

	setCursorLoc(11 + w * 2, 13);
	for (int i = 0; i < 2; i++) {
		setCursorLoc(11 + w * 2, 13 + i);
		for (int j = 0; j < 4; j++) {
			if (newShape[i][j] == 1) {
				SetConsoleTextAttribute(hConsole, getColor(newShapeIndex));
				cout << "\u2588\u2588";
			}
			else {
				cout << "  ";
			}
		}
	}
}

void printLevel() {
	SetConsoleTextAttribute(hConsole, WHITE);

	setCursorLoc(9 + w * 2, 17);
	cout << "\u2554";
	for (int i = 0; i < 10; i++) {
		cout << "\u2550";
	}
	cout << "\u2557";

	for (int i = 0; i < 4; i++) {
		setCursorLoc(9 + w * 2, 18 + i);
		cout << "\u2551";
		setCursorLoc(20 + w * 2, 18 + i);
		cout << "\u2551";
	}

	setCursorLoc(9 + w * 2, 22);
	cout << "\u255A";
	for (int i = 0; i < 10; i++) {
		cout << "\u2550";
	}
	cout << "\u255D";

	setCursorLoc(11 + w * 2, 18);
	cout << "Level: ";
	setCursorLoc(11 + w * 2, 19);
	cout << level;
	setCursorLoc(11 + w * 2, 20);
	cout << "Rows: ";
	setCursorLoc(11 + w * 2, 21);
	cout << rowsToShow;

}

void fadeRow() {
	int in_row = 0; // measure how many rows are complete
	for (int i = h - 1; i >= 0; i--) {
		bool is_full = true; // checks if row is full or not
		for (int j = 0; j < w; j++) {
			if (board[i][j] != 2) {
				is_full = false;
				break;
			}
		}

		if (is_full) {
			setCursorLoc(6, 4 + i);
			SetConsoleTextAttribute(hConsole, WHITE);
			for (int j = 0; j < w; j++) {
				board[i][j] = 0;
				cout << "\u2588\u2588";
				Sleep(5);
			}
			PlaySound(TEXT("LineBurn.wav"), NULL, SND_FILENAME | SND_ASYNC);
			in_row++;
		}
		// fade previous rows as soon as reaches an empty one
		else {
			if (in_row > 0) {
				for (int k = i; k >= 0; k--) {
					for (int j = 0; j < w; j++) {
						if (board[k][j] == 2) {
							swap(board[k][j], board[k + in_row][j]);
							swap(colors[k][j], colors[k + in_row][j]);
						}
					}
				}
				
				rows += in_row; // rows mod 10
				rowsToShow += in_row; // total rows completed
				points += in_row * 100 * level; // player gets 100 * level points if complete a row
				in_row = 0;
			}
		}
	}

	// upgrade the level
	if (rows >= 10 && level < 10) {
		rows -= 10;
		PlaySound(TEXT("LevelUp.wav"), NULL, SND_FILENAME | SND_ASYNC);
		level++;
	}

	printLevel();
}

void makeShadow() {
	int min_dist = h;
	// measure how distance to go down to make the shadow of shape
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (board[i][j] == 1) {
				for (int k = i + 1; k <= h; k++) {
					if (board[k][j] == 2) {
						if (k - i < min_dist) {
							min_dist = k - i;
							break;
						}
					}
				}
			}
		}
	}
	// put 3 for shadow on main board
	if (min_dist > 4) {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				if (board[i][j] == 1) {
					board[i + min_dist - 1][j] = 3;
				}
			}
		}
	}
}

void saveRecord() {
	// make an array of records to put all records
	int recNum = recordCounter();
	Records* recs = new Records[recNum];

	ifstream scores("leader.txt", ios::in);
	int i = 0;
	while (scores >> recs[i].user >> recs[i].point >> recs[i].time >> recs[i].m >> recs[i].n >> recs[i].level)
	{
		i++;
	}
	scores.close();

	bool isUniqueName = true; // if the name is new, create a new profile

	for (int i = 0; i < recNum; i++) {
		if (recs[i].user == name) {
			if (recs[i].point < points) {
				recs[i].user = name;
				recs[i].point = points;
				recs[i].time = now;
				recs[i].n = w;
				recs[i].m = h;
				recs[i].level = level;
				isUniqueName = false;
				break;
			}
			else if (recs[i].point == points) {
				if (recs[i].time > now) {
					recs[i].user = name;
					recs[i].point = points;
					recs[i].time = now;
					recs[i].n = w;
					recs[i].m = h;
					recs[i].level = level;
					isUniqueName = false;
					break;
				}
			}
		}
	}

	// save the new record in the file
	fstream saveRec("leader.txt", ios::out);
	for (int i = 0; i < recNum; i++) {
		if (i != 0)
			saveRec << endl;
		saveRec << recs[i].user << " " << recs[i].point << " " << recs[i].time << " " << recs[i].n << " " << recs[i].m << " " << recs[i].level;
	}
	if (isUniqueName) {
		saveRec << endl << name << " " << points << " " << now << " " << w << " " << h << " " << level;
	}
	saveRec.close();
}

void printGameOver() {
	system("cls");
	SetConsoleTextAttribute(hConsole, RED);
	PlaySound(TEXT("GameOver.wav"), NULL, SND_FILENAME | SND_ASYNC);
	setCursorLoc((Width - 106) / 2, 6);
	cout << "      ::::::::      :::       :::   :::   ::::::::::          ::::::::  :::     ::: :::::::::: :::::::::";
	setCursorLoc((Width - 106) / 2, 7);
	cout << "    :+:    :+:   :+: :+:    :+:+: :+:+:  :+:                :+:    :+: :+:     :+: :+:        :+:    :+: ";
	setCursorLoc((Width - 106) / 2, 8);
	cout << "   +:+         +:+   +:+  +:+ +:+:+ +:+ +:+                +:+    +:+ +:+     +:+ +:+        +:+    +:+  ";
	setCursorLoc((Width - 106) / 2, 9);
	cout << "  :#:        +#++:++#++: +#+  +:+  +#+ +#++:++#           +#+    +:+ +#+     +:+ +#++:++#   +#++:++#:    ";
	setCursorLoc((Width - 106) / 2, 10);
	cout << " +#+   +#+# +#+     +#+ +#+       +#+ +#+                +#+    +#+  +#+   +#+  +#+        +#+    +#+    ";
	setCursorLoc((Width - 106) / 2, 11);
	cout << "#+#    #+# #+#     #+# #+#       #+# #+#                #+#    #+#   #+#+#+#   #+#        #+#    #+#     ";
	setCursorLoc((Width - 106) / 2, 12);
	cout << "########  ###     ### ###       ### ##########          ########      ###     ########## ###    ###      ";
	Sleep(2500);
	system("cls");
	setCursorLoc(0,0);
	SetConsoleTextAttribute(hConsole, WHITE);
}

void restartGame() {
	points = 0;
	rows = 0;
	rowsToShow = 0;
	lastGameTime = 0;

	// main board
	board = new int* [h + 1];
	for (int i = 0; i <= h; i++) {
		board[i] = new int[w];
		for (int j = 0; j < w; j++) {
			board[i][j] = 0;
		}
	}
	// fill the last row with solid blocks
	for (int j = 0; j < w; j++) {
		board[h][j] = 2;
	}

	// colors board
	colors = new int* [h];
	for (int i = 0; i < h; i++) {
		colors[i] = new int[w];
		for (int j = 0; j < w; j++) {
			colors[i][j] = 7;
		}
	}

	// shape
	shape = new int* [4];
	for (int i = 0; i < 4; i++) {
		shape[i] = new int[4];
		for (int j = 0; j < 4; j++)
			shape[i][j] = 0;
	}

	// next shape
	newShape = new int* [4];
	for (int i = 0; i < 4; i++) {
		newShape[i] = new int[4];
		for (int j = 0; j < 4; j++)
			newShape[i][j] = 0;
	}

	shapeRand(shape, CM, shapeIndex);
	shapeRand(newShape, newCM, newShapeIndex);

	insertShape();

	printPoints();
	printNextShape();
	printLevel();

	int start = time(NULL);
}
