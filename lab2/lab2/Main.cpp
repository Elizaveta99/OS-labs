#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "TaxPayment.h" 

using namespace std;

int main()
{
	//setlocale(LC_ALL, "Russian");

	cout << "Binary file name and amount of entries in it: \n";
	string name, param;
	int amount;
	cin >> name >> amount;
	param = " " + name + " " + to_string(amount); 

	string lpszAppName = "Creator.exe"; // change path
	STARTUPINFO si;
	PROCESS_INFORMATION piCreator;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	// создаем новый консольный процесс
	char *temp = const_cast<char*>(param.c_str());
	int destSize = sizeof(lpszAppName) + sizeof(temp) + 1; //??
	char* lpszCommandLine;
	lpszCommandLine = new char[destSize]; // change path ??
	lpszCommandLine = const_cast<char*>(lpszAppName.c_str());
	strcat_s(lpszCommandLine, destSize, temp); // destSize??

	cout << "Name bin file = \n" << name << "\n";
	cout << "cmd : \n" << lpszCommandLine << "\n";
	//lpszCommandLine = (char*)"Creator.exe new 3";

	//char* lpszAppNameTemp = const_cast<char*>(lpszAppName.c_str());
	if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCreator))
		             //NULL - тк есть ком строка
	{
		cout << "The new process is not created.\n";
		cout << "Check a name of the process.\n";
		cout <<  "Press any key to finish.\n";
		DWORD dw = GetLastError(); //??
		return dw;
	}
	cout << "The new process Creator.exe is created.\n";

	// ждем завершения созданного процесса
	WaitForSingleObject(piCreator.hProcess, INFINITE);

	// закрываем дескрипторы этого процесса в текущем процессе ??
	CloseHandle(piCreator.hThread);
	CloseHandle(piCreator.hProcess);

	param = "";
	lpszAppName = "";
	temp = nullptr;
	lpszCommandLine = nullptr;

	//вывод на консоль
	ifstream fin_bin(name, ios::in | ios::binary); // out?
	TaxPayment tax;
	for (int i = 0; i < amount; i++) //  или до разм
	{
		fin_bin.read((char*)&tax.num, sizeof(tax.num));
		fin_bin.read((char*)&tax.name, sizeof(tax.name));
		fin_bin.read((char*)&tax.sum, sizeof(tax.sum));
		cout << setprecision(5) << fixed << tax.num << ' ' << tax.name << ' ' << tax.sum << "\n";
	}


	string nameReport, type; 
	double paymentValue;
	cout << "Report file name, payment values, report type ( < or > )\n";
	cin >> nameReport >> paymentValue >> type;
	param = " " + name + " " + nameReport + " " + to_string(paymentValue) + " " + type;
				cout << "Param = :\n"; 
				cout << param << "\n";

	lpszAppName = "Reporter.exe"; // change path	
	PROCESS_INFORMATION piReporter;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	// создаем новый консольный процесс
	temp = const_cast<char*>(param.c_str()); // ?? or new?
	destSize = sizeof(lpszAppName) + sizeof(temp) + 1;
	lpszCommandLine = new char[destSize]; // change path ??
	lpszCommandLine = const_cast<char*>(lpszAppName.c_str());
	
	strcat_s(lpszCommandLine, destSize, temp); 

					cout << "cmd1 : \n" << lpszCommandLine << "\n";
	

	if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piReporter))
					 //NULL - тк есть ком строка
	{
		cout << "The new process is not created.\n";
		cout << "Check a name of the process.\n";
		cout << "Press any key to finish.\n";
		DWORD dw = GetLastError(); //??
		return dw;
	}
	cout << "The new process Reporter.exe is created.\n";

	// ждем завершения созданного прцесса
	WaitForSingleObject(piReporter.hProcess, INFINITE);

	// закрываем дескрипторы этого процесса в текущем процессе 
	CloseHandle(piReporter.hThread);
	CloseHandle(piReporter.hProcess);

	//вывод отчёта на консоль
	cout << "NameReport = \n" << nameReport << "\n";

	system("pause");
	return 0;

	string pathToReport = "";
	ifstream fin_txt(nameReport); // другой путь - там, где создался  ?
	string temp_s;
	while (fin_txt >> temp_s)
		cout << temp_s << "\n";

	return 0;
}