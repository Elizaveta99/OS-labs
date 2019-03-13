#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "C:\Users\User\Desktop\1studing\OS\labs\OS-labs\lab2\Creator\TaxPayment.h" 

using namespace std;

int main()
{
	cout << "Binary file name and amount of entries in it: \n";
	string name, param;
	int amount;
	cin >> name >> amount;
	param = name + " " + to_string(amount); //??
	//getline(cin, param);

	string lpszAppName = "C:\\Creator.exe"; // change path
	STARTUPINFO si;
	PROCESS_INFORMATION piCreator;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	// ������� ����� ���������� �������
	//char lpszCommandLine[] = "C:\\01-1-ConsoleProcess.exe p1 p2 p3";
	char *temp = const_cast<char*>(param.c_str());
	char lpszCommandLine[] = "C:\\Creator.exe "; // change path ??
	int destSize = sizeof(lpszCommandLine) + sizeof(temp) + 1;
	strcat_s(lpszCommandLine, destSize, temp); //??

	//read in while (�� ����� ����� ������)
	// ��� ��������� ������ �� ���������� return 0, ����� ������ ����� getlastterror?
	
	//char* lpszAppNameTemp = const_cast<char*>(lpszAppName.c_str());
	// CREATE_NEW_CONSOLE or NULL ??
	if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCreator))
		             //NULL - �� ���� ��� ������
	{
		cout << "The new process is not created.\n";
		cout << "Check a name of the process.\n";
		cout <<  "Press any key to finish.\n";
		DWORD dw = GetLastError(); //??
		return dw;
	}
	//cout << "The new process is created.\n";

	// ���� ���������� ���������� �������
	WaitForSingleObject(piCreator.hProcess, INFINITE);

	param = "";
	delete[] temp; // ?? free(temp);
	delete[] lpszCommandLine; // ?? free(lpszCommandLine);

	// ��������� ����������� ����� �������� � ������� �������� ??
	CloseHandle(piCreator.hThread);
	CloseHandle(piCreator.hProcess);

	//����� �� �������
	ifstream fin(name, ios::in | ios::binary); // out?
	/*int num;
	string nameTax;
	double sum;*/
	TaxPayment tax;
	for (int i = 0; i < amount; i++) //  ��� �� ����
	{
		//output.read(); //??
		/*fin >> num >> nameTax >> sum;
		cout << num << ' ' << nameTax << ' ' << sum << "\n";*/
		fin >> tax.num >> tax.name >> tax.sum;
		cout << tax.num << ' ' << tax.name << ' ' << tax.sum << "\n";
	}

	string nameReport, type; // 1 symbol in string - ok?
	double paymentValue;
	//char type;
	cout << "Report file name, payment values, report type ( < or > )\n";
	cin >> nameReport >> paymentValue >> type;
	param = name + " " + nameReport + " " + to_string(paymentValue) + " " + type;
	//lpszAppName = "C:\\Reporter.exe"; // change path //?? or string or new name
	/*STARTUPINFO si;*/
	PROCESS_INFORMATION piReporter;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	// ������� ����� ���������� �������
	//char lpszCommandLine[] = "C:\\01-1-ConsoleProcess.exe p1 p2 p3";
	temp = const_cast<char*>(param.c_str()); // ?? or new?
	char lpszCommandLine[] = "C:\\Reporter.exe "; // change path ??
	destSize = sizeof(lpszCommandLine) + sizeof(temp) + 1; //??
	strcat_s(lpszCommandLine, sizeof(temp), temp); //??

	//read in while (�� ����� ����� ������)
	// ��� ��������� ������ �� ���������� return 0, ����� ������ ����� getlastterror?

	//lpszAppNameTemp = const_cast<char*>(param.c_str());
	// CREATE_NEW_CONSOLE or NULL ??
	if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piReporter))
					 //NULL - �� ���� ��� ������
	{
		cout << "The new process is not created.\n";
		cout << "Check a name of the process.\n";
		cout << "Press any key to finish.\n";
		DWORD dw = GetLastError(); //??
		return dw;
	}
	//cout << "The new process is created.\n";

	// ���� ���������� ���������� �������
	WaitForSingleObject(piReporter.hProcess, INFINITE);

	// ��������� ����������� ����� �������� � ������� �������� ??
	CloseHandle(piReporter.hThread);
	CloseHandle(piReporter.hProcess);

	//����� ������ �� �������

	return 0;
}