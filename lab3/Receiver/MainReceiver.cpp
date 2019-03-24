#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
//#include "TaxPayment.h"

using namespace std;

HANDLE hInEvent;
CHAR lpEventName[] = "ReadyForWork";
HANDLE hSemaphore;

// queue - 3 senders

int main()
{
	cout << "Enter file name :\n";
	string filename;
	cin >> filename; // getline;

	//open file to read from it   ??
	ifstream fin(filename, ios::out | ios::binary); // filename - char* ?? // 20 - set max size? 
													 // общий должен быть - чтобы из разных процессов

	cout << "Sender's amount :"; // 3 always
	int sender_amount; // or global variable ?
	cin >> sender_amount;

	// ждать сразу всех sender? можем читать только когда все послали или по-одному ?



	// создаем семафор
	hSemaphore = CreateSemaphore(NULL, 0, 10, NULL);
	if (hSemaphore == NULL)
		return GetLastError();
	// создаем поток, который готовит элементы массива
	hThread = CreateThread(NULL, 0, thread, NULL, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();
	// поток main выводит элементы массива
	// только после их подготовки потоком thread
	cout << "A final state of the array: ";
	for (i = 0; i < 10; i++)
	{
		WaitForSingleObject(hSemaphore, INFINITE);
		cout << a[i] << ' ';
		cout.flush();
	}
	cout << endl;
	CloseHandle(hSemaphore);
	CloseHandle(hThread);






	for (int glob_i = 0; glob_i < sender_amount; glob_i++)
	{
		string lpszAppName = "Sender_" + to_string(glob_i + 1) + ".exe"; // name ??
		STARTUPINFO si;
		PROCESS_INFORMATION piSender;


		// или здесь не надо mutex

		//HANDLE hMutex;
		//int i, j;
		//// открываем мьютекс
		//hMutex = OpenMutex(SYNCHRONIZE, FALSE, "DemoMutex");
		//if (hMutex == NULL)
		//{
		//	cout << "Open mutex failed." << endl;
		//	cout << "Press any key to exit." << endl;
		//	cin.get();
		//	return GetLastError();
		//}

		//


		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);

		string 	param = lpszAppName + " " + filename;
		int destSize = param.size() + 1;
		char* lpszCommandLine = const_cast<char*>(param.c_str());
		if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piSender))
		{
			cout << "The new process is not created.\n";
			cout << "Check a name of the process.\n";
			cout << "Press any key to finish.\n";
			DWORD dw = GetLastError();
			return dw;
		}

		// point 4
		hInEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, lpEventName);
		if (hInEvent == NULL)
		{
			cout << "Open event failed." << endl;
			cout << "Input any char to exit." << endl;
			cin >> c;
			return GetLastError();
		}

		// след 3 - надо сейчас?

		// ждем завершения созданного процесса
		WaitForSingleObject(piSender.hProcess, INFINITE);

		// закрываем дескрипторы этого процесса в текущем процессе ??
		CloseHandle(piSender.hThread);
		CloseHandle(piSender.hProcess);
	}

	/*cout << "Records of the binary file :\n";
	int sz = atoi((char*)args[2]);
	while (sz--)
	{
		cin >> tax.num >> tax.name >> tax.sum;
		fout.write((char*)&tax.num, sizeof(tax.num));
		fout.write((char*)&tax.name, sizeof(tax.name));
		fout.write((char*)&tax.sum, sizeof(tax.sum));
	}*/

	fin.close();

	cout << "Receiver works\n";
	cin.get(); // ??

	return 0;
}