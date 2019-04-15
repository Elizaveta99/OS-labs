#include<iostream>
#include "SynqQueue.h"

SynqQueue::SynqQueue()
{
}

SynqQueue::~SynqQueue()
{
	std::cout << "Dectructor\n";
	DeleteCriticalSection(&cs);
	CloseHandle(hSemaphoreProduce);
	CloseHandle(hSemaphoreConsume);
	delete[] readyForWorkEvent;
	delete queue;
}

void SynqQueue::insert(int value)
{
	WaitForSingleObject(hSemaphoreProduce, INFINITE);
	if (count == size)										// очередь заполнена
		WaitForSingleObject(hSemaphoreConsume, INFINITE);
	else
		if (end_pos > size - 1)
			end_pos = 0;	
	queue[end_pos++] = value;
	count++;
	ReleaseSemaphore(hSemaphoreConsume, 1, NULL);
}

int SynqQueue::remove()
{
	WaitForSingleObject(hSemaphoreConsume, INFINITE);
	if (count == 0)											// очередь пуста
		WaitForSingleObject(hSemaphoreProduce, INFINITE);
	else
		if (beg_pos > size - 1)
			beg_pos = 0;
	int value = queue[beg_pos++];
	count--;
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

CRITICAL_SECTION* SynqQueue::getCriticalSection()
{
	return &cs;
}

int SynqQueue::createSemaphores()
{
	hSemaphoreConsume = CreateSemaphore(NULL, size, size, "SemaphoreConsume");
	if (hSemaphoreConsume == NULL)
		return GetLastError();
	hSemaphoreProduce = CreateSemaphore(NULL, 0, size, "SemaphoreProduce");
	if (hSemaphoreProduce == NULL)
		return GetLastError();
	return -1;
}
