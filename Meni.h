#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
void gotoxy(int, int);
void hidecursor();
int check2(int, int);
void ChooseFleds(int, Ship[10][10], Game,int);
void SetShips(Game, Ship[10][10]);
void Colors(Game*);
void options(Game*);
void Attack(char[30], short[10][10], Game, short[10][10],short*,int*,int*,int*);
void AttackCell(short[10][10], short[10][10], Game,short*,int*,int*,int*);
int getit(int);
int eq(int);
Ship** setBoats_AI();
int isSunk(int, int, Ship[10][10], short[10][10], Game igra);
Game* loadGame();
void saveGame(Game *);
void endMatch(char[30], int, char[30], int, int);
void HallofFame();
void mediumMode(Ship **, short **, int *, int *, int *, Game);
void easyMode(short **, Ship **, Game, short *);