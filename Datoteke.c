#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include "types.h"
#include "Meni.h"
#define p 10001
int toi(char * x) {
	int i = 0;
	int sol = 0;
	for (i = 0; i < strlen(x); i++) {
		sol += x[i];
	}
	return sol;
}
int pow(int n) {
	if (n == 0)	return 1;
	if (n == 1)	return p;
	int z = pow(n / 2);
	if (n % 2 == 0)	return z * z;
	return z * z*p;
}
int mc(int a, int b) {
	return (a > b) ? b : a;
}
Game* loadGame() {
	system("CLS");
	hidecursor();
	int i,ok = 0,j,len;
	FILE *f, *f1;
	Game *arr, load;
	char gameName[31];


	int n; //broj partija
	f1 = fopen("about.txt", "r");
	if (f1 != NULL) {
		fscanf(f1, "%d", &n);
		fclose(f1);
	}
	arr = malloc(n * sizeof(Game));   //niz velicine koliko ima partija ukupno
	f = fopen("games.bin", "rb");
	if (f == NULL) exit(1);
	for (int i = 0; i < n; i++) {
		fread(&arr[i], sizeof(Game), 1, f);									//u arr su igre..
	}
	fclose(f);
	if (n == 0) {
		gotoxy(35, 15);
		printf("NO GAMES LOADED");
		Sleep(2000);
		return NULL;
	}
	i = 0;
	int cur = 0;
	int tmp = n;
	int have[100];
	int page = n / 5;
	if (n % 5 != 0) page++;
	int curpage = 0;
	Game **matrix = malloc(page * sizeof(Game*));
	while(tmp){
		have[i] = mc(5, tmp);
		matrix[i++] = malloc(mc(5, tmp) * sizeof(Game));					//u matrici igre
		for (j = 0; j < mc(5, tmp); j++) {
	//		printf("Stavlja u %d %d %d\n", i - 1, j, cur);
			matrix[i - 1][j] = arr[cur++];
		}
		tmp -= mc(5, tmp);
	}					//ocitao
	char c = '0';
	ok = 0;
	int sh = 0;
	while (c != 13) {
		if (!ok) {
			system("CLS");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			for (i = 0; i < have[curpage]; i++) {
				gotoxy(35, 13 + 2 * i);
				printf("%s", matrix[curpage][i].match);
			}
			gotoxy(35, 13+have[curpage] * 2);
			printf("BACK");
			gotoxy(35, 13+have[curpage] * 2 + 2);
			printf("PAGE %d", curpage);
			ok = 1;		//promenila se stranica
			sh = 0;
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		gotoxy(32, 13 + sh);
		printf("->");
		gotoxy(70, 13 + sh);
		printf("<-");
		c = _getch();
		gotoxy(32, 13 + sh);               //brisi
		printf("  ");
		gotoxy(70, 13 + sh);
		printf("  ");
		switch (c) {
		case 'w':
			if (sh == 0) {
				sh = 2 * have[curpage];
			}
			else {
				sh -= 2;
			}
			break;
		case 's':
			if (sh == 2 * have[curpage]) {
				sh = 0;
			}
			else
				sh += 2;
			break;
		case 'a':
			ok = 0;
			if (curpage == 0)
				curpage == page;
			else
				curpage--;
			break;
		case 'd':
			ok = 0;
			if (curpage == page - 1)
				curpage = 0;
			else
				curpage++;
			break;
		}
	}
	if (sh == have[curpage] * 2) {
		return NULL;					//ucitao BACK
	}
	return &matrix[curpage][sh / 2];
	return &load;
}

void saveGame(Game *saved) {
	FILE *f, *f1;
	int matchNum, playerNum;

	f1 = fopen("about.txt", "r");
	if (f1 == NULL) exit(1);
	fscanf(f1, "%d%d", &matchNum, &playerNum);
	matchNum++;

	f1 = fopen("about.txt", "w");
	if (f1 == NULL) exit(1);
	fprintf(f1, "%d %d", matchNum, playerNum);
	fclose(f1);

	f = fopen("games.bin", "a+b");
	if (f != NULL) {
		fwrite(saved, sizeof(Game), 1, f);
	}
	fclose(f);

	return;
}

void endMatch(char name1[30], int num1, char name2[30], int num2, int win) {    //argumenti fje su imena igraca i broj pogadjanja i 1/2
	FILE *f, *f1;
	int n, i, matchNum, okWin = 0, okLose = 0;  //n je ukupan broj igraca
	char *winner, *loser;
	Player *arr, *newPl;

	f1 = fopen("about.txt", "r");
	if (f1 == NULL) return;
	fscanf(f1, "%d%d", &matchNum, &n);

	arr = malloc(n * sizeof(Player));
	f = fopen("best_players.bin", "rb");
	if (f != NULL) {
		for (int j = 0; j < n; j++) {
			fread(&arr[j], sizeof(Player), 1, f);   //citamo sve iz datoteke
		}
	}
	else return;
	fclose(f);
	if (win == 1) {
		winner = malloc(strlen(name1) * sizeof(char));
		strcpy(winner, name1);
		winner[strlen(name1)] = '\0';
		loser = malloc(strlen(name2) * sizeof(char));
		strcpy(loser, name2);
		loser[strlen(name2)] = '\0';
	}
	else {
		winner = malloc(strlen(name2) * sizeof(char));
		strcpy(winner, name2);
		winner[strlen(name2)] = '\0';
		loser = malloc(strlen(name1) * sizeof(char));
		strcpy(loser, name1);
		loser[strlen(name1)] = '\0';
	}
	for (i = 0; i < n; i++) {         //azurira se broj pogadjanja i dodaje se jedna pobeda pobedniku
		if (strcmp(arr[i].name, winner) == 0) {
			arr[i].wins++;
			arr[i].cnt += num1;
			okWin = 1;
		}
		if (strcmp(arr[i].name, loser) == 0) {
			arr[i].cnt += num2;
			okLose = 1;
		}
	}
	//Nece naci COMP,srecom
	f = fopen("best_players.bin", "wb");
	int hes = 0;
	if (f != NULL) {
		if (!(okWin) && strcmp(winner,"COMP")) {						//COMP je racunar,ne upisuj ga
			newPl = malloc(sizeof(Player));
			newPl->wins = 1;
			newPl->cnt = num1;
			strcpy(newPl->name, winner);
			newPl->name[strlen(winner)] = '\0';
			fwrite(newPl, sizeof(Player), 1, f);
			hes += pow(toi(newPl->name));												//promena,ponovo hesuj
			hes += pow(newPl->wins);
			hes += pow(newPl->cnt);
			free(newPl);
		}
		if (!(okLose) && strcmp(loser,"COMP")) {							//COMP je racunar ne upisuj ga
			newPl = malloc(sizeof(Player));
			newPl->wins = 0;
			newPl->cnt = num2;
			strcpy(newPl->name, loser);
			newPl->name[strlen(loser)] = '\0';
			fwrite(newPl, sizeof(Player), 1, f);
			hes += pow(toi(newPl->name));										//promena,ponovo hesuj!
			hes += pow(newPl->wins);
			hes += pow(newPl->cnt);
			free(newPl);
		}
		fwrite(arr, sizeof(Player), n, f);
	}
	else return;
	fclose(f);
	int lol;
	FILE* MOS = fopen("HES.txt", "w");
	if (((!okLose) && strcmp(loser, "COMP")) || ((!okWin) && strcmp(winner, "COMP"))) {
		f = fopen("best_players.bin", "ab");
	}
	else {
		f = fopen("best_players.bin", "wb");
	}
	for (i = 0; i < n; i++) {
		fwrite(&arr[i], sizeof(Player), 1, f);
		hes += pow(toi(arr[i].name));
		hes += pow(arr[i].wins);
		hes += pow(arr[i].cnt);
	}
	fclose(f);
	fprintf(MOS,"%d",hes);
	fclose(MOS);
	if ((!okWin) && strcmp(winner, "COMP")) n++;
	if ((!okLose) && strcmp(loser, "COMP")) n++;
//	matchNum++;
	f1 = fopen("about.txt", "w");
	fprintf(f1, "%d %d", matchNum, n);
	fclose(f1);
	return;
}
void HallofFame() {										//dobro hesovano..
	int sol;
	FILE* f2 = fopen("HES.txt", "r");
	fscanf(f2, "%d", &sol);
	fclose(f2);
	int hes = 0;
	system("CLS");
	FILE *f, *f1;
	Elem *first, *newEl, *cur, *old;
	Player *arr, tmp;
	int playerCnt, i, j, cnt, matchNum, playerNum;

	f1 = fopen("about.txt", "r");
	if (f1 == NULL) return;
	fscanf(f1, "%d%d", &matchNum, &playerNum);
	fclose(f1);

	arr = malloc(playerNum * sizeof(Player));
	f = fopen("best_players.bin", "rb");
	if (f != NULL) {
		for (i = 0; i < playerNum; i++) {
			fread(&arr[i], sizeof(Player), 1, f);   //citamo sve iz datoteke
			hes += pow(toi(arr[i].name));										//hesuj sve rezultate
			hes += pow(arr[i].wins);
			hes += pow(arr[i].cnt);
		}
	}
	else return;
	if (hes != sol) {
		system("CLS");
		gotoxy(35, 15);
		printf("FILE WAS CHANGED!",hes);
		Sleep(5000);
		exit(0);
	}
	if (playerNum > 10) cnt = 10;
	else cnt = playerNum;

	for (i = 0; i < playerNum - 1; i++) {         //sortiramo ono sto smo procitali iz datoteke
		for (j = i; j < playerNum; j++) {
			if (arr[j].wins > arr[i].wins) {
				tmp = arr[j];
				arr[j] = arr[i];
				arr[i] = tmp;
			}
		}
	}
	first = NULL;
	for (i = cnt - 1; i >= 0; i--) {
		newEl = malloc(sizeof(Elem));
		if (newEl == NULL) {
			printf("Neuspesna alokacija (HallofFame)");
			return;
		}
		strcpy(newEl->info.name, arr[i].name);
		newEl->info.name[strlen(arr[i].name)] = '\0';
		newEl->info.cnt = arr[i].cnt;
		newEl->info.wins = arr[i].wins;

		newEl->next = first;
		first = newEl;
	}

	/*DODAJ ISPIS NA EKRAN*/
	int useit = 0,aa=0,sz=0,sh=0;
	cur = first;
	while (cur && useit!=10) {
		sz++;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		gotoxy(30, 5+aa);
		printf("%-20s %10d %10d", cur->info.name, cur->info.cnt, cur->info.wins);
		aa += 2;
		useit++;
		cur = cur->next;
	}
	gotoxy(45, 5 + aa);
	printf("BACK");
	char c ='0';
	while (c != 13 || sh != aa) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		gotoxy(27, 5 + sh);
		printf("->");
		gotoxy(85, 5 + sh);
		printf("<-");
		c = _getch();
		gotoxy(27, 5 + sh);
		printf("  ");
		gotoxy(85, 5 + sh);
		printf("  ");
		switch (c) {
		case 'w':if (sh == 0)
			sh = aa;
				 else
			sh -= 2;
			break;
		case 's':if (sh == aa)
			sh = 0;
				 else
			sh += 2;
		}
	}
	while (first) {						//dealocira datu listu
		old = first;
		first = first->next;
		free(old);
	}
	return;
}