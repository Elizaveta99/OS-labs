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

	HANDLE hCanStartEventProducer = CreateEvent(NULL, FALSE, FALSE, canStartEventProducer);
	if (hCanStartEventProducer == NULL)
		return GetLastError();
	HANDLE hCanStartEventConsumer = CreateEvent(NULL, FALSE, FALSE, canStartEventConsumer);
	if (hCanStartEventConsumer == NULL)
		return GetLastError();

	for (int i = 0; i < producer_threads; i++) 
	{
		string temp = "Producer";
		string producer_name = readyForWorkEvent + temp + to_string(i);
		char* producer_name_c = const_cast<char*>(producer_name.c_str());
		queue.setReadyForWorkEvent(producer_name_c);
		queue.numberOfThread = i + 1;
		HANDLE hReadyForWorkEventProducer = CreateEvent(NULL, FALSE, FALSE, producer_name_c);
		printf("Handle1 : %d\n", hReadyForWorkEventProducer);
		//printf("name_main_produser : %s\n", producer_name_c);
		if (hReadyForWorkEventProducer == NULL)
			return GetLastError();
		hThreadProducer[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Producer, &queue, 0, &IDThreadProducer[i]);
		WaitForSingleObject(hReadyForWorkEventProducer, INFINITE);
		CloseHandle(hReadyForWorkEventProducer);

		temp = "Consumer";
		string consumer_name = readyForWorkEvent + temp + to_string(i);
		char* consumer_name_c = const_cast<char*>(consumer_name.c_str());
		queue.setReadyForWorkEvent(consumer_name_c);
		HANDLE hReadyForWorkEventConsumer = CreateEvent(NULL, FALSE, FALSE, consumer_name_c);
		if (hReadyForWorkEventConsumer == NULL)
			return GetLastError();
		hThreadConsumer[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Consumer, &queue, 0, &IDThreadConsumer[i]);
		WaitForSingleObject(hReadyForWorkEventConsumer, INFINITE);
		CloseHandle(hReadyForWorkEventConsumer);
	}

	// если здесь, то дождались сигнала на готовность от всех producer и consumer
	queue.initializeCriticalSection();					// или до создания потоков?
	int hSemaphores = queue.createSemaphores();
	if (hSemaphores != -1)
		return hSemaphores;

	//cout << "SetEvent(hCanStartEventProducer)!!!!!!!!\n";

	SetEvent(hCanStartEventProducer); // всем установилось?
	CloseHandle(hCanStartEventProducer);

	SetEvent(hCanStartEventConsumer);
	/*for (int i = 0; i <  consumer_threads; i++)
		SetEvent(hCanStartEventConsumer);*/
	CloseHandle(hCanStartEventConsumer);


	for (int i = 0; i < producer_threads; i++)
	{
		WaitForSingleObject(hThreadProducer[i], INFINITE);
		WaitForSingleObject(hThreadConsumer[i], INFINITE);
		CloseHandle(hThreadConsumer[i]);
		CloseHandle(hThreadProducer[i]);
	}
	//cout << "!!!!!!!!!!!!!!!SetEvent(hCanStartEventProducer)!!!!!!!!\n";


	/*for (int i = 0; i < consumer_threads; i++)
	{
		WaitForSingleObject(hThreadConsumer[i], INFINITE);
		CloseHandle(hThreadConsumer[i]);
	}*/
	delete[] hThreadProducer;
	delete[] hThreadConsumer;

	cout << "Press any key to exit ";
	cin.ignore();
	cin.get();

	return 0;
}