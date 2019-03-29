#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

HANDLE hInEvent, hWorkEvent;
CHAR lpEventName[] = "ReadyForWork"; 
CHAR lpWorkName[] = "CanWork";
HANDLE hSemaphore;

int FILE_SIZE = 63;
int MSG_AMOUNT = 3;

int main(int argv, char** args)
{
	cout << "Sender !\n";
	fstream file;

	//fstream file(args[1], ios::in | ios::out | ios::binary); 

	SetEvent(hInEvent); // ������� ������ � ����� ����������
	Sleep(500); //??

	//DWORD dwWaitResult = WaitForSingleObject(hWorkEvent, INFINITE); // ��� ���������� �� ����������� ������, ����� ��� Senders ������

//system("pause");
//	return 0;

	/*if (dwWaitResult != WAIT_OBJECT_0) 
	{
		cout << dwWaitResult << endl;
		system("pause");
		return dwWaitResult;
	}*/

	HANDLE hMutex = OpenMutex(SYNCHRONIZE, FALSE, "FileMutex");
	if (hMutex == NULL)
	{
		cout << "Open mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}

	HANDLE hSemaphoreReceive = OpenSemaphore(SEMAPHORE_MODIFY_STATE, FALSE, "SemaphoreReceive");
	HANDLE hSemaphoreSend = OpenSemaphore(SYNCHRONIZE, FALSE, "SemaphoreSend");
	
	cout << "Press 1 to send message or 2 to exit\n"; 
	while (true)
	{
		int type;
		cin >> type;
		if (type == 1)
		{
			cout << "Message : \n";
			string msg;
			cin >> msg;
			char* mesg = const_cast<char*>(msg.c_str()); // ?? ���� ��?

			WaitForSingleObject(hSemaphoreSend, INFINITE);
			WaitForSingleObject(hMutex, INFINITE);

			file.open(args[1], ios::binary | ios::in | ios::out);

			int end_pos = -1, beg_pos = -1, amount = 0;
			file.read((char*)&amount, sizeof(int));
			file.read((char*)&beg_pos, sizeof(int));
			file.read((char*)&end_pos, sizeof(int));

			
				if (amount == MSG_AMOUNT) // ���� ����������
				{
					WaitForSingleObject(hSemaphoreReceive, INFINITE);
					// wait ??
					// end_pos �������� 
				}
				else if (end_pos >= FILE_SIZE * sizeof(int))// � ����� �������� ������, �� ��� ����� ����, �����������
				//{
					end_pos = 3 * sizeof(int); // ��������� ����� � ������ ? 4 * sizeof ??
				//}
			
			
			file.seekp(end_pos + amount * sizeof(mesg), ios::beg); // +/- 1 ?? �������� ������� ������� ����� �������
			file.write((char*)&mesg, sizeof(mesg)); // �������� ���������

			file.seekp(0, ios::beg);
			amount++;
			file.write((char*)&amount, sizeof(int));

			end_pos += amount * sizeof(mesg); // ����������� ����� ������� ����� �������
			file.seekp(sizeof(int), ios::beg); // �� 1 ����� �������� �������� ����� �������
			file.write((char*)&end_pos, sizeof(int)); // �������� ����� �������� ����� �������

			ReleaseMutex(hMutex);
			ReleaseSemaphore(hSemaphoreReceive, 1, NULL); // ++ read
			Sleep(500); // ??
		}
		else
			break;
	}

	CloseHandle(hMutex);
	CloseHandle(hSemaphoreReceive);
	CloseHandle(hSemaphoreSend);
	file.close();

	cout << "Press any key to exit: ";
	system("pause"); // �������� �� get?

	return 0;
}