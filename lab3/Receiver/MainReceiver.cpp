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

int FILE_SIZE = 60;
const int MSG_AMOUNT = 3;

int main()
{
	cout << "Enter file name :\n";
	string filename;
	cin >> filename; 

	fstream file(filename, ios::out | ios::in | ios::binary);

	file.seekp(0, ios::beg); // сдвиг на 0 с начала
	int beg_pos = 0, end_pos = 1 * sizeof(int), amount = 0; // pos 0 и 1 для указателей начала и конца очереди, amount - кол-во сообщений
	file.write((char*)&amount, sizeof(int));
	file.write((char*)&beg_pos, sizeof(int));
	file.write((char*)&end_pos, sizeof(int));

	cout << "Sender's amount : "; 
	int sender_amount; 
	cin >> sender_amount;

	HANDLE wait_array[MSG_AMOUNT];
	PROCESS_INFORMATION	sender_array[MSG_AMOUNT];

	HANDLE hMutex = CreateMutex(NULL, FALSE, "FileMutex"); 
	if (hMutex == NULL)
	{
		cout << "Create mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		system("pause");
		return GetLastError();
	}

	//  параметры?
	HANDLE hSemaphoreReceive = CreateSemaphore(NULL, 0, MSG_AMOUNT, "SemaphoreReceive");
	if (hSemaphoreReceive == NULL)
		return GetLastError();
	HANDLE hSemaphoreSend = CreateSemaphore(NULL, 0, MSG_AMOUNT, "SemaphoreSend");
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
		string 	param = lpszAppName + " " + filename;
		int destSize = param.size() + 1;
		char* lpszCommandLine = const_cast<char*>(param.c_str());

		//cout << "cmd = " << param << ' ' << lpszCommandLine << "\n";

		if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piSender))
		{
			cout << "The new process is not created.\n";
			cout << "Check a name of the process.\n";
			cout << "Press any key to finish.\n";
			DWORD dw = GetLastError();
			return dw;
		}

		// создание события для отправки Receiver сигнала на готовность к работе
		hInEvent = CreateEvent(NULL, FALSE, FALSE, lpEventName);
		if (hInEvent == NULL)
		{
			//system("pause");
			//cout << GetLastError() << endl;
			return GetLastError();
		}

		wait_array[event_amount] = hInEvent;
		sender_array[event_amount++] = piSender;

		// ждем оповещение о готовности от каждого? процесса Sender  ??
		DWORD dwWaitResult = WaitForSingleObject(hInEvent, INFINITE); // 300 ?
		if (dwWaitResult != WAIT_OBJECT_0)
		{
			cout << dwWaitResult << endl;
			cout << "not ok wait\n";
			system("pause");		
			return dwWaitResult;
		}
		
		//или дальше ?
		/*WaitForSingleObject(piSender.hProcess, INFINITE);
		CloseHandle(piSender.hThread);
		CloseHandle(piSender.hProcess);*/

		//CloseHandle(hInEvent); // или дальше
	}

	//if (WaitForMultipleObjects(sender_amount, wait_array, TRUE, INFINITE) == WAIT_FAILED)
	//{
	//	cout << "Wait for multiple objects failed." << endl;
	//	cout << "Press any key to exit." << endl;
	//	system("pause");
	//	return 0; // ??
	//}

	for (int i = 0; i < sender_amount; i++) 
	{
		CloseHandle(wait_array[i]);
		WaitForSingleObject(sender_array[i].hProcess, INFINITE);
		CloseHandle(sender_array[i].hThread);
		CloseHandle(sender_array[i].hProcess);
	}

	delete[] wait_array;

	

		// ?? надо?
		//hWorkEvent = CreateEvent(NULL, FALSE, TRUE, lpWorkName); //посылаю разрешение на роботу всех Sender : сразу в сигнальном состоянии
		//SetEvent(hWorkEvent);
		//if (hWorkEvent == NULL)
		//{
		//	cout << "error = " <<  GetLastError() << endl;
		//	system("pause");
		//	return GetLastError();
		//}

		

		// читаем сообщения

		cout << "Press 1 to receive message or 2 to exit\n"; // in 1 string or 2 ??
		while (true)
		{
			int type;
			cin >> type;
			if (type == 1)
			{
				//string msg;
				char* mesg;// = const_cast<char*>(msg.c_str()); // ?? надо ли?

				// --
				WaitForSingleObject(hSemaphoreReceive, INFINITE);

				// захватываем мьютекс
				WaitForSingleObject(hMutex, INFINITE);

				file.open(filename, ios::binary | ios::in | ios::out);

				int end_pos = -1, beg_pos = -1, amount = 0;
				file.read((char*)&amount, sizeof(int));
				file.read((char*)&beg_pos, sizeof(int));
				file.read((char*)&end_pos, sizeof(int));

				if (amount == 0)
				{
					if (beg_pos == end_pos) // файл пустой
					{
						WaitForSingleObject(hSemaphoreSend, INFINITE);
						// wait ??
						//  
					}                       
					else 
					//{
						end_pos = 2;
					//}
				}

				file.seekg(0, end_pos); // +/- 1 ?? установила текущую позицию конца очереди
				file.read((char*)&mesg, sizeof(mesg)); // записала сообщение
				beg_pos += sizeof(mesg); // пересчитала новую позицию начала очереди
				file.seekg(0, 0); // на 0 месте хранится значение начала очереди
				file.write((char*)&beg_pos, sizeof(int)); // записала новое значение начала очереди

				file.close();
				// освобождаем мьютекс
				ReleaseMutex(hMutex);
				// ++ произвели единицу ресурса
				ReleaseSemaphore(hSemaphoreSend, 1, NULL); // NULL??
				//Sleep(500); // ??
			}
			else
				break;
		}

		
		//CloseHandle(hWorkEvent);

	/*for (int i = 0; i < sender_amount; i++)
	{
		CloseHandle(piSender.hThread);
		CloseHandle(piSender.hProcess);
	}*/

	CloseHandle(hSemaphoreReceive);
	CloseHandle(hSemaphoreSend);

	cout << "Receiver works\n";
	//cin.get(); // ??
	system("pause");

	return 0;
}