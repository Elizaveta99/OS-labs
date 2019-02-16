#pragma once

DWORD WINAPI CalculatePolynom(LPVOID polynom);

const int maxSize = 100;

struct Polynom
{
	int degree;
	int x;
	int value;
	int coef[maxSize];
};
