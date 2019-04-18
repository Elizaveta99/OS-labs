#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <string>
#include <cstdio>
#include "SynqQueue.h"

using namespace std;

DWORD WINAPI Consumer(SynqQueue *queue) 
{
	CHAR canStartEventConsumer[] = "CanStartEventConsumer";
	//CRITICAL_SECTION *cs = (*queue).getCriticalSection();
	//EnterCriticalSection(cs);
	//cout << "Amount of consumed numbers by Consumer #" << (*queue).numberOfThread << " : ";
	//LeaveCriticalSection(cs);
	printf("Amount of consumed numbers by Consumer #%d : ", (*queue).numberOfThread);
	int amount;
	cin >> amount;
	//printf("name_consumer : %s\n", (*queue).getReadyForWorkEvent());

	// открытие события для отправки Main сигнала на готовность к работе
	HANDLE hReadyForWorkEventConsumer = OpenEvent(EVENT_ALL_ACCESS, FALSE, (*queue).getReadyForWorkEvent());
	if (hReadyForWorkEventConsumer == NULL)
	{
		cout << "Open event failed. Input any char to exit.\n" << endl;
		char c;
		cin >> c;
		return GetLastError();
	}

	SetEvent(hReadyForWorkEventConsumer);			// послали сигнал о своей готовности
	CloseHandle(hReadyForWorkEventConsumer);

	HANDLE hCanStartEventConsumer = OpenEvent(EVENT_ALL_ACCESS, FALSE, canStartEventConsumer);
	if (hCanStartEventConsumer == NULL)
	{
		cout << "Open event failed start. Input any char to exit.\n" << endl;
		char c;
		cin >> c;
		return GetLastError();
	}
	WaitForSingleObject(hCanStartEventConsumer, INFINITE);	// ждём сигнал на начало работы

	CRITICAL_SECTION *cs = (*queue).getCriticalSection();
	EnterCriticalSection(cs);  
	//cout << "AfterSetEventCanWorkConsumer!!!\n";
	for (int i = 0; i < amount; i++)
	{
		int value = (*queue).remove();
		cout << "Consumed number : " << value << "\n";
		Sleep(15);
	}
	LeaveCriticalSection(cs);
	CloseHandle(hCanStartEventConsumer);

	return 0;
}