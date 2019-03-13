#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "C:\Users\User\Desktop\1studing\OS\labs\OS-labs\lab2\Creator\TaxPayment.h"

using namespace std;

int main(int argc, char* args) // ?? argc - always?
{
	int* tempArr;
	tempArr = new int[]; //??how
	double value = stod(args[3], );

	ifstream fin((char*)args[1], ios::binary);
	TaxPayment tax;
	while (fin)
	{
		fin.read((char*)&tax.num, sizeof(tax.num));
		fin.read((char*)&tax.name, sizeof(tax.name));
		fin.read((char*)&tax.sum, sizeof(tax.sum));

		if ()
	}
	

	//text file
	ofstream fout((char*)args[2]); //??[1] - if without argc?

	int sz = (int)args[2];
	while (sz--)
	{
		cin >> tax.name >> tax.num >> tax.sum;
		fout.write((char*)&tax, sizeof(tax)); //??
	}

	fout.close(); //?
	return 0;
}