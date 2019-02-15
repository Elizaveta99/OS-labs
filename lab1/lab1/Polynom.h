#pragma once

DWORD WINAPI CalculatePolynom(LPVOID polynom);

CONST DWORD maxSize = 100;

struct Polynom
{
	DWORD degree;
	DWORD x;
	DWORD value;
	DWORD coef[maxSize];
};
