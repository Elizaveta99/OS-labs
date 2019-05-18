#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <string>
#include "message.h"

using namespace std;

int main(int args, char** argv)
{
	cout << "Client " << argv[1] << "\n";
	cout << "Press 1 to write, 2 to read, 3 to exit\n";

	while (1) 
	{
		cout << ">> ";
		int type;
		cin >> type;
		struct message Message;
		HANDLE hReadPipe = (HANDLE)atoi(argv[2]); 
		HANDLE hWritePipe = (HANDLE)atoi(argv[3]);

		if (type == 1)
		{
			cout << "Name of process-receiver : ";
			cin.ignore();
			cin.getline(Message.receiver, sizeof(Message.receiver));
			cout << "Message : ";
			cin.getline(Message.text, sizeof(Message.text));
			strcpy(Message.sender, argv[1]);

			// пишем сообщение в анонимный канал
			DWORD dwBytesWritten;
			if (!WriteFile(hWritePipe, &Message, sizeof(Message), &dwBytesWritten, NULL))
			{
				cout << "Write to file failed.\n";
				cout << "Press any key to finish.\n";
				cin.ignore();
				cin.get();
				return GetLastError();
			}
			cout << "The process finished writing to the pipe.\n";
		}
		else
		if (type == 2)
		{
			// читаем из анонимного канала
			struct message data;
			DWORD dwBytesRead;
			if (!ReadFile(hReadPipe, &data, sizeof(data), &dwBytesRead, NULL))
			{
				cout << "Read from the pipe failed.\n";
				cout << "Press any key to finish.\n";
				cin.ignore();
				cin.get();
				return GetLastError();
			}
			else {
				cout << "-sender name: " << data.sender << "\n";
				cout << "-" << data.text << "\n";
			}
			cout << "The process finished reading from the pipe.\n";
		}
		else
		{
			CloseHandle(hReadPipe);
			CloseHandle(hWritePipe);
			break;
		}
	}

	return 0;
}