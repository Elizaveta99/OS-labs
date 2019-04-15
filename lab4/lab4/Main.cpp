#include <windows.h>
#include <iostream>
#include <string>
#include <cstdio>
#include "SynqQueue.h"

using namespace std;

DWORD WINAPI Producer(SynqQueue *queue);
DWORD WINAPI Consumer(SynqQueue *queue);

CHAR readyForWorkEvent[] = "ReadyForWork";
CHAR canStartEventProducer[] = "CanStartEventProducer";
CHAR canStartEventConsumer[] = "CanStartEventConsumer";

int main()
{
	cout << "Queue size : ";
	int size;
	cin >> size;
	SynqQueue queue;
	queue.setSize(size);								//создание очереди
	cout << "Amount Producer's threads : ";
	int producer_threads;
	cin >> producer_threads;
	cout << "Amount Consumer's threads : ";
	int consumer_threads;
	cin >> consumer_threads;

	//создание требуемого количества потоков producer и consumer
	HANDLE *hThreadProducer, *hThreadConsumer;
	DWORD *IDThreadProducer, *IDThreadConsumer;
	hThreadProducer = new HANDLE[producer_threads];
	hThreadConsumer = new HANDLE[consumer_threads];
	IDThreadProducer = new DWORD[producer_threads];
	IDThreadConsumer = new DWORD[consumer_threads];

	for (int i = 0; i < min(producer_threads, consumer_threads); i++) // ??
	{
		string name = readyForWorkEvent + to_string(i);
		char* name_c = const_cast<char*>(name.c_str());
		queue.setReadyForWorkEvent(name_c);
		queue.numberOfThread = i + 1;

		HANDLE hReadyForWorkEvent = CreateEvent(NULL, FALSE, FALSE, name_c);
		//cout << "Handle1 : " << hReadyForWorkEvent << "\n";
		printf("Handle1 : %d\n", hReadyForWorkEvent);
		//cout << "name_main_produser : " << name_c << "\n";
		printf("name_main_produser : %s\n", name_c);
		if (hReadyForWorkEvent == NULL)
			return GetLastError();
		hThreadProducer[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Producer, &queue, 0, &IDThreadProducer[i]);
		WaitForSingleObject(hReadyForWorkEvent, INFINITE);
		CloseHandle(hReadyForWorkEvent);

		// такое же имя - будет работать?
		hReadyForWorkEvent = CreateEvent(NULL, FALSE, FALSE, name_c);
		if (hReadyForWorkEvent == NULL)
			return GetLastError();
		hThreadConsumer[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Consumer, &queue, 0, &IDThreadConsumer[i]);
		WaitForSingleObject(hReadyForWorkEvent, INFINITE);
		CloseHandle(hReadyForWorkEvent);

		/*wait_array_producer[event_amount] = hReadyForWorkEvent;
		wait_array_consumer[event_amount] = hReadyForWorkEvent;*/
	}

	// если здесь, то дождались сигнала на готовность от всех producer и consumer
	queue.initializeCriticalSection();					// или до создания потоков?
	int hSemaphores = queue.createSemaphores();
	if (hSemaphores != -1)
		return hSemaphores;

	HANDLE hCanStartEventProducer = CreateEvent(NULL, FALSE, FALSE, canStartEventProducer);
	if (hCanStartEventProducer == NULL)
		return GetLastError();
	SetEvent(hCanStartEventProducer); // всем установилось?
	CloseHandle(hCanStartEventProducer);

	HANDLE hCanStartEventConsumer = CreateEvent(NULL, FALSE, FALSE, canStartEventConsumer);
	if (hCanStartEventConsumer == NULL)
		return GetLastError();
	SetEvent(hCanStartEventConsumer); // всем установилось?
	CloseHandle(hCanStartEventConsumer);

	//for (int i = 0; i < min(producer_threads, consumer_threads); i++) // ??
	//{ }

	for (int i = 0; i < producer_threads; i++)
	{
		WaitForSingleObject(hThreadProducer[i], INFINITE);
		CloseHandle(hThreadProducer[i]);
	}
	for (int i = 0; i < consumer_threads; i++)
	{
		WaitForSingleObject(hThreadConsumer[i], INFINITE);
		CloseHandle(hThreadConsumer[i]);
	}
	delete[] hThreadProducer;
	delete[] hThreadConsumer;

	cout << "Press any key to exit ";
	cin.ignore();
	cin.get();

	return 0;
}