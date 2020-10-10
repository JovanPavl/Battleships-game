#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include "types.h"
#include "Meni.h"
#define ASCII 254
#define BOAT 177
#define START1 37
#define START2 11
#define pogodak 'X'
#define promasaj 240
#define potopljen 248
#define boat2 'C'
#define boat3 'D'
#define boat4 'E'
#define boat5 'F'
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
int check2(int a, int b) {
	if (a<10 && a>-1 && b<10 && b>-1)
		return 1;
	return 0;
}
int getit(int a) {
	if (a == 0) return 0;
	if (a <= 4) return 1;
	if (a <= 7) return 2;
	if (a <= 9) return 3;
	return 4;
}
int eq(int x, int y) {
	if (y == 1) return(x != 0 && x <= 4);
	if (y == 2) return(x != 0 && x > 4 && x <= 7);
	if (y == 3)return (x != 0 && x > 7 && x <= 9);
	return x == 10;
}
void ChooseFleds(int cells, Ship matr[10][10], Game game, int flag) {                        //treba izabrati x polja
	char sos[5] = { 254,'C','D','E','F' };
	char c = '0';
	int i, j, x = 0, y = 0, le = cells + 1;
	Ship use[10][10];
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			use[i][j].num = matr[i][j].num;
			use[i][j].size = matr[i][j].size;
		}
	}
	int h = 1, v = 1;                                                        //moze horizontalno moze vertikalno
	while (le) {
		while (1) {
			gotoxy(35 + x, 13 + y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), game.A[cells]);
			printf("%c", ASCII/* sos[cells]*/);
			c = _getch();
			if (c == 13) break;
			gotoxy(35 + x, 13 + y);
			if (eq(use[x / 2][y].num, cells) != 1 && use[x / 2][y].num != -1) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), game.A[getit(matr[x / 2][y].num)]);              //vrati staru boju,laganini
				printf("%c", ASCII/*sos[getit(matr[x / 2][y].num)]*/);
			}
			switch (c) {
			case 'w':if (y == 0)
				y = 9;
					 else
				y--;
				break;
			case 's':if (y == 9)
				y = 0;
					 else
				y++;
				break;
			case 'a':if (x == 0)
				x = 18;
					 else
				x -= 2;
				break;
			case 'd':if (x == 18)
				x = 0;
					 else
				x += 2;
				break;
			default:break;
			}
		}
		//Kliknut je ENTER
		if (use[x / 2][y].num == -1) {            //vrati staru boju,brise trenutnu samo je najdesnja ili najlevlja
			int ok2 = 0;
			if (h && ((x / 2 == 9 || x == 0) || use[x / 2 + 1][y].num != -1 || use[x / 2 - 1][y].num != -1))         //najlevlja ili najdesnja
				ok2 = 1;
			if (v && ((y == 9 || y == 0) || use[x / 2][y + 1].num != -1 || use[x / 2][y - 1].num != -1))         //najgornja ili najdonja
				ok2 = 1;
			if (le == cells) ok2 = 1;                                                        //samo jedna
			if (ok2) {
				le++;
				if (le > cells - 2) {
					h = 1;
					v = 1;
				}
				use[x / 2][y] = matr[x / 2][y];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), game.A[getit(matr[x / 2][y].num)]);
				gotoxy(35 + x, 13 + y);
				printf("%c", ASCII);
			}
		}
		else {
			if (use[x / 2][y].num == 0) {                          //mora biti ne selektovana boja
				int ok = 0;
				if (le == cells + 1) {                                  //ako je prva odma upisi`
					ok = 1;
				}
				else {
					if (h == 1) {                                           //trazi horizontalno susedne
						if (check2(x / 2 - 1, y) && use[x / 2 - 1][y].num == -1) {
							ok = 1;
							v = 0;
						}
						if (check2(x / 2 + 1, y) && use[x / 2 + 1][y].num == -1) {
							ok = 1;
							v = 0;
						}
					}
					if (v == 1) {                                           //trazi vertikalne susede
						if (check2(x / 2, y - 1) && use[x / 2][y - 1].num == -1) {
							ok = 1;
							h = 0;
						}
						if (check2(x / 2, y + 1) && use[x / 2][y + 1].num == -1) {
							ok = 1;
							h = 0;
						}
					}
				}
				if (ok) {
					use[x / 2][y].num = -1;               //nova markiraj ga
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), game.A[cells]);
					gotoxy(35 + x, 13 + y);
					le--;
					printf("%c", ASCII);
				}
			}
		}
	}
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			if (use[i][j].num != -1)
				matr[i][j].num = use[i][j].num;
			else {
				matr[i][j].num = flag;
				matr[i][j].size = getit(flag);
			}
		}
	}
}
void SetShips(Game igra, Ship matr[10][10], char ime[30], int x) {
	system("CLS");
	gotoxy(35, 7);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	printf("PLAYER%d:", x);
	scanf("%s", ime);
	int sz = 6, i, j;
	char *cul[6] = { "TABLE","2 BOAT","3 BOAT","4 BOAT","5 BOAT" };
	int leftl[] = { 0,0,4,3,2,1 };
	int cur = 0, have = 2;
	gotoxy(35, 12);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[0]);
	for (i = 0; i < 10; i++) {
		printf("%d ", i + 1);
	}
	for (i = 0; i < 10; i++) {
		gotoxy(33, 13 + i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[0]);
		printf("%c ", 'A' + i);
		gotoxy(35, 13 + i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[0]);
		for (j = 0; j < 10; j++) {
			printf("%c ", ASCII);
		}
	}                                                   //iscrtavanje tabele
	while (cur < 10) {
		if (!leftl[have]) have++;
		gotoxy(35, 9);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		printf("%s : %d", cul[have - 1], leftl[have]);//tolko brodova te velicine
		ChooseFleds(have - 1, matr, igra, cur + 1);
		leftl[have]--;
		cur++;
	}
}
int isSunk(int x, int y, Ship opp[10][10], short guess[10][10], Game igra) {
	short tmp = opp[x][y].size;
	short **t;
	int i, cnt = 0;
	i = 1;
	t = malloc(2 * sizeof(short*));
	t[0] = malloc(2 * tmp*(sizeof(short)));
	t[1] = malloc(2 * tmp*(sizeof(short)));
	while ((x + i) <= 9 && i <= tmp) {       //proverava horizontalne 
		if (guess[x + i][y] == 2 && cnt < tmp && opp[x + i][y].size == tmp && opp[x + i][y].num == opp[x][y].num) {
			t[0][cnt] = x + i;
			t[1][cnt] = y;
			cnt++;
		}
		else
			break;
		i++;
	}
	i = 1;
	while ((x - i) >= 0 && i <= tmp) {       //proverava horizontalne 
		if (guess[x - i][y] == 2 && cnt < tmp && opp[x - i][y].size == tmp && opp[x - i][y].num == opp[x][y].num) {
			t[0][cnt] = x - i;
			t[1][cnt] = y;
			cnt++;
		}
		else
			break;
		i++;
	}
	if (cnt == tmp) {
		guess[x][y] = 3;
		gotoxy(35 + 2 * x, 10 + y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
		printf("%c", potopljen);
		for (int j = 0; j < tmp; j++) {
			gotoxy(35 + 2 * t[0][j], 10 + t[1][j]);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
			guess[t[0][j]][t[1][j]] = 3;
			printf("%c", potopljen);
		}
		free(t[0]);
		free(t[1]);
		free(t);
		return 1;
	}
	cnt = 0;
	i = 1;
	while ((y + i) <= 9 && i <= tmp) {       //proverava vertikalne
		if (guess[x][y + i] == 2 && cnt < tmp && opp[x][y + i].size == tmp && opp[x][y + i].num == opp[x][y].num) {
			t[0][cnt] = x;
			t[1][cnt] = y + i;
			cnt++;
		}
		else
			break;
		i++;
	}
	i = 1;
	while ((y - i) >= 0 && i <= tmp) {       //proverava vertikalne 
		if (guess[x][y - i] == 2 && cnt < tmp && opp[x][y - i].size == tmp && opp[x][y - i].num == opp[x][y].num) {
			t[0][cnt] = x;
			t[1][cnt] = y - i;
			cnt++;
		}
		else
			break;
		i++;
	}
	if (cnt == tmp) {
		guess[x][y] = 3;
		gotoxy(35 + 2 * x, 10 + y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
		printf("%c", potopljen);
		for (int j = 0; j < tmp; j++) {
			guess[t[0][j]][t[1][j]] = 3;
			gotoxy(35 + 2 * t[0][j], 10 + t[1][j]);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
			printf("%c", potopljen);
		}
		free(t[0]);
		free(t[1]);
		free(t);
		return 1;
	}
	free(t[0]);
	free(t[1]);
	free(t);
	return 0;
}
void COLORS(Game *igra) {
	int i = 0;
	char *cul[7] = { "TABLE","2 BOAT","3 BOAT","4 BOAT","5 BOAT","MISS","OK" };
	while (1) {
		system("CLS");
		hidecursor();
		for (i = 0; i < 7; i++) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra->A[i]);
			gotoxy(42, 8 + i * 2);
			printf("%s", cul[i]);
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		gotoxy(42, 22);
		printf("BACK");
		char c = '0';
		int sh = 0;
		while (c != 13 || sh != 14) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			gotoxy(39, 8 + sh);
			printf("->");
			gotoxy(55, 8 + sh);
			printf("<-");
			gotoxy(0, 0);
			c = _getch();
			gotoxy(39, 8 + sh);               //brisi
			printf("  ");
			gotoxy(55, 8 + sh);
			printf("  ");
			if (c == 's') {
				if (sh < 14) {
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
					sh = 14;
			}
			if (c == 'a' && sh != 14) {
				int use = sh / 2;
				igra->A[use]--;
				if (igra->A[use] < 0)
					igra->A[use] += 15;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra->A[use]);
				gotoxy(42, 8 + sh);
				printf("%s", cul[use]);
			}
			if (c == 'd' && sh != 14) {
				int use = sh / 2;
				igra->A[use]++;
				if (igra->A[use] > 15)
					igra->A[use] -= 15;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra->A[use]);
				gotoxy(42, 8 + sh);
				printf("%s", cul[use]);
			}
		}
		return;
	}
}
void ChangeName(Game *igra) {		
	system("CLS");
	hidecursor();
	char *x[2] = { "PLAYER 1 :","PLAYER 2 :" };
	int ff = 0;
	int sh = 0;
	char c = '0';
	int lol = 0;
	if (igra->mode == 0) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		gotoxy(37, 13);
		printf("PLAYERS'S NAME : %20s", igra->name1);
		gotoxy(37, 15);
		printf("BACK");
		while (c != 13 || sh != 2) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			gotoxy(34, 13 + sh);
			printf("->");
			gotoxy(70, 13 + sh);
			printf("<-");
			c = _getch();
			gotoxy(34, 13 + sh);               //brisi
			printf("  ");
			gotoxy(70, 13 + sh);
			printf("  ");
			switch (c) {
			case 'w':
				if (sh == 0)
					sh == 2;
				else
					sh -= 2;
				break;
			case 's':
				if (sh == 2)
					sh = 0;
				else
					sh += 2;
				break;
			}
			if (c == 13 && sh == 0) {							//menja ime
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gotoxy(54, 13);
				scanf("%s", igra->name1);
			}
		}
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		gotoxy(37, 13);
		if (!ff)	printf("%s %15s", x[ff], igra->name1);
		else
		{
			printf("%s %15s", x[ff], igra->name2);
		}
		gotoxy(37, 15);
		printf("BACK");
		while (c != 13 || sh != 2) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			gotoxy(34, 13 + sh);
			printf("->");
			gotoxy(70, 13 + sh);
			printf("<-");
			c = _getch();
			gotoxy(34, 13 + sh);               //brisi
			printf("  ");
			gotoxy(70, 13 + sh);
			printf("  ");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			switch (c) {
			case 'w':
				if (sh == 0)
					sh == 2;
				else
					sh -= 2;
				break;
			case 's':
				if (sh == 2)
					sh = 0;
				else
					sh += 2;
				break;
			case 'a':
				if (sh == 0) {
					ff ^= 1;
					gotoxy(37, 13);
					if (!ff)
						printf("%s %15s", x[ff], igra->name1);
					else
						printf("%s %15s", x[ff], igra->name2);
				}
				break;
			case 'd':
				if (sh == 0) {
					ff ^= 1;
					gotoxy(37, 13);
					if (!ff)
						printf("%s %15s", x[ff], igra->name1);
					else
						printf("%s %15s", x[ff], igra->name2);
				}
				break;
			}
			if (c == 13 && sh == 0) {							//menja ime
				if (ff == 0) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					gotoxy(48, 13);
					scanf("%s", igra->name1);
				}
				else {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					gotoxy(48, 13);
					scanf("%s", igra->name2);
				}
			}
		}
	}
}
void options(Game *igra) {
	char cc;
	char *needme[] = { "SINGLE PLAYER","2 PLAYERS    " };
	char *emara[] = { "REGULAR","EASY   " };
	while (1) {
		system("CLS");
		hidecursor();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		gotoxy(37, 9);
		printf("CHANGE NAME");
		gotoxy(37, 11);
		printf("SAVE GAME");
		gotoxy(37, 13);
		printf("BACK");
		gotoxy(37, 15);
		printf("%s", needme[igra->mode]);                //"2 PLAYERS    "
		gotoxy(37, 17);
		printf("%s", emara[igra->em]);
		gotoxy(37, 19);
		printf("CHOOSE COLORS");
		char c = '0';
		int sh = 0;                      //1 za single player 0 za MULTIPLEJER
		while (c != 13) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			gotoxy(34, 9 + sh);
			printf("->");
			gotoxy(51, 9 + sh);
			printf("<-");
			gotoxy(0, 0);
			c = _getch();
			gotoxy(34, 9 + sh);               //brisi
			printf("  ");
			gotoxy(51, 9 + sh);
			printf("  ");
			if (c == 'a' && sh == 6) {
				igra->mode ^= 1;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gotoxy(37, 9 + sh);
				printf("%s", needme[igra->mode]);
			}
			if (c == 'd' && sh == 6) {
				igra->mode ^= 1;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gotoxy(37, 9 + sh);
				printf("%s", needme[igra->mode]);
			}
			if (c == 'a' && sh == 8) {
				igra->em ^= 1;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gotoxy(37, 9 + sh);
				printf("%s", emara[igra->em]);
			}
			if (c == 'd' && sh == 8) {
				igra->em ^= 1;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gotoxy(37, 9 + sh);
				printf("%s", emara[igra->em]);
			}
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
		switch (sh) {                                    //klinkuo je enter
		case 2:system("CLS");
			gotoxy(35, 13);
			printf("Name of game: ");
			scanf("%s", igra->match);
			saveGame(igra);
			break;
		case 0:ChangeName(igra);
		case 4:return;
		case 10:COLORS(igra);
			break;
		}
	}
}
void Attack(char A[30], short matr[10][10], Game igra, Ship matr2[10][10], short *cnt, int* ok, int *shots,int* mzd) {								//treba menjati matricu pogadjanja
	int i = 0, j = 0;
	system("CLS");
	gotoxy(35, 7);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	printf("%s's move", A);
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
			if (matr[j][i] == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[0]);
				printf("%c ", ASCII);
			}
			else {
				if (matr[j][i] == 1) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
					printf("%c ", promasaj);
				}
				else {
					if (matr[j][i] == 2) {
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
	//iscrtavanje tabele
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
	gotoxy(38, 26);
	printf("SHIPS LEFT %d ", *mzd);
	gotoxy(38, 27);
	printf("SHIPS SUNK %d ", 10 - *mzd);
	AttackCell(matr, matr2, igra, cnt, ok, shots,mzd);
}
void AttackCell(short matr[10][10], Ship matr2[10][10], Game igra, short* cnt, int *flag, int *shots,int* mzd) {					//vraca 1 ako je pogodio inace 0
	int x = 0, y = 0;
	char c = '0';
	int ok = 1, i, j;
	while (ok && *cnt != 30) {
		while (1) {
			gotoxy(35 + x, 10 + y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[6]);
			printf("%c ", pogodak);
			c = _getch();
			if (c == 27) {
				*flag = 0;
				return;
			}
			int needme = 1;
			if (c == 'h') {								//hoce hint
				for (i = 0; i < 10 && needme; i++) {
					for (j = 0; j < 10; j++) {
						if (!matr[i][j] && matr2[i][j].size) {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[6]);
							gotoxy(35 + 2*i, 10+j);
							printf("O");															//oznaci gde da gadja
							Sleep(1000);
							needme = 0;
							gotoxy(35 + 2 * i, 10 + j);
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[0]);			//vrati mu staru boju
							printf("%c ", ASCII);
							break;
						}
					}
				}
			}
			if (c == 13)	break;
			gotoxy(35 + x, 10 + y);
			if (matr[x / 2][y] != 2) {
				if (!matr[x / 2][y]) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[0]);
					printf("%c ", ASCII);
				}
				else {
					if (matr[x / 2][y] == 1) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[matr[x / 2][y] + 4]);              //vrati staru boju,laganini
						printf("%c ", promasaj);
					}
					else {
						if (matr[x / 2][y] == 2) {
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
			switch (c) {
			case 'w':if (y == 0)
				y = 9;
					 else
				y--;
				break;
			case 's':if (y == 9)
				y = 0;
					 else
				y++;
				break;
			case 'a':if (x == 0)
				x = 18;
					 else
				x -= 2;
				break;
			case 'd':if (x == 18)
				x = 0;
					 else
				x += 2;
				break;
			default:break;
			}
		}
		*shots = *shots + 1;				//gadja
		if (!matr2[x / 2][y].size || matr[x / 2][y]) {//nije pogodio ili je polje vec gadjano
			ok = 0;
			gotoxy(35 + x, 10 + y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[5]);
			printf("%c", promasaj);
			if (!matr[x / 2][y]) matr[x / 2][y] = 1;		//ako nije gadjann,promasaj je
			gotoxy(40,22);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),igra.A[5]);
			printf("MISS !");
			Sleep(3000);
		}
		else {
			(*cnt)++;
			matr[x / 2][y] = 2;
			int a = isSunk(x / 2, y, matr2, matr, igra);
			if (!a) {
				gotoxy(35 + x, 10 + y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[6]);
				printf("%c", pogodak);
				gotoxy(40, 22);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[6]);
				printf("OK !");
			}
			else {
				gotoxy(40,22);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), igra.A[6]);
				printf("SUNK !  ");
				(*mzd)--;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gotoxy(38, 26);
				printf("SHIPS LEFT %d ", *mzd);
				gotoxy(38, 27);
				printf("SHIPS SUNK %d ", 10 - *mzd);
			}
		}
	}
}

