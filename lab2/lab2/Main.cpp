#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "TaxPayment.h" 

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	cout << "Binary file name and amount of entries in it: \n";
	string name;
	int amount;
	cin >> name >> amount;

	string lpszAppName = "Creator.exe"; 
	STARTUPINFO si;
	PROCESS_INFORMATION piCreator;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	// создаем новый консольный процесс
	string 	param = lpszAppName + " " + name + " " + to_string(amount);
	int destSize = param.size() + 1;
	char* lpszCommandLine = const_cast<char*>(param.c_str());
	if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCreator))
	{
		cout << "The new process is not created.\n";
		cout << "Check a name of the process.\n";
		cout <<  "Press any key to finish.\n";
		//DWORD dw = GetLastError(); //??
		//return dw;
	}
	cout << "The new process Creator.exe is created.\n";
	
	// ждем завершения созданного процесса
	WaitForSingleObject(piCreator.hProcess, INFINITE);
	
	// закрываем дескрипторы этого процесса в текущем процессе ??
	CloseHandle(piCreator.hThread);
	CloseHandle(piCreator.hProcess);

	param = "";
	lpszAppName = "";

	//вывод на консоль
	ifstream fin_bin(name, ios::in | ios::binary); 
	TaxPayment tax;
	for (int i = 0; i < amount; i++) 
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

	lpszAppName = "Reporter.exe"; 	
	PROCESS_INFORMATION piReporter;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	// создаем новый консольный процесс
	param = lpszAppName + " " + name + " " + nameReport + " " + to_string(paymentValue) + " " + type;
	destSize = param.size() + 1;
	char * lpszCommandLineReporter = const_cast<char*>(param.c_str());

	if (!CreateProcess(NULL, lpszCommandLineReporter, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piReporter))
	{
		cout << "The new process is not created.\n";
		cout << "Check a name of the process.\n";
		cout << "Press any key to finish.\n";
		//DWORD dw = GetLastError(); //??
		//return dw;
	}
	cout << "The new process Reporter.exe is created.\n";

	// ждем завершения созданного прцесса
	WaitForSingleObject(piReporter.hProcess, INFINITE);

	// закрываем дескрипторы этого процесса в текущем процессе 
	CloseHandle(piReporter.hThread);
	CloseHandle(piReporter.hProcess);

	//вывод отчёта на консоль
	ifstream fin_txt(nameReport, ios::in);  
	string temp_s;

	while (getline(fin_txt, temp_s))
	{
			cout << temp_s << "\n";
	}

	fin_bin.close();
	fin_txt.close();

	return 0;
}