#include <windows.h>
#include <iostream>
#include "Polynom.h"

using namespace std;

//volatile int n;

DWORD WINAPI CalculatePolynom(LPVOID iNum)
{
	cout << "Thread is started." << endl;
	n = n + (int)iNum;
	cout << "Thread is finished." << endl;

	return 0;
}

int main()
{
	Polynom polynom;
	//int degreeNum,
		//degreeDen;
	cout << "Enter degree of polynom in numerator\n";
	cin >> polynom.degreeNum;
	cout << "Enter coefficients of polynom in numerator\n";
	for (int i = 0; i < polynom.degreeNum; i++)
		cin >> polynom.coefNum[i];
	cout << "Enter degree of polynom in denominator\n";
	cin >> polynom.degreeDen;
	cout << "Enter coefficients of polynom in denominator\n";
	for (int i = 0; i < polynom.degreeDen; i++)
		cin >> polynom.coefDen[i];

	int	inc = 10;
	HANDLE	hThread;
	DWORD	IDThread;

	cout << "n = " << n << endl;

	hThread = CreateThread(NULL, 0, Polynom, (void*)inc, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();

	// ждем пока поток Add закончит работу
	WaitForSingleObject(hThread, INFINITE);
	// закрываем дескриптор потока
	CloseHandle(hThread);

	cout << "n = " << n << endl;

	return 0;
}