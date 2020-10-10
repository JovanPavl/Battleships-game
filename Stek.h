#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
void push(short *s, short b, int *top);
short pop(short *s, int *top);
int stackEmpty(short *s, int *top);