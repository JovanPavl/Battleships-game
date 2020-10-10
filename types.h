#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct ship {
	short size, num;
} Ship;
typedef struct  game {
	char match[30];
	char name1[30];
	char name2[30];     //imena igraca
	int cur; // trentuni igrac(1 ili 2)
	int SL;
	int mode;
	int em;
	int time;    //vreme u sekundama
	int A[7];		//boje brodova i table
	Ship p1[10][10], p2[10][10];
	short g1[10][10], g2[10][10];
} Game;

typedef struct player {
	char name[30];     //ime
	int wins;       //broj pobeda
	int cnt;        //broj gadjanja
} Player;

typedef struct elem { //cvor liste za Hall of Fame
	Player info;
	struct elem *next;
} Elem;
