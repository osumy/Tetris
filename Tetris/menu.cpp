#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <cmath>
#include <stdlib.h>
#include <fstream>
#include "game.h"
#include "leaderboard.h"

using namespace std;

HANDLE consoleHandle;
int Width, Height;
int buttonI = 0;
bool continueGame = false;

void printHTP(); // How To Play
void prepareConsoleHandle(); // get width & height of page
void setCursorLoc(SHORT x, SHORT y); // move cursor
void printFL_Line(bool isTop); // print the top and bottom border line (the first and last line)
void printEmptyLine(); // print a new empty line (just for border)
void printGameLogo(); // print a big Tetris
void printButton(string s); // print a game button
void changeChoice(int& i, bool isUp);
void clearHoverMarker(int choice); // clear some marker around bottuns
void printHoverMarker(int choice); // print some marker around bottuns
void pressEnter(int choice);
void hideCursor(bool isHide);

void printBorder() {
    // print border around the screen
    printFL_Line(true);
    for (int i = 0; i < Height - 2; i++)
        printEmptyLine();
    printFL_Line(false);
}

void printMainMenu() {
    system("cls");

    printBorder();

    printGameLogo();

    string buttons[5] = { "New Game", "Continue", "LeaderBoard", "How to Play", "Exit" };

    for (int i = 0; i < 5; i++) {
        printButton(buttons[i]);
    }
}

void mainMenu() {
    prepareConsoleHandle();
    
    printMainMenu();

    char move;
    int choice = 0;
    while (true) {
        
        move = _getch();
        while (move != 119 && move != 115 && move != 72 && move != 80 && move != 27 && move != 13 && move != 32) {
            move = _getch();
        }

        clearHoverMarker(choice);
        switch (move)
        {
        case 72:
            changeChoice(choice, true);
            break;
        case 119:
            changeChoice(choice, true);
            break;
        case 80:
            changeChoice(choice, false);
            break;
        case 115:
            changeChoice(choice, false);
            break;
        case 27:
            setCursorLoc(Width, Height + 2);
            exit(0);
            break;
        case 13:
            pressEnter(choice);
            break;
        case 32:
            pressEnter(choice);
            break;
        }
        printHoverMarker(choice);
    }
}

void prepareConsoleHandle() {
    // get the handle in order to get width and height of screen
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;

    GetConsoleScreenBufferInfo(consoleHandle, &bufferInfo);

    Width = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
    Height = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;

    hideCursor(false);
}

void setCursorLoc(SHORT x, SHORT y) {
    COORD loc;
    loc.X = x;
    loc.Y = y;
    SetConsoleCursorPosition(consoleHandle, loc);
}

void changeChoice(int& i, bool isUp) {
    if (isUp) {
        if (i == 0)
            i = 12;
        else
            i -= 3;
    }

    else {
        if (i == 12)
            i = 0;
        else
            i += 3;
    }
}

void clearHoverMarker(int choice) {
    setCursorLoc((SHORT)(((Width - 18) / 2) - 2), (SHORT)(choice + 10));
    cout << "  ";
    setCursorLoc((SHORT)(((Width - 18) / 2) + 18), (SHORT)(choice + 10));
    cout << "  ";
}

void printHoverMarker(int choice) {
    setCursorLoc((SHORT)(((Width - 18) / 2) - 2), (SHORT)(choice + 10));
    cout << ">>";
    setCursorLoc((SHORT)(((Width - 18) / 2) + 18), (SHORT)(choice + 10));
    cout << "<<";
}

