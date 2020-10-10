#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
//#include "structures.h"   ne znam treba li
void push(short *s, short b, int *top) {
	(*top)++;
	s[(*top)] = b;
	return;
}

int stackEmpty(short *s, int *top) {
	if ((*top) == 0) return 1;
	return 0;
}

short pop(short *s, int *top) {
	short r;
	if (!(stackEmpty(s, top))) {
		r = s[*top];
		(*top)--;
	}
	return r;  
}