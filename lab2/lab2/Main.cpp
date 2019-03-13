#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "TaxPayment.h" 

using namespace std;

int main()
{
	cout << "Binary file name and amount of entries in it: \n";
	string name, param;
	int amount;
	cin >> name >> amount;
	param = name + " " + to_string(amount); //??
	//getline(cin, param);

	string lpszAppName = "Creator.exe"; // change path
	STARTUPINFO si;
	PROCESS_INFORMATION piCreator;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	// ������� ����� ���������� �������
	char *temp = const_cast<char*>(param.c_str());
	char *lpszCommandLine = (char*)"Creator.exe "; // change path ??
	int destSize = sizeof(lpszCommandLine) + sizeof(temp) + 1; //??
	strcat_s(lpszCommandLine, destSize, temp); // destSize??

	//read in while (�� ����� ����� ������)
	// ��� ��������� ������ �� ���������� return 0, ����� ������ ����� getlastterror?
	
	//char* lpszAppNameTemp = const_cast<char*>(lpszAppName.c_str());
	if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCreator))
		             //NULL - �� ���� ��� ������
	{
		cout << "The new process is not created.\n";
		cout << "Check a name of the process.\n";
		cout <<  "Press any key to finish.\n";
		DWORD dw = GetLastError(); //??
		return dw;
	}
	cout << "The new process Creator is created.\n";

	// ���� ���������� ���������� ��������
	WaitForSingleObject(piCreator.hProcess, INFINITE);

	param = "";
	delete[] temp; // ?? free(temp);
	delete[] lpszCommandLine; // ?? free(lpszCommandLine);

	// ��������� ����������� ����� �������� � ������� �������� ??
	CloseHandle(piCreator.hThread);
	CloseHandle(piCreator.hProcess);

	//����� �� �������
	ifstream fin_bin(name, ios::in | ios::binary); // out?
	/*int num;
	string nameTax;
	double sum;*/
	TaxPayment tax;
	for (int i = 0; i < amount; i++) //  ��� �� ����
	{
		//fin >> tax.num >> tax.name >> tax.sum;
		/*fin.read((char*)&tax.num, sizeof(tax.num));
		fin.read((char*)&tax.name, sizeof(tax.name));
		fin.read((char*)&tax.sum, sizeof(tax.sum));*/
		fin_bin.read((char*)&tax, sizeof(tax));
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
	lpszCommandLine = (char*)"Reporter.exe "; // change path ??
	destSize = sizeof(lpszCommandLine) + sizeof(temp) + 1; //??
	strcat_s(lpszCommandLine, sizeof(temp), temp); //??

	if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piReporter))
					 //NULL - �� ���� ��� ������
	{
		cout << "The new process is not created.\n";
		cout << "Check a name of the process.\n";
		cout << "Press any key to finish.\n";
		DWORD dw = GetLastError(); //??
		return dw;
	}
	cout << "The new process Reporter is created.\n";

	// ���� ���������� ���������� �������
	WaitForSingleObject(piReporter.hProcess, INFINITE);

	// ��������� ����������� ����� �������� � ������� �������� 
	CloseHandle(piReporter.hThread);
	CloseHandle(piReporter.hProcess);

	//����� ������ �� �������
	ifstream fin_txt(nameReport);
	string temp_s;
	while (fin_txt >> temp_s)
		cout << temp_s << "\n";

	return 0;
}