#include <iostream>
#include <Windows.h>
#include "menu.h"

#pragma comment(lib, "winmm.lib")

using namespace std;

int main()
{
	PlaySound(TEXT("main.wav"), NULL, SND_ASYNC | SND_FILENAME);
	mainMenu();
	return 0;
}
