#include <windows.h>
#include <iostream>
#include "Polynom.h"

using namespace std;

DWORD WINAPI CalculatePolynom(LPVOID polynom)
{
	Polynom* pol = (Polynom*)polynom;
	int xDegree = 1; 
	for (int i = (*pol).degree; i > -1 ; i--) // ?? int ?? order?
	{
		(*pol).value += (*pol).coef[i] * xDegree;
		xDegree *= (*pol).x;
		Sleep(15);
	}
	printf("p(%d)=%d\n", (*pol).x, (*pol).value);
	return 0;
}