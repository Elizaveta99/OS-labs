#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

HANDLE hInEvent; 
CHAR lpEventName[] = "ReadyForWork"; 
HANDLE hSemaphore;

int main(int argv, char** args)
{
	ofstream fout(args[1], ios::out | ios::binary); // filename - char* no string ?? // 20 - set max size? 
													 // общий должен быть - чтобы из разных процессов

	// создание события для отправки Receiver сигнала на готовность к работе
	hInEvent = CreateEvent(NULL, FALSE, FALSE, lpEventName);
	if (hInEvent == NULL)
		return GetLastError();

	// ждем оповещение о наступлении события от процесса Receiver  ??
	DWORD dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
	if (dwWaitResult != WAIT_OBJECT_0)
		return dwWaitResult;

	// here close or later ??
	CloseHandle(hInEvent);









	cout << "Press 1 to write message or 2 to exit\n"; // in 1 string or 2 ??
	int type;
	cin >> type;
	if (type == 1)
	{
		cout << "Message : \n";
		string msg;
		cin >> msg;
		 // write into file in queue in the end (variable with the end) ??
	}
	else
	{
		fout.close();
		//cin.get(); ??
		return 0;
	}

	//cout << "Press any key to exit: ";
	
	//??
	cin.get();
	return 0;
}