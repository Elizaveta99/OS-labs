#pragma once
#include <windows.h>

class SynqQueue
{
private:
	int size;
	int* queue; 
	int beg_pos = 0;
	int end_pos = 0;
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
	void setReadyForWorkEvent(char* name);
	char* getReadyForWorkEvent();
	void initializeCriticalSection();
	CRITICAL_SECTION* getCriticalSection();
	int createSemaphores();
};

