#pragma once
#include <windows.h>

class SynqQueue
{
private:
	int size;
	int* queue; 
	int beg_pos;
	int end_pos;
	int count;
	char *readyForWorkEvent;
	CRITICAL_SECTION cs;
	HANDLE hSemaphoreProduce;
	HANDLE hSemaphoreConsume;
public:
	SynqQueue();
	virtual ~SynqQueue(); 
	int numberOfThread;
	void insert(int value);
	int remove(); 
	void setSize(int sz);
	int getSize();
	/*HANDLE hSemaphoreProduce;
	HANDLE hSemaphoreConsume;*/
	void setReadyForWorkEvent(char* name);
	char* getReadyForWorkEvent();
	void initializeCriticalSection();
	void deleteCriticalSection();
	CRITICAL_SECTION* getCriticalSection();
	int createSemaphores();
};

