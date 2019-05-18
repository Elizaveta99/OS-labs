#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <string>
#include "message.h"
#include "Pipe.h"

using namespace std;

DWORD WINAPI sendFromServer(struct Pipe pipe);

int main(int args, char** argv)
{
	int amount;
	cout << "Amount of clients: ";
	cin >> amount;

	HANDLE *hThreadClient;
	DWORD *IDThreadClient;
	HANDLE *hWritePipe, *hReadPipe;
	struct Pipe *pipes;
	string *names;

	hThreadClient = new HANDLE[amount];
	IDThreadClient = new DWORD[amount];
	hWritePipe = new HANDLE[amount];
	hReadPipe = new HANDLE[amount];
	pipes = new struct Pipe[amount];
	names = new string[amount];

	for (int i = 0; i < amount; i++)
	{
		//cin.ignore();
		cout << "Name of Client #" << i + 1 << ": ";
		cin >> names[i];
		// создание анонимного канала
		SECURITY_ATTRIBUTES sa;
		// устанавливаем атрибуты защиты канала
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;					// защита по умолчанию
		sa.bInheritHandle = TRUE;						// дескрипторы наследуемые
		// создаём анонимный канал
		if (!CreatePipe(
			&hReadPipe[i], // дескриптор для чтения
			&hWritePipe[i], // дескриптор для записи
			&sa, // атрибуты защиты по умолчанию,
			// дескрипторы наследуемые
			0)) // размер буфера по умолчанию
		{
			cout << "Create pipe failed.\n";
			cout << "Press any key to finish.\n";
			cin.ignore();
			cin.get();
			return GetLastError();
		}
		// устанавливаем атрибуты нового процесса
		string lpszAppName = "Client.exe";
		STARTUPINFO si;
		PROCESS_INFORMATION piClient;
		ZeroMemory(&si, sizeof(STARTUPINFO)); 
		si.cb = sizeof(STARTUPINFO);
		// формируем командную строку
		string param = lpszAppName + " " + names[i] + " " + to_string((int)hReadPipe[i]) + " " + to_string((int)hWritePipe[i]);
		char *lpszComLine = const_cast<char*>(param.c_str());
		// запускаем новый консольный процесс
		if (!CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE,	CREATE_NEW_CONSOLE,	NULL, NULL, &si, &piClient))
		{
			cout << "Create process failed.\n";
			cout << "Press any key to finish.\n";
			cin.ignore();
			cin.get();
			return GetLastError();
		}
		// закрываем дескрипторы нового процесса                      
		CloseHandle(piClient.hProcess);
		CloseHandle(piClient.hThread);
	}

	for (int i = 0; i < amount; i++)
	{
		pipes[i].amount = amount;
		pipes[i].names = names;
		pipes[i].handlesWritePipe = hWritePipe;
		pipes[i].handleReadPipe = hReadPipe[i];
	}
	for (int i = 0; i < amount; i++)
	{
		hThreadClient[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendFromServer, &pipes[i], 0, &IDThreadClient[i]);
	}
	for (int i = 0; i < amount; i++)
	{
		WaitForSingleObject(hThreadClient[i], INFINITE);
		CloseHandle(hThreadClient[i]);
	}

	cout << "Press any key to exit ";
	cin.ignore();
	cin.get();

	return 0;
}