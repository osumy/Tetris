#include <iostream>
#include <conio.h>
#include "menu.h"
#include "game.h"

using namespace std;

int buttonIndex = 0;
void printGameMenuButton(string s);
void clearHoverMarker_gameMenu(int choice);
void printHoverMarker_gameMenu(int choice);
void changeChoice_gameMenu(int& i, bool isUp);
void pressEnter_gameMenu(int choice, bool& remainInWhile);

void pauseMenu() {
    system("cls");
    printBorder();
    printGameMenuButton("Continue");
    printGameMenuButton("Restart");
    printGameMenuButton("Save & Exit to Main Menu");
    printGameMenuButton("Save & Exit the Game");

    char move;
    int choice = 0;
    bool remainInWhile = true;
    while (remainInWhile) {
        move = _getch();
        while (move != 119 && move != 115 && move != 72 && move != 80 && move != 27 && move != 13 && move != 32) {
            move = _getch();
        }

        clearHoverMarker_gameMenu(choice);
        if (move == 72)
            changeChoice_gameMenu(choice, true);
        else if (move == 119)
            changeChoice_gameMenu(choice, true);
        else if (move == 80)
            changeChoice_gameMenu(choice, false);
        else if (move == 115)
            changeChoice_gameMenu(choice, false);
        else if (move == 27) {
            PlaySound(TEXT("click2.wav"), NULL, SND_FILENAME | SND_ASYNC);
            pressEnter_gameMenu(choice, remainInWhile);
            continue;
        }
        else if (move == 13) {
            PlaySound(TEXT("click2.wav"), NULL, SND_FILENAME | SND_ASYNC);
            pressEnter_gameMenu(choice, remainInWhile);
            continue;
        }
        else if (move == 32) {
            PlaySound(TEXT("click2.wav"), NULL, SND_FILENAME | SND_ASYNC);
            pressEnter_gameMenu(choice, remainInWhile);
            continue;
        }
        printHoverMarker_gameMenu(choice);
    }
}

void changeChoice_gameMenu(int& i, bool isUp) {
    if (isUp) {
        if (i == 0)
            i = 12;
        else
            i -= 4;
    }

    else {
        if (i == 12)
            i = 0;
        else
            i += 4;
    }
}

void printGameMenuButton(string s) {
    int len = s.length();
    prepareConsoleHandle();

    // top
    setCursorLoc((SHORT)((Width - 30) / 2), (SHORT)(buttonIndex + 7));
    buttonIndex++;
    cout << "\u256D";
    for (int i = 0; i < 28; i++)
        cout << "\u2500";
    cout << "\u256E";

    // button name
    setCursorLoc((SHORT)((Width - 30) / 2), (SHORT)(buttonIndex + 7));
    buttonIndex++;
    cout << "\u2502";
    for (int i = 0; i < floor((28 - len) / 2); i++)
        cout << " ";
    cout << s;
    for (int i = 0; i < ceil(static_cast<double>(28 - len) / 2.0); i++)
        cout << " ";
    cout << "\u2502";

    // bottom
    setCursorLoc((SHORT)((Width - 30) / 2), (SHORT)(buttonIndex + 7));
    buttonIndex++;
    cout << "\u2570";
    for (int i = 0; i < 28; i++)
        cout << "\u2500";
    cout << "\u256F";
    buttonIndex++;
}

void clearHoverMarker_gameMenu(int choice) {
    setCursorLoc((SHORT)(((Width - 30) / 2) - 2), (SHORT)(choice + 8));
    cout << "  ";
    setCursorLoc((SHORT)(((Width - 30) / 2) + 30), (SHORT)(choice + 8));
    cout << "  ";
}

void printHoverMarker_gameMenu(int choice) {
    setCursorLoc((SHORT)(((Width - 30) / 2) - 2), (SHORT)(choice + 8));
    cout << ">>";
    setCursorLoc((SHORT)(((Width - 30) / 2) + 30), (SHORT)(choice + 8));
    cout << "<<";
}
void pressEnter_gameMenu(int choice, bool& remainInWhile) {
    switch (choice)
    {
    case 0:
        remainInWhile = false;
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
        printNextShape();
        printPoints();
        printLevel();
        break;
    case 4:
        remainInWhile = false;
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
        restart = true;
        break;
    case 8:
        remainInWhile = false;
        remainInGame = false;
        printNextShape();
        printPoints();
        printLevel();
        break;
    case 12:
        remainInWhile = false;
        remainInGame = false;
        exitGame = true;
        break;
    }
    buttonIndex = 0;
    setCursorLoc(0, 0);
}