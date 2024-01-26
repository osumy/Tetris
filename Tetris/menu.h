#pragma once
#include <Windows.h>

void mainMenu();
void prepareConsoleHandle();
void setCursorLoc(SHORT x, SHORT y);
void printBorder();
void printGameLogo();
extern int Width, Height;
extern bool continueGame;