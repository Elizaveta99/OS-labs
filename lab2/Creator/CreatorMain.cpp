#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "TaxPayment.h"

using namespace std;

int main(int argc, char* args) // ?? argc - always?
{
	ofstream fout((char*)args[1], ios::out | ios::binary); //??[1]
	TaxPayment tax;

	cout << "Records of the binary file :\n";
	int sz = (int)args[2];
	while (sz--) 
	{
		cin >> tax.name >> tax.num >> tax.sum;
		fout.write((char*)&tax, sizeof(tax)); //??
	}

	fout.close(); //?
	return 0;
}