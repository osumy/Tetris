#include <iostream>
#include <fstream>
#include <conio.h>
#include <String>

using namespace std;

struct Record {
	string user;
	int point;
	int time;
};

void saveLB()
{

}

int recordCounter()
{
	ifstream scores("leader.txt", ios::in);
	string a;
	int b, c;
	int i = 0;
	while (scores >> a >> b >> c)
	{
		i++;
	}
	scores.close();
	return i;
}

void printLB()
{
	int recNum = recordCounter();
	Record* recs = new Record[recNum];

	ifstream scores("leader.txt", ios::in);
	int i = 0;
	while (scores >> recs[i].user >> recs[i].point >> recs[i].time)
	{
		i++;
	}
	scores.close();

	system("cls");
	for (int i = 0; i < 25; i++) {
		cout << "*";
	}
	cout << endl;

	cout << " Name";
	for (int i = 0; i < 35; i++) {
		cout << " ";
	}

	cout << "Score";
	for (int i = 0; i < 15; i++) {
		cout << " ";
	}

	cout << "Time";
	cout << endl;

	for (int i = 0; i < recNum; i++) {
		cout << " " << recs[i].user;
		int len = recs[i].user.length();
		for (int i = 0; i < 39 - len; i++) {
			cout << " ";
		}
		cout << recs[i].point;
		len = to_string(recs[i].point).length();
		for (int i = 0; i < 20 - len; i++) {
			cout << " ";
		}
		cout << recs[i].time;
		cout << endl;
	}
	cout << "\nPress any key to continue...";
	_getch();
}