void hideCursor(bool isHide) {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = isHide; // set the cursor visibility
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

// print the top and bottom border line (the first and last line)
void printFL_Line(bool isTop) {
    if (isTop) {
        for (int i = 0; i < Width / 2; i++)
            cout << "\u259B\u259C";
        cout << endl;
    }
    else {
        for (int i = 0; i < Width / 2; i++)
            cout << "\u2599\u259F";
    }
}

// print a new empty line (just for border)
void printEmptyLine() {
    cout << "\u259B";
    for (int i = 0; i < Width - 2; i++)
        cout << " ";
    cout << "\u259C\n";
}

// print a game button
void printButton(string s) {
    int len = s.length();

    // top
    setCursorLoc((SHORT)((Width - 18) / 2), (SHORT)(buttonI + 9));
    buttonI++;
    cout << "\u256D";
    for (int i = 0; i < 16; i++)
        cout << "\u2500";
    cout << "\u256E";

    // button name
    setCursorLoc((SHORT)((Width - 18) / 2), (SHORT)(buttonI + 9));
    buttonI++;
    cout << "\u2502";
    for (int i = 0; i < floor((16 - len) / 2); i++)
        cout << " ";
    cout << s;
    for (int i = 0; i < ceil(static_cast<double>(16 - len) / 2.0); i++)
        cout << " ";
    cout << "\u2502";

    // bottom
    setCursorLoc((SHORT)((Width - 18) / 2), (SHORT)(buttonI + 9));
    buttonI++;
    cout << "\u2570";
    for (int i = 0; i < 16; i++)
        cout << "\u2500";
    cout << "\u256F";
}

// print a big Tetris
void printGameLogo() {
    string logo[5] = { "  ______     __       _     ",
                       " /_  __/__  / /______(_)____",
                       "  / / / _ \\/ __/ ___/ / ___/",
                       " / / /  __/ /_/ /  / (__  ) ",
                       "/_/  \\___/\\__/_/  /_/____/  " };
    for (int i = 0; i < 5; i++) {
        setCursorLoc((SHORT)((Width - 28) / 2), (SHORT)(i + 2));
        cout << logo[i];
    }                 
}
void sleepyPrint(string str, int time) {
    for (int i = 0; i < str.length(); i++) {
        Sleep(time); // pause printing for time "time"
        cout << str[i];
    }
}

bool is_pressed = false;
void sleepyPrintHTP(string str, int time) {
    if (!is_pressed) {
        for (int i = 0; i < str.length(); i++) {
            Sleep(time); // pause printing for time "time"
            cout << str[i];
            if (_kbhit()) {
                is_pressed = true;
                break;
            }
        }
    }
}

void pressEnter(int choice) {
    ifstream saveFile("save.txt", ios::in);
    switch (choice)
    {
    case 0:
        game();
        break;
    case 3:
        if (saveFile)
            continueGame = true;
        game();
        break;
    case 6:
        printLB();
        break;
    case 9:
        printHTP();
        break;
    case 12:
        setCursorLoc(Width, Height + 2);
        exit(0);
        break;
        break;
    }
    saveFile.close();
    setCursorLoc(0, 0);
    buttonI = 0;
    printMainMenu();
}

// start to print how to play using sleepy print
void skipHTP() {
    sleepyPrintHTP("How to Play!", 60);
    setCursorLoc(5, 3);
    sleepyPrintHTP("It's Tetris! Here you've gotta survive as much as you can. But how?", 20);
    setCursorLoc(5, 4);
    sleepyPrintHTP("Random Blocks are falling down and all you have to do with them is to match them in a row completely.", 20);
    setCursorLoc(5, 6);
    sleepyPrintHTP("First, Press new game and choose the difficulty. enter username and start your game.", 20);
    setCursorLoc(5, 7);
    sleepyPrintHTP("You can also pause and save game.", 20);
    setCursorLoc(5, 9);
    sleepyPrintHTP("Let's talk about controllers:", 20);
    setCursorLoc(5, 10);
    sleepyPrintHTP("1.A to lean left and D to lean right", 20);
    setCursorLoc(5, 11);
    sleepyPrintHTP("2.Q to turn left and E to turn right", 20);
    setCursorLoc(5, 12);
    sleepyPrintHTP("3.S to speed up", 20);
    setCursorLoc(5, 13);
    sleepyPrintHTP("4.Space to drop", 20);
    setCursorLoc(5, 14);
    sleepyPrintHTP("5.Esc to open pause menu", 20);
    setCursorLoc(5, 16);
    sleepyPrintHTP("And you can check settings to change almost everything :)", 20);
    _getch();
}

// print all of it at once if a key is pressed
void printHTP() {
    system("cls");

    printBorder();

    setCursorLoc(5, 2);
    skipHTP();

    if (is_pressed) {
        setCursorLoc(5, 2);
        cout << "How to Play!";
        setCursorLoc(5, 3);
        cout << "It's Tetris! Here you've gotta survive as much as you can. But how?";
        setCursorLoc(5, 4);
        cout << "Random Blocks are falling down and all you have to do with them is to match them in a row completely.";
        setCursorLoc(5, 6);
        cout << "First, Press new game and choose the difficulty. enter username and start your game.";
        setCursorLoc(5, 7);
        cout << "You can also pause and save game.";
        setCursorLoc(5, 9);
        cout << "Let's talk about controllers:";
        setCursorLoc(5, 10);
        cout << "1.A to lean left and D to lean right";
        setCursorLoc(5, 11);
        cout << "2.Q to turn left and E to turn right";
        setCursorLoc(5, 12);
        cout << "3.S to speed up";
        setCursorLoc(5, 13);
        cout << "4.Space to drop";
        setCursorLoc(5, 14);
        cout << "5.Esc to open pause menu";
        setCursorLoc(5, 16);
        cout << "And you can check settings to change almost everything :)";

        is_pressed = false;
        _getch();
    }
}
