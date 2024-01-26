#include <iostream>
#include <windows.h>
#include "menu.h"

#pragma comment(lib, "winmm.lib")

using namespace std;

int main()
{
	PlaySound(TEXT("main.wav"), NULL, SND_FILENAME | SND_ASYNC);
	mainMenu();
	return 0;
}
