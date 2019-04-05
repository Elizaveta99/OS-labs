#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main()
{
	cout << "Queue size : ";
	int size;
	cin >> size;
	cout << "Amount Producer's threads : ";
	int producer_threads;
	cin >> producer_threads;
	cout << "Amount Consumer's threads : ";
	int consumer_threads;
	cin >> consumer_threads;

	// create queue
	// create pr cons theads amount
	// wait signal from all? pr coms
	// send signal to start all pr cons
	//

	cout << "Sender's amount : ";
	int sender_amount;
	cin >> sender_amount;

	HANDLE wait_array[MSG_AMOUNT];
	PROCESS_INFORMATION	sender_array[MSG_AMOUNT];

	HANDLE hMutex = CreateMutex(NULL, FALSE, "FileMutex");
	if (hMutex == NULL)
	{
		cout << "Create mutex failed." << endl;
		return GetLastError();
	}

	HANDLE hSemaphoreReceive = CreateSemaphore(NULL, 0, MSG_AMOUNT, "SemaphoreReceive");
	if (hSemaphoreReceive == NULL)
		return GetLastError();
	HANDLE hSemaphoreSend = CreateSemaphore(NULL, MSG_AMOUNT, MSG_AMOUNT, "SemaphoreSend");
	if (hSemaphoreSend == NULL)
		return GetLastError();

	int event_amount = 0;
	for (int glob_i = 0; glob_i < sender_amount; glob_i++)
	{
		string lpszAppName = "Sender.exe";
		STARTUPINFO si;
		PROCESS_INFORMATION piSender;
		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		string 	param = lpszAppName + " " + filename + " " + lpEventName + to_string(glob_i);
		string name1 = lpEventName + to_string(glob_i);
		char* name = const_cast<char*>(name1.c_str());

		int destSize = param.size() + 1;
		char* lpszCommandLine = const_cast<char*>(param.c_str());

		// создание события для отправки Receiver сигнала на готовность к работе
		hInEvent = CreateEvent(NULL, FALSE, FALSE, name);
		if (hInEvent == NULL)
			return GetLastError();

		if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piSender))
		{
			cout << "The new process is not created.\n";
			cout << "Check a name of the process.\n";
			cout << "Press any key to finish.\n";
			DWORD dw = GetLastError();
			return dw;
		}

		wait_array[event_amount] = hInEvent;
		sender_array[event_amount++] = piSender;
	}

	if (WaitForMultipleObjects(sender_amount, wait_array, TRUE, INFINITE) == WAIT_FAILED)
	{
		cout << "Wait for multiple objects failed." << endl;
		cout << "Press any key to exit ";
		cin.ignore();
		cin.get();
		return 0;
	}

	// читаем сообщения
	cout << "Press 1 to receive message or 2 to exit\n";
	while (true)
	{
		int type;
		cin >> type;
		if (type == 1)
		{
			char mesg[MSG_SIZE];

			WaitForSingleObject(hSemaphoreReceive, INFINITE);
			WaitForSingleObject(hMutex, INFINITE);

			fstream file(filename, ios::out | ios::in | ios::binary);

			int end_pos = -1, beg_pos = -1, amount = 0;
			file.read((char*)&amount, sizeof(int));
			file.read((char*)&beg_pos, sizeof(int));
			file.read((char*)&end_pos, sizeof(int));

			if (amount == 0) // файл пустой
			{
				WaitForSingleObject(hSemaphoreSend, INFINITE);
			}
			else if (beg_pos >= FILE_SIZE + 3 * sizeof(int))
				beg_pos = 3 * sizeof(int);


			file.seekg(beg_pos, ios::beg); // установила текущую позицию начала очереди
			file.read((char*)&mesg, MSG_SIZE); // прочитала сообщение 

			cout << "Message : " << mesg;

			file.seekp(0, ios::beg);
			amount--;
			file.write((char*)&amount, sizeof(int));

			beg_pos += MSG_SIZE;						// пересчитала новую позицию начала очереди
			file.seekp(1 * sizeof(int), 0);				// на 1 месте хранится значение начала очереди
			file.write((char*)&beg_pos, sizeof(int));	// записала новое значение начала очереди

			file.close();
			// освобождаем мьютекс
			ReleaseMutex(hMutex);
			// ++ произвели единицу ресурса


	cout << "Main works\n";
	cout << "Press any key to exit ";
	cin.ignore();
	cin.get();

	return 0;
}