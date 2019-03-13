#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "TaxPayment.h"

using namespace std;

int main(int argc, char** args) 
{
	setlocale(LC_ALL, "Russian");

	cout << "Отчёт по файлу \" " << args[1] << "\"\n";
	cout << "Список компаний, налоговые платежи которых " << args[4] << " " << args[3] << "\n";

	ofstream fout((char*)args[2]);
	size_t offset = 0;
	double value = stod((char*)args[3], &offset);
	ifstream fin((char*)args[1], ios::binary);
	TaxPayment tax;

	cout << "val = \n" << value << "\n";
	/*system("pause");
	return 0;*/

	while (true)
	{
		cout << "????????\n";
		//fin.read((char*)&tax, sizeof(tax)); ??
		if (!fin.read((char*)&tax.num, sizeof(tax.num)))
			break;
		//fin.read((char*)&tax.num, sizeof(tax.num));
		fin.read((char*)&tax.name, sizeof(tax.name));
		fin.read((char*)&tax.sum, sizeof(tax.sum));

		

		if (strcmp(args[4], "<") == 0)
		{
//printf(args[4], "\n");
			if (tax.sum < value)
				fout << tax.num << ' ' << tax.name << ' ' << tax.sum << "\n"; // fout.write() ??
		}
		else
		{
			if (tax.sum > value)
				fout << tax.num << ' ' << tax.name << ' ' << tax.sum << "\n";
		}
	}

	fin.close();
	fout.close(); 


//	system("pause");

	return 0;
}