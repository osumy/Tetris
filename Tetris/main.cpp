#include <iostream>
#include <windows.h>
#include "menu.h"

// just for sound player
#pragma comment(lib, "winmm.lib")

using namespace std;

int main()
{
	PlaySound(TEXT("soundtracks\\main.wav"), NULL, SND_FILENAME | SND_ASYNC);
	mainMenu();
	return 0;
}
