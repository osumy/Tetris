#include <iostream>
#include <conio.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <Windows.h>
#include "menu.h"
#include "gameMenu.h"

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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
	string username;
	int width;
	int height;
	int level;

};

struct Location
{
	int i; // row
	int j; // column
};

/////////////////////////////////
// Functions
/////////////////////////////////

GameSettings getGameSettings();
void getConsoleSize(int& Width, int& Height);
void pauseMenu();
void loading();
void game();
void load();
void makeSolid();
void makeSolidFor(bool& canShiftD);
void fall();
void shiftR();
void shiftL();
void shiftD();
void turnR();
void turnL();
void insertShape();
void shapeRand(int** shape, Location& cm, int& index);
int getColor(int index);
void gameSave();
void printMainBoarder();
void printPoints();
void printNextShape();
void printLevel();
void fadeRow();
void makeShadow();
bool gameOver();
void saveRecord();
void printGameOver();


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
bool gameOverBool = false;
int points = 0;
int rows = 0, rowsToShow;
int now = 0;

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
		srand(static_cast<unsigned int>(time(nullptr)));
		c++;
	}
	int counter = 1;
	remainInGame = true;

	if (continueGame) {
		load();
		loading();
	}
	
	else {
		GameSettings gs = getGameSettings();

		h = gs.height;
		w = gs.width;
		name = gs.username;
		level = gs.level;

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

		shapeRand(shape, CM, shapeIndex);
		shapeRand(newShape, newCM, newShapeIndex);
		insertShape();
	}

	printPoints();
	printNextShape();
	printLevel();

	int start = time(NULL);

	while (remainInGame) {
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
				else {
					cout << "  ";
				}
			}
		}

		makeShadow();
		now = time(NULL) - start;

		SetConsoleTextAttribute(hConsole, WHITE);
		setCursorLoc(11 + w * 2, 4);
		cout << "Time: ";
		setCursorLoc(11 + w * 2, 5);
		cout << now;
		setCursorLoc(11 + w * 2, 6);
		cout << "Points: ";
		setCursorLoc(11 + w * 2, 7);
		cout << points;

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
				pauseMenu();
			}
		}

		bool canShiftD = true;
		if (counter % 10 == 0) {
			makeSolidFor(canShiftD);
			if (canShiftD)
				shiftD();
			counter = 0;
		}

		counter++;
		Sleep(60 - 4 * level);
	}
	
	system("cls");
	setCursorLoc(0, 0);
	if (!gameOverBool) {
		gameSave();
		if (exitGame)
			exit(1);
	}
	else {
		printGameOver();
	}
}

void makeSolidFor(bool& canShiftD) {
	for (int i = h; i >= 0; i--) {
		for (int j = 0; j < w; j++) {
			if (board[i][j] == 2 && board[i - 1][j] == 1) {
				makeSolid();
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++)
						shape[i][j] = newShape[i][j];
				}
				CM.i = newCM.i;
				CM.j = newCM.j;
				shapeIndex = newShapeIndex;
				if (gameOver()) {
					saveRecord();
					remainInGame = false;
					gameOverBool = true;
					return;
				}
				insertShape();
				shapeRand(newShape, newCM, newShapeIndex);
				canShiftD = false;
				rotateIndex = 0;
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
	
	SetConsoleTextAttribute(hConsole, getColor(shapeIndex));
	for (int i = h - 1; i >= 0; i--) {
		for (int j = 0; j < w; j++) {
			if (board[i][j] == 1) {
				setCursorLoc(6 + j * 2, 4 + i);
				cout << "\u2588\u2588";
			}
		}
	}
	PlaySound(TEXT("Fall.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void shiftR() {
	bool shiftable = true;

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
	for (int i = h; i >= 0; i--) {
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
	if (shapeIndex == 1)
		return;

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
			if (CM.i - 1 < 0 || CM.i + 2 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 2][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 2][CM.j] = 1;
			rotateIndex++;
			break;
		case 1:
			if (CM.j - 2 < 0 || CM.j + 1 > w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i][CM.j - 2] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i][CM.j - 2] = 1;
			rotateIndex++;
			break;
		case 2:
			if (CM.i - 2 < 0 || CM.i + 1 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 2][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 2][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			rotateIndex++;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.j + 2 > w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j + 2] == 2 || temp[CM.i][CM.j - 1] == 2)
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
			if (CM.i - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i - 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			rotateIndex++;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex++;
			break;
		case 2:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 > w || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 > w || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j - 1] == 2 || temp[CM.i][CM.j + 1] == 2)
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
			if (CM.i - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex++;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i][CM.j + 1] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 2:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.i + 1 > h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 > w || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j + 1] == 2)
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
			if (CM.i - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j + 1] == 2 || temp[CM.i + 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			rotateIndex++;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i][CM.j - 1] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex++;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.i + 1 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 3:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.j + 1 > w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j - 1] == 2)
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
			if (CM.i - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex++;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.i + 1 > h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 3:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.j + 1 > w || temp[CM.i][CM.j - 1] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i - 1][CM.j + 1] == 2)
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
			if (CM.i - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			rotateIndex++;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			rotateIndex++;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.i + 1 > h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			rotateIndex++;
			break;
		case 3:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.j + 1 > w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j] == 2)
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

	for (int i = 0; i < h; i++)
		delete[] temp[i];
	delete[] temp;
}

