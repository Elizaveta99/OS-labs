#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

HANDLE hInEvent, hWorkEvent;
CHAR lpEventName[] = "ReadyForWork";
HANDLE hSemaphore;

int main(int args, char** argv)
{
	cout << "Amount and value : ";
	int amount, value;
	cin >> amount >> value;

	// открытие события для отправки Main сигнала на готовность к работе
	hInEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, argv[2]);
	if (hInEvent == NULL)
	{
		cout << "Open event failed." << endl;
		cout << "Input any char to exit." << endl;
		char c;
		cin >> c;
		return GetLastError();
	}

	SetEvent(hInEvent); // послали сигнал о своей готовности
	WaitForSingleObject(hWorkEvent, INFINITE);
	cout << "Priducer \n";
	CloseHandle(hInEvent);

	for (int i = 0; i < amount; i++)
		queue.insert(value);
	

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
			//string msg;
			char mesg[MSG_SIZE];
			cin >> mesg;

			WaitForSingleObject(hSemaphoreSend, INFINITE);
			WaitForSingleObject(hMutex, INFINITE);

			//cout << mesg << endl;

			file.open(argv[1], ios::binary | ios::in | ios::out);

			int end_pos = -1, beg_pos = -1, amount = -1;
			file.read((char*)&amount, sizeof(int));
			file.read((char*)&beg_pos, sizeof(int));
			file.read((char*)&end_pos, sizeof(int));

			//cout << "read : " << amount << ' ' << beg_pos << ' ' << end_pos << "\n";

			if (amount == MSG_AMOUNT) // файл переполнен
			{
				WaitForSingleObject(hSemaphoreReceive, INFINITE);
			}
			else if (end_pos >= FILE_SIZE + 3 * sizeof(int))
				end_pos = 3 * sizeof(int);

			file.seekp(end_pos, ios::beg);
			//file.seekp(end_pos + amount * sizeof(mesg), ios::beg); // +/- 1 ?? получила текущую позицию конца очереди
			file.write((char*)&mesg, MSG_SIZE); // записала сообщение

			file.seekp(0, ios::beg);
			amount++;
			file.write((char*)&amount, sizeof(int));

			end_pos += MSG_SIZE; // пересчитала новую позицию конца очереди
			file.seekp(2 * sizeof(int), ios::beg); // на 2 месте хранится значение конца очереди
			file.write((char*)&end_pos, sizeof(int)); // записала новое значение конца очереди

			file.close();
			ReleaseMutex(hMutex);
			ReleaseSemaphore(hSemaphoreReceive, 1, NULL); // ++ read			
		}
		else
			break;
		cout << "Press type : \n";
	}

	file.close();

	cout << "Press any key to exit ";
	cin.ignore();
	cin.get();

	return 0;
}