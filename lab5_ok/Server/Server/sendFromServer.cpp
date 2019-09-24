#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <string>
#include "message.h"
#include "Pipe.h"

using namespace std;

DWORD WINAPI sendFromServer(struct Pipe pipe)
{
	struct message data;
	DWORD dwBytesRead;
	while (1)
	{
		if (!ReadFile(pipe.handleReadPipe, &data, sizeof(data), &dwBytesRead, NULL))
		{
			cout << "Read from the pipe failed.\n";
			cout << "Press any key to finish.\n";
			cin.ignore();
			cin.get();
			return GetLastError();
		}
		else
		{
			int numb = -1;
			for (int j = 0; j < pipe.amount; j++) 
				if (strcmp(const_cast<char*>(pipe.names[j].c_str()), data.receiver) == 0)
				{
					numb = j;
					break;
				}
			DWORD dwBytesWritten;
			if (!WriteFile(pipe.handlesWritePipe[numb], &data, sizeof(data), &dwBytesWritten, NULL))
			{
				cout << "Write to file failed.\n";
				cout << "Press any key to finish.\n";
				cin.ignore();
				cin.get();
				return GetLastError();
			}
		}
	}

	return 0;
}