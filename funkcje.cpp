#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <fstream>
#include "funkcje.h"

using namespace std;
#define wysokosc 20
#define szerokosc 20

COORD snakeCoord;
COORD fruitCoord;
COORD bonusCoord;

bool gameOver = false;
bool bonus = false;
char kierunek;
int wynik = 0;
int maksWynik = 0;
int ogonX[100], ogonY[100];
int nTail = 0;

void plansza() {

	system("cls");
	cout << "  P - koniec gry" << endl << "  C - zmiana koloru" << endl;
	cout << "  ";
	for (int i = 0; i < (szerokosc + 2); i++) {
		cout << "*";

	}
	cout << endl;

	for (int i = 0; i < wysokosc; i++) {
		cout << "  ";
		for (int j = 0; j < szerokosc; j++) {
			if (j == 0) {
				cout << "*";
			}

			if (i == snakeCoord.Y && j == snakeCoord.X) {
				cout << "O";
			}
			else if (i == fruitCoord.Y && j == fruitCoord.X) {
				cout << "F";
			}
			else if (i == bonusCoord.Y && j == bonusCoord.X && bonus == true) {
				cout << "B";
			}
			else
			{
				bool wyswietlOgon = false;
				for (int k = 0; k < nTail; k++)
				{
					if (ogonX[k] == j && ogonY[k] == i)
					{
						cout << "o";
						wyswietlOgon = true;
					}
				}
				if (!wyswietlOgon)
					cout << " ";
			}

			if (j == szerokosc - 1) {
				cout << "*";
			}
		}
		cout << endl;
	}

	cout << "  ";
	for (int i = 0; i < (wysokosc + 2); i++) {
		cout << "*";
	}
	cout << endl << endl;
	cout << "  Wynik: " << wynik << endl;
	cout << "  Najwyzszy wynik: " << maksWynik << endl;

}

//spawn owoca w losowym miejscu
void putFruit() {
	fruitCoord.X = rand() % szerokosc;
	fruitCoord.Y = rand() % wysokosc;
}

//spawn bonusu
void putBonus() {
	int czas = GetTickCount64();
	if (czas % 5000 == 0) {
		bonusCoord.X = rand() % szerokosc;
		bonusCoord.Y = rand() % wysokosc;
		bonus = true;
	}

}

void zebranieOwocu() {
	if (snakeCoord.X == fruitCoord.X && snakeCoord.Y == fruitCoord.Y) {
		wynik += 10;
		nTail++;
		putFruit();
	}
}

void zebranieBonusu() {
	if (snakeCoord.X == bonusCoord.X && snakeCoord.Y == bonusCoord.Y) {
		bonus = false;
		wynik += 30;
	}
}

//zmiana koloru terminalu
void wylosujKolor() {

	int kolor = rand() % 20 + 1;
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, kolor);
}

//ustawienie glowy weza na srodku i owoca w losowym miejscu
void poczatek() {

	snakeCoord.X = wysokosc / 2;
	snakeCoord.Y = szerokosc / 2;
	putFruit();
}


void obslugaKlawiatury() {

	if (_kbhit()) {
		switch (_getch())
		{
		case 'a':
			kierunek = 'l';
			break;
		case 'd':
			kierunek = 'p';
			break;
		case 'w':
			kierunek = 'g';
			break;
		case 's':
			kierunek = 'd';
			break;
		case 'p':
			gameOver = true;
			break;
		case 'c':
			wylosujKolor();
			break;
		}
	}


}

void ruch() {
	switch (kierunek)
	{
	case 'l':
		snakeCoord.X--;
		break;
	case 'p':
		snakeCoord.X++;
		break;
	case 'g':
		snakeCoord.Y--;
		break;
	case 'd':
		snakeCoord.Y++;
		break;
	}

	//przechodzenie przez sciany
	if (snakeCoord.X >= szerokosc)
		snakeCoord.X = 0;
	else if (snakeCoord.X < 0)
		snakeCoord.X = szerokosc - 1;

	if (snakeCoord.Y >= wysokosc)
		snakeCoord.Y = 0;
	else if (snakeCoord.Y < 0)
		snakeCoord.Y = wysokosc - 1;

}

void kolizja() {
	for (int i = 0; i < nTail; i++)
		if (ogonX[i] == snakeCoord.X && ogonY[i] == snakeCoord.Y)
			gameOver = true;
}

void ogon() {
	int ogonpom2X, ogonpom2Y;
	int ogonpomX = ogonX[0];
	int ogonpomY = ogonY[0];

	ogonX[0] = snakeCoord.X;
	ogonY[0] = snakeCoord.Y;
	for (int i = 1; i < nTail; i++)
	{
		ogonpom2X = ogonX[i];
		ogonpom2Y = ogonY[i];
		ogonX[i] = ogonpomX;
		ogonY[i] = ogonpomY;
		ogonpomX = ogonpom2X;
		ogonpomY = ogonpom2Y;
	}


}

void czyRekord() {
	if (wynik > maksWynik) {
		maksWynik = wynik;
	}
}

void spowolnienie() {
	if (kierunek == 'l' || kierunek == 'p')
	{
		Sleep(5);
	}

	Sleep(100);

	if (kierunek == 'g' || kierunek == 'd')
	{
		Sleep(40);
	}
}

void znajdzMaxWynik() {
	int liczba;

	fstream historiaWynikow;
	historiaWynikow.open("wynik.txt", ios::in);

	while (!historiaWynikow.eof()) {
		historiaWynikow >> liczba;
		if (liczba > maksWynik) {
			maksWynik = liczba;
		}
	}
	historiaWynikow.close();

}

void zapiszWynik() {
	if (maksWynik >= wynik) {
		fstream historiaWynikow;
		historiaWynikow.open("wynik.txt", ios::app);
		historiaWynikow << wynik << endl;
		historiaWynikow.close();
	}
}

void start() {
	
	srand(time(NULL));
	poczatek();
	znajdzMaxWynik();

	while (!gameOver) {
		plansza();
		ogon();
		obslugaKlawiatury();
		ruch();
		kolizja();
		zebranieOwocu();
		putBonus();
		zebranieBonusu();
		spowolnienie();
		czyRekord();
	}

	zapiszWynik();
}