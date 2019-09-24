#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "TaxPayment.h"

using namespace std;

int main(int argc, char** args) 
{
	setlocale(LC_ALL, "Russian");

	ofstream fout((char*)args[2]);
	size_t offset = 0;
	double value = stod((char*)args[3], &offset);
	ifstream fin((char*)args[1], ios::binary);
	TaxPayment tax;

	fout << "Отчёт по файлу \"" << args[1] << "\"\n";
	fout << "Список компаний, налоговые платежи которых " << args[4] << " " << setprecision(5) << fixed << args[3] << "\n";

	bool flag = false;
	while (true)
	{
		if (!fin.read((char*)&tax.num, sizeof(tax.num))) 
			break;
		fin.read((char*)&tax.name, sizeof(tax.name));
		fin.read((char*)&tax.sum, sizeof(tax.sum));
		if (flag)
			fout << "\n";
		else
			flag = true;
		if (strcmp(args[4], "<") == 0)
		{
			if (tax.sum < value)
				fout << tax.num << ' ' << tax.name << ' ' << tax.sum; 
		}
		else
		{
			if (tax.sum > value)
				fout << tax.num << ' ' << tax.name << ' ' << tax.sum;
		}
	}

	fin.close();
	fout.close(); 

	cout << "Reporter works\n";
	system("pause");

	return 0;
}