#include<iostream>
#include "SynqQueue.h"

SynqQueue::SynqQueue()
{
	beg_pos = 0;
	end_pos = 0;
	count = 0;
}

SynqQueue::~SynqQueue()
{
	CloseHandle(hSemaphoreProduce);
	CloseHandle(hSemaphoreConsume);
	delete queue;
}

void SynqQueue::insert(int value)
{
	//LeaveCriticalSection(&cs);
	WaitForSingleObject(hSemaphoreProduce, INFINITE);
	EnterCriticalSection(&cs);
	if (count == size)										// очередь заполнена
	{
		//LeaveCriticalSection(&cs);
		WaitForSingleObject(hSemaphoreConsume, INFINITE);
		//EnterCriticalSection(&cs);
	}
	else
		if (end_pos > size - 1)
			end_pos = 0;	
	queue[end_pos++] = value;
	count++;
	std::cout << "Produced number : " << value << "\n";
			LeaveCriticalSection(&cs);
	ReleaseSemaphore(hSemaphoreConsume, 1, NULL);
}

int SynqQueue::remove()
{
	//LeaveCriticalSection(&cs);
	WaitForSingleObject(hSemaphoreConsume, INFINITE);
	EnterCriticalSection(&cs);
	if (count == 0)											// очередь пуста
	{
		//LeaveCriticalSection(&cs);
		WaitForSingleObject(hSemaphoreProduce, INFINITE);
		//EnterCriticalSection(&cs);
	}
	else
		if (beg_pos > size - 1)
			beg_pos = 0;
	int value = queue[beg_pos++];
	count--;
	std::cout << "Consumed number : " << value << "\n";
			LeaveCriticalSection(&cs);
	ReleaseSemaphore(hSemaphoreProduce, 1, NULL);
	return value;
}

void SynqQueue::setSize(int sz)
{
	size = sz;
	queue = new int[size];
}

int SynqQueue::getSize()
{
	return size;
}

void SynqQueue::setReadyForWorkEvent(char* name)
{
	readyForWorkEvent = name;
}

char* SynqQueue::getReadyForWorkEvent()
{
	return readyForWorkEvent;
}

void SynqQueue::initializeCriticalSection()
{
	InitializeCriticalSection(&cs);
}

void SynqQueue::deleteCriticalSection()
{
	DeleteCriticalSection(&cs);
}

CRITICAL_SECTION* SynqQueue::getCriticalSection()
{
	return &cs;
}

int SynqQueue::createSemaphores()
{
	hSemaphoreConsume = CreateSemaphore(NULL, 0, size, "SemaphoreConsume"); 
	if (hSemaphoreConsume == NULL)
		return GetLastError();
	hSemaphoreProduce = CreateSemaphore(NULL, size, size, "SemaphoreProduce");
	if (hSemaphoreProduce == NULL)
		return GetLastError();
	return -1;
}
