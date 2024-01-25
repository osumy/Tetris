#include <iostream>
#include <fstream>
#include <conio.h>
#include <String>
#include "menu.h"

using namespace std;

struct Record {
	string user;
	int point;
	int time;
	int m; // rows
	int n; // columns
	int level;
};

void saveLB()
{

}

int recordCounter()
{
	ifstream scores("leader.txt", ios::in);
	string a;
	int b, c, d, e, f;
	int i = 0;
	while (scores >> a >> b >> c >> d >> e >> f)
	{
		i++;
	}
	scores.close();
	return i;
}

void printLB()
{
	system("cls");
	printBorder();

	int recNum = recordCounter();
	Record* recs = new Record[recNum];

	ifstream scores("leader.txt", ios::in);
	int i = 0;
	while (scores >> recs[i].user >> recs[i].point >> recs[i].time >> recs[i].m >> recs[i].n >> recs[i].level)
	{
		i++;
	}
	
	setCursorLoc(3, 2);
	cout << "Name";

	setCursorLoc(43, 2);
	cout << "Score";

	setCursorLoc(65, 2);
	cout << "Time";

	setCursorLoc(90, 2);
	cout << "m x n";

	setCursorLoc(109, 2);
	cout << "Level";

	setCursorLoc(3, 3);
	for (int i = 0; i < Width - 6; i++)
		cout << "\u2505";

	for (int i = 0; i < recNum; i++) {
		setCursorLoc(3, 5+i);
		cout << recs[i].user;

		setCursorLoc(43, 5 + i);
		cout << recs[i].point;

		setCursorLoc(65, 5 + i);
		cout << recs[i].time;

		setCursorLoc(90, 5 + i);
		cout << recs[i].m << "x" << recs[i].n;

		setCursorLoc(109, 5 + i);
		cout << recs[i].level;
	}

	setCursorLoc(3, 5 + recNum + 2);
	cout << "Press any key to continue...";
	_getch();
}