Ship** setBoats_AI() {
	enum direction { horizontal, vertical } dir;
	Ship tab[10][10];
	Ship **r;
	int i, j, num, size, boats[4], tmp, t, ok = 1, cnt = 1;

	num = 4;
	for (i = 0; i < 4; i++) {   //i+2 je velicina
		boats[i] = num;
		num--;
	}
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			tab[i][j].size = 0;
			tab[i][j].num = 0;
		}
	}
	//Obavezna polja
	srand(time(NULL));
	//Gornji desni ugao 2x2
	i = rand() % 2;
	j = rand() / (double)RAND_MAX*(10 - 8) + 8;
	dir = rand() % 2;
	do {
		size = rand() / (double)RAND_MAX*(6 - 2) + 2;
	} while (boats[size - 2] == 0);
	//	printf("%d %d size=%d dir=%d\n", i, j, size, dir);
	if (dir == horizontal) {
		tmp = size;
		while (tmp) {
			tab[i][j].size = size - 1;
			tab[i][j].num = cnt;
			tmp--;
			j--;
		}
		boats[size - 2]--;
		cnt++;
	}
	else if (dir == vertical) {
		tmp = size;
		while (tmp) {
			tab[i][j].size = size - 1;
			tab[i][j].num = cnt;
			tmp--;
			i++;
		}
		boats[size - 2]--;
		cnt++;
	}
	//Donji desni ugao 3x3
	i = rand() / (double)RAND_MAX *(10 - 7) + 7;
	j = rand() / (double)RAND_MAX*(10 - 7) + 7;
	dir = rand() % 2;
	do {
		size = rand() / (double)RAND_MAX*(6 - 2) + 2;
	} while (boats[size - 2] == 0);
	//	printf("%d %d size=%d dir=%d\n", i, j, size, dir);
	if (dir == horizontal) {
		tmp = size;
		while (tmp) {
			tab[i][j].size = size - 1;
			tab[i][j].num = cnt;
			tmp--;
			j--;
		}
		boats[size - 2]--;
		cnt++;
	}
	else if (dir == vertical) {
		tmp = size;
		while (tmp) {
			tab[i][j].size = size - 1;
			tab[i][j].num = cnt;
			tmp--;
			i--;
		}
		boats[size - 2]--;
		cnt++;
	}
	while (boats[0] > 0 || boats[1] > 0 || boats[2] > 0 || boats[3] > 0) {
		ok = 1;
		i = rand() % 10;
		j = rand() % 10;
		dir = rand() % 2;
		size = rand() / (double)RAND_MAX*(6 - 2) + 2;
		if (i != 4 && i != 6 && j != 4 && j != 6 && boats[size - 2]) {
			if (dir == horizontal) {
				tmp = size;
				t = j;
				if (j - size < 0) ok = 0;
				while (tmp) {
					if (tab[i][t].size != 0) {
						ok = 0;
						break;
					}
					tmp--;
					t--;
				}
				tmp = size;
				while (tmp && ok) {
					tab[i][j].size = size - 1;
					tab[i][j].num = cnt;
					tmp--;
					j--;
				}
				if (ok) {
					boats[size - 2]--;
					cnt++;
					//					printf("hor postavio brod %d od %d %d do %d %d\n", size - 1, i, j + size + 1, i, j);
				}
			}
			else if (dir == vertical) {
				tmp = size;
				t = i;
				if (i + size > 9) ok = 0;
				while (tmp) {
					if (tab[t][j].size != 0) {
						ok = 0;
						break;
					}
					tmp--;
					t++;
				}
				tmp = size;
				while (tmp && ok) {
					tab[i][j].size = size - 1;
					tab[i][j].num = cnt;
					tmp--;
					i++;
				}
				if (ok) {
					boats[size - 2]--;
					cnt++;
					//					printf("ver postavio brod %d na %d %d do %d %d\n", size - 1, i - size, j, i + 1, j);
				}

			}
		}
	}

	r = malloc(10 * sizeof(Ship*));
	for (i = 0; i < 10; i++) {
		r[i] = malloc(10 * sizeof(Ship));
		for (j = 0; j < 10; j++) {
			r[i][j] = tab[i][j];
		}
	}
	return r;
}
void Zavrsi(int cnt1, int cnt2, int shots1, int shots2, Game igra) {
	system("CLS");
	hidecursor();
	gotoxy(30, 8);
	printf(" .----------------.  .-----------------. .----------------.");
	gotoxy(30, 9);
	printf("| .--------------. || .--------------. || .--------------. |");
	gotoxy(30, 10);
	printf("| |  _________   | || | ____  _____  | || |  ________    | |");
	gotoxy(30, 11);
	printf("| | |_   ___  |  | || ||_   \\|_   _| | || | |_   ___ `.  | |");
	gotoxy(30, 12);
	printf("| |   | |_  \\_|  | || |  |   \\ | |   | || |   | |   `. \\ | |");
	gotoxy(30, 13);
	printf("| |   |  _|  _   | || |  | |\\ \\| |   | || |   | |    | | | |");
	gotoxy(30, 14);
	printf("| |  _| |___/ |  | || | _| |_\\   |_  | || |  _| |___.' / | |");
	gotoxy(30, 15);
	printf("| | |_________|  | || ||_____|\\____| | || | |________.'  | |");
	gotoxy(30, 16);
	printf("| |              | || |              | || |              | |");
	gotoxy(30, 17);
	printf("| '--------------' || '--------------' || '--------------' |");
	gotoxy(30, 18);
	printf(" '----------------'  '----------------'  '----------------'");
	Sleep(6000);
	if (cnt1 == 30) {
		endMatch(igra.name1, shots1, igra.name2, shots2, 1);
		return;
	}
	else {
		endMatch(igra.name1, shots1, igra.name2, shots2, 2);
	}
}
void ControllGame(Game* igra, int NG) {					//NG 1 ako jeste nova igra inace 0
	int *s = malloc(60 * sizeof(int));
	int *top = malloc(sizeof(int));
	*top = 0;
	int i = 0, j = 0;
	int flag = 1, cnt1 = 0, cnt2 = 0;
	int winner, cur = 0, shots1 = 0, shots2 = 0;
	srand(time(NULL));
	int x = rand();
	if (NG == 1) {
		igra->SL = 10;
		if (igra->mode == 1) {
			SetShips(*igra, igra->p1, igra->name1, 1);
			SetShips(*igra, igra->p2, igra->name2, 2);
			while (flag && cnt1 != 30 && cnt2 != 30) {
				if (x % 2 == 0) {
					igra->cur = 0;																//1. pocinje
					Attack(igra->name1, igra->g1, *igra, igra->p2, &cnt1, &flag, &shots1,&igra->SL);
					if (!flag/* || cnt1 == 30*/) {
						return;
					}
					if (cnt1 == 30) break;
					Attack(igra->name2, igra->g2, *igra, igra->p1, &cnt2, &flag, &shots2,&igra->SL);
					igra->cur = 1;																		//2.pocinje
				}
				else {
					igra->cur = 1;
					Attack(igra->name2, igra->g2, *igra, igra->p1, &cnt2, &flag, &shots2,&igra->SL);
					if (!flag/* || cnt2 == 30*/) return;
					if (cnt2 == 30) break;
					Attack(igra->name1, igra->g1, *igra, igra->p2, &cnt1, &flag, &shots1, &igra->SL);
					igra->cur = 0;
				}
			}
			if (flag)	Zavrsi(cnt1, cnt2, shots1, shots2, *igra);
		}
		else {
			igra->name2[0] = 'C', igra->name2[1] = 'O', igra->name2[2] = 'M', igra->name2[3] = 'P', igra->name2[4] = 0;
			SetShips(*igra, igra->p1, igra->name1, 1);									//postavlja prvi igrac
			Ship** matr = malloc(10 * sizeof(Ship*));
			for (i = 0; i < 10; i++) matr[i] = malloc(10 * sizeof(Ship));
			matr = setBoats_AI();
			Ship pl2[10][10];
			for (i = 0; i < 10; i++) {
				for (j = 0; j < 10; j++) {
					igra->p2[i][j] = matr[i][j];									//tako je racunar postavio brodove
				}
			}
			while (flag && cnt1 != 30 && cnt2 != 30) {
				if (x % 2 == 0) {
					igra->cur = 0;													//samo drugi moze da prekine igru
					Attack(igra->name1, igra->g1, *igra, igra->p2, &cnt1, &flag, &shots1,&igra->SL);
					if (!flag/* || cnt1 == 30*/) return;
					if (cnt1 == 30)break;
					if (!igra->em)
						easyMode(igra->g2, igra->p1, *igra, &cnt2);
					else
						mediumMode(igra->p1, igra->g2, s, &cnt2, top, *igra);
				}
				else {
					if (!igra->em)
						easyMode(igra->g2, igra->p1, *igra, &cnt2);
					else
						mediumMode(igra->p1, igra->g2, s, &cnt2, top, *igra);
					if (cnt2 == 30) break;
					Attack(igra->name1, igra->g1, *igra, igra->p2, &cnt1, &flag, &shots1,&igra->SL);
					igra->cur = 1;						//moze samo 2. da prekine igru
				}
			}
			if (flag)	Zavrsi(cnt1, cnt2, shots1, shots2, *igra);
		}
	}
	else {
		for (i = 0; i < 10; i++) {
			for (j = 0; j < 10; j++) {						//trenutna stanja
				if (igra->g1[i][j])
					shots1++;
				if (igra->g1[i][j] == 2 || igra->g1[i][j] == 3)
					cnt1++;
				if (igra->g2[i][j])
					shots2++;
				if (igra->g2[i][j] == 2 || igra->g2[j][j] == 3)
					cnt2++;
			}
		}
		int useit = igra->cur;
		if (igra->mode == 1) {						//2 igraca
			while (flag && cnt1 != 30 && cnt2 != 30) {
				if (useit == 0) {													//1. je na potezu
					igra->cur = 0;
					Attack(igra->name1, igra->g1, *igra, igra->p2, &cnt1, &flag, &shots1,&igra->SL);
					if (!flag/* || cnt1 == 30*/) return;
					if (cnt1 == 30)	break;
					Attack(igra->name2, igra->g2, *igra, igra->p1, &cnt2, &flag, &shots2,&igra->SL);
					igra->cur = 1;
				}
				else {
					igra->cur = 1;
					Attack(igra->name2, igra->g2, *igra, igra->p1, &cnt2, &flag, &shots2,&igra->SL);
					if (!flag)	return;
					if (cnt2 == 30) break;
					Attack(igra->name1, igra->g1, *igra, igra->p2, &cnt1, &flag, &shots1,&igra->SL);
					igra->cur = 0;
				}
			}
			if (flag)	Zavrsi(cnt1, cnt2, shots1, shots2, *igra);
		}
		else {
			igra->cur = 0;
			while (flag && cnt1 != 30 && cnt2 != 30) {
				if (igra->cur % 2 == 0) {
					Attack(igra->name1, igra->g1, *igra, igra->p2, &cnt1, &flag, &shots1,&igra->SL);
					if (!flag/* || cnt1 == 30*/) return;
					if (cnt1 == 30)	break;
					if (!igra->em)
						easyMode(igra->g2, igra->p1, *igra, &cnt2);
					else
						mediumMode(igra->p1, igra->g2, s, &cnt2, top, *igra);
				}
				else {
					if (!igra->em)
						easyMode(igra->g2, igra->p1, *igra, &cnt2);
					else
						mediumMode(igra->p1, igra->g2, s, &cnt2, top, *igra);
					if (cnt2 == 30) break;
					Attack(igra->name1, igra->g1, *igra, igra->p2, &cnt2, &flag, &shots1,&igra->SL);
				}
			}
			if (flag)	Zavrsi(cnt1, cnt2, shots1, shots2, *igra);
		}
	}
}