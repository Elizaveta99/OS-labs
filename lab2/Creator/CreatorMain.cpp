#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "TaxPayment.h"

using namespace std;

int main(int argc, char* args) 
{
	ofstream fout((char*)args[1], ios::out | ios::binary); // ios::out ??
	TaxPayment tax;

	cout << "Records of the binary file :\n";
	int sz = (int)args[2];
	while (sz--) 
	{
		cin >> tax.name >> tax.num >> tax.sum;
		fout.write((char*)&tax, sizeof(tax)); //?? or 3 fout ?
	}

	fout.close(); 

	return 0;
}