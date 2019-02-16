#include <windows.h>
#include <iostream>
#include "Polynom.h"

using namespace std;

int main()
{
	Polynom polynomNum;
	Polynom polynomDen;
	cout << "Enter degree of polynom in numerator" << endl;
	cin >> polynomNum.degree;
	cout << "Enter coefficients of polynom in numerator" << endl;
	for (int i = 0; i <= polynomNum.degree; i++)
		cin >> polynomNum.coef[i];
	cout << "Enter degree of polynom in denominator" << endl;
	cin >> polynomDen.degree;
	cout << "Enter coefficients of polynom in denominator" << endl;
	for (int i = 0; i <= polynomDen.degree; i++)
		cin >> polynomDen.coef[i];
	cout << "Enter value of variable x" << endl;
	cin >> polynomNum.x;
	polynomDen.x = polynomNum.x;
	polynomNum.value = 0;
	polynomDen.value = 0;
	
	HANDLE	hThreadFirst;
	HANDLE	hThreadSecond;
	DWORD	IDThreadFirst;
	DWORD	IDThreadSecond;
	hThreadFirst = CreateThread(NULL, 0, CalculatePolynom, &polynomNum, 0, &IDThreadFirst);  //???
	hThreadSecond = CreateThread(NULL, 0, CalculatePolynom, &polynomDen, 0, &IDThreadSecond);  //???
	
	// ждем, пока потоки закончат работу
	WaitForSingleObject(hThreadFirst, INFINITE);
	WaitForSingleObject(hThreadSecond, INFINITE);

	printf("f(%d)=%d/%d\n", polynomNum.x, polynomNum.value, polynomDen.value);
	// закрываем дескрипторы потоков
	CloseHandle(hThreadFirst);
	CloseHandle(hThreadSecond);
	return 0;
}