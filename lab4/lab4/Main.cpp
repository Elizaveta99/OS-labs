#include <windows.h>
#include <iostream>
#include <string>
#include "SynqQueue.h"

using namespace std;

DWORD WINAPI Producer(SynqQueue *queue);
DWORD WINAPI Consumer(SynqQueue *queue);

const CHAR readyForWorkEvent[] = "ReadyForWork";
const CHAR canStartEventProducer[] = "CanStartEventProducer";
const CHAR canStartEventConsumer[] = "CanStartEventConsumer";

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

	queue.initializeCriticalSection();					
	int hSemaphores = queue.createSemaphores();
	if (hSemaphores != -1)
		return hSemaphores;

	for (int i = 0; i < producer_threads; i++)
	{
		SetEvent(hCanStartEventProducer); 
		SetEvent(hCanStartEventConsumer);
	}

	CloseHandle(hCanStartEventProducer);
	CloseHandle(hCanStartEventConsumer);
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
	queue.deleteCriticalSection();
	delete[] hThreadProducer;
	delete[] hThreadConsumer;

	cout << "Press any key to exit ";
	cin.ignore();
	cin.get();

	return 0;
}