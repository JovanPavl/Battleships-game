#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include "Stek.h"
#include "types.h"
#include "Meni.h"
#define ASCII 254
#define BOAT 177
#define START1 37
#define START2 11
#define pogodak 'R'
#define promasaj 240
#define potopljen 248
void easyMode(short **guess,Ship **opp,Game igra,short * cnt) {
	system("CLS");
	hidecursor();
	int i, j, ok = 1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoxy(35, 7);
	printf("Computer's move");
	gotoxy(35, 9);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[0]);
	for (i = 0; i < 10; i++) {
		printf("%d ", i + 1);
	}
	for (i = 0; i < 10; i++) {
		gotoxy(33, 10 + i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[0]);
		printf("%c ", 'A' + i);
		gotoxy(35, 10 + i);
		for (j = 0; j < 10; j++) {
			if (guess[j][i] == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[0]);
				printf("%c ", ASCII);
			}
			else {
				if (guess[j][i] == 1) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
					printf("%c ", promasaj);
				}
				else {
					if (guess[j][i] == 2) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[6]);
						printf("%c ", pogodak);
					}
					else {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
						printf("%c ", potopljen);
					}
				}
			}
		}
	}
	while (ok && *cnt < 30) {      //30 je ukupan broj popunjenih polja
		i = rand() % 10;
		j = rand() % 10;
		if (guess[i][j] == 0) {
			gotoxy(35 + i, 2 * j);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
			printf("%c ", pogodak);
			Sleep(1000);
			guess[i][j] = 1;        //1 -> gadjao ali nije pogodio
			if (opp[i][j].size != 0) {
				guess[i][j] = 2;    //2 -> gadjao i pogodio
				(*cnt)++;
				int a = isSunk(i, j, opp, guess, igra);
				if (!a) {
					gotoxy(35 + i, 10 + 2 * j);											//mozda treba menjati
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[6]);
					printf("%c", pogodak);
				}
			}
			else {
				ok = 0;
				gotoxy(35 + i, 10 + 2 * j);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[6]);
				printf("%c ", promasaj);
				Sleep(1000);
			}
		}
	}
}
void mediumMode(Ship opp[10][10], short guess[10][10], int *s, int *cnt, int *top,Game igra) {
	system("CLS");
	hidecursor();
	int i, j, ok = 1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoxy(35, 7);
	printf("Computer's move");
	gotoxy(35, 9);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[0]);
	for (i = 0; i < 10; i++) {
		printf("%d ", i + 1);
	}
	for (i = 0; i < 10; i++) {
		gotoxy(33, 10 + i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[0]);
		printf("%c ", 'A' + i);
		gotoxy(35, 10 + i);
		for (j = 0; j < 10; j++) {
			if (guess[j][i] == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[0]);
				printf("%c ", ASCII);
			}
			else {
				if (guess[j][i] == 1) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
					printf("%c ", promasaj);
				}
				else {
					if (guess[j][i] == 2) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[6]);
						printf("%c ", pogodak);
					}
					else {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
						printf("%c ", potopljen);
					}
				}
			}
		}
	}

	while (ok && *cnt < 30) {
		if (*top == 0) {
			do {
				i = rand() % 10;
				j = rand() % 10;
			} while (guess[i][j] != 0 && !(i % 2) && !(j % 2));
			push(s, i, top);
			push(s, j, top);
		}
		j = pop(s, top);
		i = pop(s, top);
		if (guess[i][j] == 0) {										//nije gadjao 
			gotoxy(35 + 2 * i, 10 + j);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
			printf("%c ", pogodak);
			Sleep(1000);
			guess[i][j] = 1;
			if (opp[i][j].size != 0) {
				(*cnt)++;
				guess[i][j] = 2;
				if (!isSunk(i, j, opp, guess, igra)) {
					gotoxy(35 + 2 * i, 10 + j);											//mozda treba menjati
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
					printf("%c", pogodak);
				}
				if ((i + 1) <= 9 && guess[i + 1][j] == 0) {
					push(s, i + 1, top);
					push(s, j, top);
				}
				if ((i - 1) >= 0 && guess[i - 1][j] == 0) {
					push(s, i - 1, top);
					push(s, j, top);
				}
				if ((j + 1) <= 9 && guess[i][j + 1] == 0) {
					push(s, i, top);
					push(s, j + 1, top);
				}
				if ((j - 1) >= 0 && guess[i][j - 1] == 0) {
					push(s, i, top);
					push(s, j - 1, top);
				}
			}
			else {
				ok = 0;
				gotoxy(35 + 2 * i, 10 + j);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[6]);
				printf("%c ", promasaj);
				Sleep(1000);
			}
		}
	}
	return;
}