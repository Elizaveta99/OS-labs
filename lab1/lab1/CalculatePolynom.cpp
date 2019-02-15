#include <windows.h>
#include <iostream>
#include "Polynom.h"

using namespace std;

DWORD WINAPI CalculatePolynom(LPVOID polynom)
{
	cout << "Thread is started." << endl;
	Polynom* pol = (Polynom*)polynom;
	int xDegree = 1; 

	for (int i = (*pol).degree; i > -1 ; i--) // ?? int ?? order?
	{
		(*pol).value += (*pol).coef[i] * xDegree;
		xDegree *= (*pol).x;
		Sleep(15);
	}
	cout << "p(" << (*pol).x << ")=" << (*pol).value << endl;

	return 0;
}