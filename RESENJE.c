#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include "types.h"
#include "Meni.h"
#define START1 37
#define START2 11
void reset(Game * igra) {
	int i = 0, j = 0;
	for (i = 0; i < 7; i++) igra->A[i] = i;
	igra->A[0] = 15;
//	igra->mode = 1;
	igra->em = 1;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			igra->p1[i][j].num = 0;
			igra->p2[i][j].num = 0;
			igra->p1[i][j].size = 0;
			igra->p2[i][j].size = 0;
			igra->g1[i][j] = 0;
			igra->g2[i][j] = 0;
		}
	}
}
void AbouGame() {
	system("CLS");
	gotoxy(30, 8);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	printf("The game of Battleship is thought to have its origins");
	gotoxy(30, 9);
	printf("in the French game L'Attaque played during World War1");
	gotoxy(30, 10);
	printf("although parallels have also been drawn to E. I. Horsman's");
	gotoxy(30, 11);
	printf("1890 game Basilinda and the game is said to have");
	gotoxy(30, 12);
	printf("been played by Russian officers before World War I");
	gotoxy(30, 13);
	printf("The first commercial version of the game was Salvo");
	gotoxy(30, 14);
	printf("published in 1931 in the United States by the Starex company");
	gotoxy(30, 15);
	printf("Other versions of the game were printed in the 1930s and 1940s");
	gotoxy(30, 16);
	printf("including the Strathmore Company's Combat: The Battleship Game");
	gotoxy(30, 17);
	printf("Milton Bradley's Broadsides: A Game of Naval Strategy");
	gotoxy(30, 18);
	printf("and Maurice L.Freedman's Warfare Naval Combat. Strategy Games Co. produced");
	gotoxy(30, 19);
	printf("a version called Wings which pictured planes flying over the Los ");
	gotoxy(30, 20);
	printf("Angeles Coliseum.All of these early editions of the game consisted");
	gotoxy(30, 21);
	printf("of pre-printed pads of paper");
	gotoxy(30, 25);
	printf("DEVOLOPED BY RICOX");
	char x = 0;
	while(x!=27)	
		x = _getch();
}
int main()
{
	int ok = 1, cnt1, cnt2, flag = 1,usee;							//na pocetku nije nista igrano
	Game igra;
	Game* use;
	reset(&igra);
	igra.mode = 1;
	int i = 0, j = 0;
	system("CLS");
	hidecursor();
	while (1) {
		system("CLS");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		gotoxy(40, 6);
		printf("%c", 24);
		gotoxy(40, 7);
		printf("W");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		char* scr[6] = { "NEW GAME","LOAD GAME","HALL OF FAME","OPTIONS","ABOUT GAME","EXIT"};					//Exit da se doda
		for (i = START2; i <= 21; i += 2) {
			gotoxy(START1, i);
			printf("%s", scr[(i-START2)/2]);
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		gotoxy(40, 25);
		printf("%c", 25);
		gotoxy(40, 26);
		printf("S");
		char c = '0';
		int sh = 0;
		while (c != 13) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			gotoxy(34, 11 + sh);
			printf("->");
			gotoxy(50, 11 + sh);
			printf("<-");
			gotoxy(0, 0);
			c = _getch();
			gotoxy(34, 11 + sh);               //brisi
			printf("  ");
			gotoxy(50, 11 + sh);
			printf("  ");
			if (c == 's') {
				if (sh < 10) {
					sh += 2;
				}
				else
					sh = 0;
			}
			if (c == 'w') {
				if (sh > 0) {
					sh -= 2;
				}
				else
					sh = 10;
			}
		}
		switch (sh) {                                   //klinkuo je enter
		case 0: reset(&igra);
				ControllGame(&igra, 1);
				use=&igra;
				break;
		case 2:use = loadGame();
				if(use==NULL){
					break;
				}
				ControllGame(use, 0);							//nastavi ovu igru
				igra = *use;
				break;
		case 4:HallofFame();
			break;
		case 6:options(&igra);
			break;
		case 8:AbouGame();
			break;
		case 10:return 0;
			break;
		}
	}
	return 0;
}