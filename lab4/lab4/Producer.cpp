#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <string>
#include <cstdio>
#include "SynqQueue.h"

using namespace std;

DWORD WINAPI Producer(SynqQueue *queue) 
{
	CHAR canStartEventProducer[] = "CanStartEventProducer";
	printf("Amount and value of produced numbers by Producer #%d : ", (*queue).numberOfThread);
	int amount, value;
	cin >> amount >> value;

	// открытие события для отправки Main сигнала на готовность к работе
	//cout << "name_producer : " << (*queue).getReadyForWorkEvent() << "\n";
	//printf("name_producer : %s\n", (*queue).getReadyForWorkEvent());
	HANDLE hReadyForWorkEventProducer = OpenEvent(EVENT_ALL_ACCESS, FALSE, (*queue).getReadyForWorkEvent());
	if (hReadyForWorkEventProducer == NULL)
	{
		cout << "Open event failed. Input any char to exit.\n" << endl;
		char c;
		cin >> c;
		return GetLastError();
	}

	SetEvent(hReadyForWorkEventProducer);					// послали сигнал о своей готовности
	CloseHandle(hReadyForWorkEventProducer);   

	HANDLE hCanStartEventProducer = OpenEvent(EVENT_ALL_ACCESS, FALSE, canStartEventProducer);
	if (hCanStartEventProducer == NULL)
	{
		cout << "Open event failed start. Input any char to exit.\n" << endl;
		char c;
		cin >> c;
		return GetLastError();
	}
	//cout << "??????????WaitForSingleObject(hCanStartEventProducer\n";
	WaitForSingleObject(hCanStartEventProducer, INFINITE);	// ждём сигнал на начало работы

	CRITICAL_SECTION *cs = (*queue).getCriticalSection();
	EnterCriticalSection(cs);  
	//cout << "AfterSetEventCanWork!!!\n";
	for (int i = 0; i < amount; i++)
	{
		cout << "Produced number : " << value << "\n";
		(*queue).insert(value);
		Sleep(15);
	}
	LeaveCriticalSection(cs);
	//cout << "AfterSetEventCanWork222222222!!!\n";
	CloseHandle(hCanStartEventProducer);

	return 0;
}