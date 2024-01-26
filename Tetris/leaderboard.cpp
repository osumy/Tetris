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

void leaderBoardSort();

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
	leaderBoardSort();
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
		cout << recs[i].n << "x" << recs[i].m;

		setCursorLoc(109, 5 + i);
		cout << recs[i].level;
	}

	setCursorLoc(3, 5 + recNum + 2);
	cout << "Press any key to continue...";
	_getch();
}

void bubbleSort(Record* arr, int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - i - 1; j++) {
			if (arr[j].point < arr[j + 1].point) {
				swap(arr[j], arr[j + 1]);
			}
			else if (arr[j].point == arr[j + 1].point) {
				if (arr[j].time > arr[j + 1].time)
					swap(arr[j], arr[j + 1]);
			}
		}
	}
}

void leaderBoardSort() {
	int recNum = recordCounter();
	Record* recs = new Record[recNum];

	ifstream scores("leader.txt", ios::in);
	int i = 0;
	while (scores >> recs[i].user >> recs[i].point >> recs[i].time >> recs[i].m >> recs[i].n >> recs[i].level)
	{
		i++;
	}
	bubbleSort(recs, recNum);
	scores.close();
	
	fstream saveRec("leader.txt", ios::out);
	for (int i = 0; i < recNum; i++) {
		if (i != 0)
			saveRec << endl;
		saveRec << recs[i].user << " " << recs[i].point << " " << recs[i].time << " " << recs[i].n << " " << recs[i].m << " " << recs[i].level;
	}
	saveRec.close();
}