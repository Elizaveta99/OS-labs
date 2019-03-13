#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "TaxPayment.h"

using namespace std;

int main(int argc, char** args) 
{
	//cout << "FileName :\n";
	//printf("%s\n", (char*)args[1]);
	ofstream fout((char*)args[1], ios::out | ios::binary); // ios::out ??
	TaxPayment tax;

	cout << "Records of the binary file :\n";
	int sz = atoi((char*)args[2]);
	//cout << "sz = \n" << sz << endl;
	while (sz--) 
	{
		cin >> tax.num >> tax.name >> tax.sum;
		//fout.write((char*)&tax, sizeof(tax)); does not work
		fout.write((char*)&tax.num, sizeof(tax.num));
		fout.write((char*)&tax.name, sizeof(tax.name));
		fout.write((char*)&tax.sum, sizeof(tax.sum));
	}

	fout.close(); 

	//cout << "Creator is done!\n";
	//system("pause");

	return 0;
}