void turnL() {
	if (shapeIndex == 1)
		return;

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
			if (CM.i - 2 < 0 || CM.i + 1 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 2][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 2][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			rotateIndex = 3;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.j + 2 > w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j + 2] == 2 || temp[CM.i][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j + 2] = 1;
			temp[CM.i][CM.j - 1] = 1;
			rotateIndex--;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.i + 2 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 2][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 2][CM.j] = 1;
			rotateIndex--;
			break;
		case 3:
			if (CM.j - 2 < 0 || CM.j + 1 > w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i][CM.j - 2] == 2)
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
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.i + 1 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex = 3;
			break;
		case 1:
			if (CM.i - 1 < 0 || CM.j - 1 < 0 || CM.j + 1 > w || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j - 1] == 2 || temp[CM.i][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			temp[CM.i][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j + 1 > w || temp[CM.i + 1][CM.j] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i - 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
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
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.i + 1 > h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			rotateIndex = 3;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 > w || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i][CM.j + 1] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
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
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.i + 1 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j - 1] = 1;
			rotateIndex = 3;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 > w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			rotateIndex--;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j + 1] == 2 || temp[CM.i + 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			temp[CM.i + 1][CM.j] = 1;
			rotateIndex--;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i][CM.j - 1] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
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
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.i + 1 > h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i - 1][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i - 1][CM.j - 1] = 1;
			rotateIndex = 3;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 > w || temp[CM.i][CM.j - 1] == 2 || temp[CM.i][CM.j + 1] == 2 || temp[CM.i - 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i - 1][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i - 1][CM.j] == 2 || temp[CM.i + 1][CM.j] == 2 || temp[CM.i + 1][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i + 1][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j - 1] == 2)
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
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.i + 1 > h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j - 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j - 1] = 1;
			rotateIndex = 3;
			break;
		case 1:
			if (CM.j - 1 < 0 || CM.i - 1 < 0 || CM.j + 1 > w || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i - 1][CM.j] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			temp[CM.i][CM.j - 1] = 1;
			temp[CM.i - 1][CM.j] = 1;
			rotateIndex--;
			break;
		case 2:
			if (CM.i - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i + 1][CM.j] == 2 || temp[CM.i - 1][CM.j] == 2 || temp[CM.i][CM.j + 1] == 2)
				return;
			temp[CM.i][CM.j] = 1;
			temp[CM.i + 1][CM.j] = 1;
			temp[CM.i - 1][CM.j] = 1;
			temp[CM.i][CM.j + 1] = 1;
			rotateIndex--;
			break;
		case 3:
			if (CM.j - 1 < 0 || CM.j + 1 > w || CM.i + 1 > h || temp[CM.i][CM.j + 1] == 2 || temp[CM.i][CM.j - 1] == 2 || temp[CM.i + 1][CM.j] == 2)
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

	for (int i = 0; i < h; i++)
		delete[] temp[i];
	delete[] temp;
}

void makeSolid() {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (board[i][j] == 1) {
				board[i][j] = 2;
				colors[i][j] = shapeIndex;
			}
		}
	}

	fadeRow();
}

void insertShape() {
	int J = 0;
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

void getConsoleSize(int& Width, int& Height) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;

	GetConsoleScreenBufferInfo(consoleHandle, &bufferInfo);

	Width = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
	Height = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;
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
	save << name << endl << w << endl << h << endl << level << endl;
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

	x = (w - 4) / 2;
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
	int in_row = 0;
	for (int i = h - 1; i >= 0; i--) {
		bool is_full = true;
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
			
			in_row++;
		}
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
				
				rows += in_row;
				rowsToShow += in_row;
				points += in_row * 100 * level;
				in_row = 0;
			}
		}
	}

	if (rows >= 10 && level < 10) {
		rows -= 10;
		level++;
	}

	printLevel();
}

void makeShadow() {
	int min_dist = h;

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
	
	SetConsoleTextAttribute(hConsole, GRAY);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (board[i][j] == 1) {
				setCursorLoc(6 + j * 2, 3 + i + min_dist);
				cout << "\u2588\u2588";
			}
		}
	}

	SetConsoleTextAttribute(hConsole, WHITE);
}

bool gameOver() {
	int J = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = x; j < x + 4; j++) {
			if (shape[i][J] == 1 && board[i][j] == 2)
				return true;
			J++;
		}
		J = 0;
	}
	return false;
}

void saveRecord() {
	fstream saveRec("leader.txt", ios::app);
	saveRec << endl << name << " " << points << " " << now << " " << w << " " << h << " " << level;
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
