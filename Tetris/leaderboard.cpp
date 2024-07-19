#include <iostream>
#include <fstream>
#include <conio.h>
#include <String>
#include "menu.h"

using namespace std;

// a structure for leader board records
struct Record {
	string user;
	int point;
	int time;
	int m; // rows
	int n; // columns
	int level;
};

// sort the records
void leaderBoardSort();

// returns the number of records
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

// prints the leader board
void printLB()
{
	leaderBoardSort();
	system("cls");
	printBorder();

	int lvl; // level
	setCursorLoc(3, 2);
	cout << ">> Enter Level(1-10): ";
	cin >> lvl;

	system("cls");
	printBorder();

	int recNum = 0;
	recNum += recordCounter();
	if (recNum == 0) {
		setCursorLoc(3, 2);
		cout << "There is no file...";
		Sleep(2000);
		system("cls");
		return;
	}
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

	int I = 0;
	for (int i = 0; i < recNum; i++) {
		if (recs[i].level == lvl) {
			setCursorLoc(3, 5 + I);
			cout << recs[i].user;

			setCursorLoc(43, 5 + I);
			cout << recs[i].point;

			setCursorLoc(65, 5 + I);
			cout << recs[i].time;

			setCursorLoc(90, 5 + I);
			cout << recs[i].n << "x" << recs[i].m;

			setCursorLoc(109, 5 + I);
			cout << recs[i].level;
			I++;
		}
	}

	_getch();
}

// bubbleSort algorithm for sorting the records
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

// sort the records
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
